#!/usr/bin/env python3
"""
prepare_vgg_inputs.py

Create inputs/workload.txt for the Verilator harness from a JSON description.

Default locations (relative to this script):
  JSON in:  ../simulator/inputs/vgg16_tuned.json
  OUT file: ../inputs/workload.txt

Supported JSON shapes:

1) Explicit requests:
   {
     "requests": [
       {"id":0,"epoch":0,"addr":4096,"len":256,"data_hex":"0x..."},
       ...
     ]
   }

2) Top-level list:
   [
     {"id":0,"epoch":0,"addr":4096,"len":256},
     ...
   ]

3) Generic "layers" (older style):
   {
     "layers": [
       {"base_addr":4096,"len":256,"epoch":0, "data_hex":"0x..."},
       ...
     ],
     "base_addr": 4096   # optional default
   }

4) Your VGG-16 JSON (this file):
   {
     "network": "VGG-16-TUNED",
     "layers": [
       { "layer_name": "Conv1_1", "compute_cycles": 50000,
         "config_bits": 2000000, "pointer_walks": 256, "data_size": 10240 },
       ...
     ]
   }

   → For each layer i we generate:
       id      = i
       epoch   = 0
       addr    = base_addr + i * addr_stride
       len     = data_size
       data_hex empty (harness synthesizes a pattern)

Output workload format (one request per line):
  <id> <epoch> <addr> <len> [data_hex]

Example line:
  0 0 0x1000 256 0x01010101

Usage:
  cd rtl
  python3 scripts/prepare_vgg_inputs.py \
      --json simulator/inputs/vgg16_tuned.json \
      --out  inputs/workload.txt
"""

from __future__ import annotations
import argparse
import json
import os
import sys
from typing import Any, Dict, List


def normalize_addr(a: Any) -> int:
    if isinstance(a, str):
        try:
            return int(a, 0)  # supports "0x1000" or "4096"
        except Exception:
            return int(a)
    return int(a)


def make_requests_from_json(data: Any) -> List[Dict[str, Any]]:
    """Convert various JSON shapes into a unified list of request dicts."""
    reqs: List[Dict[str, Any]] = []

    # 1) Explicit "requests" field
    if isinstance(data, dict) and "requests" in data:
        for i, r in enumerate(data["requests"]):
            rid    = int(r.get("id", i))
            epoch  = int(r.get("epoch", 0))
            addr   = normalize_addr(r.get("addr", 0))
            length = int(r.get("len", r.get("length", 256)))
            data_hex = r.get("data_hex", "")
            reqs.append({
                "id": rid,
                "epoch": epoch,
                "addr": addr,
                "len": length,
                "data_hex": data_hex
            })
        return reqs

    # 2) Top-level list of request-like objects
    if isinstance(data, list):
        for i, r in enumerate(data):
            rid    = int(r.get("id", i))
            epoch  = int(r.get("epoch", 0))
            addr   = normalize_addr(r.get("addr", 0))
            length = int(r.get("len", r.get("length", 256)))
            data_hex = r.get("data_hex", "")
            reqs.append({
                "id": rid,
                "epoch": epoch,
                "addr": addr,
                "len": length,
                "data_hex": data_hex
            })
        return reqs

    # 3) "layers" field – handle both generic and VGG-style
    if isinstance(data, dict) and "layers" in data:
        layers = data["layers"]
        # Defaults for address mapping (can be tuned if needed)
        base_default   = int(data.get("base_addr", 0x1000))
        addr_stride    = int(data.get("addr_stride", 0x1000))
        default_epoch  = int(data.get("epoch", 0))

        for i, L in enumerate(layers):
            rid   = int(L.get("id", i))
            epoch = int(L.get("epoch", default_epoch))

            # Address: prefer explicit base_addr if present, otherwise default + stride
            if "base_addr" in L:
                addr = normalize_addr(L["base_addr"])
            else:
                addr = base_default + i * addr_stride

            # Length: priority order - len / bytes / data_size
            if "len" in L:
                length = int(L["len"])
            elif "bytes" in L:
                length = int(L["bytes"])
            elif "data_size" in L:
                # <-- This is the key for YOUR JSON
                length = int(L["data_size"])
            else:
                length = 256  # fallback

            data_hex = L.get("data_hex", "")

            reqs.append({
                "id": rid,
                "epoch": epoch,
                "addr": addr,
                "len": length,
                "data_hex": data_hex
            })
        return reqs

    raise ValueError("JSON format not recognized. Expect 'requests', 'layers' or a top-level list.")


def write_workload(reqs: List[Dict[str, Any]], out_path: str) -> None:
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    with open(out_path, "w") as f:
        for r in reqs:
            addr = r["addr"]
            addr_str = hex(addr) if isinstance(addr, int) else str(addr)
            if r.get("data_hex"):
                f.write(f'{r["id"]} {r["epoch"]} {addr_str} {r["len"]} {r["data_hex"]}\n')
            else:
                f.write(f'{r["id"]} {r["epoch"]} {addr_str} {r["len"]}\n')


def main() -> None:
    here = os.path.dirname(os.path.abspath(__file__))
    default_json = os.path.normpath(os.path.join(here, "../simulator/inputs/vgg16_tuned.json"))
    default_out  = os.path.normpath(os.path.join(here, "../inputs/workload.txt"))

    ap = argparse.ArgumentParser(description="Prepare workload.txt for FlexPipe harness from JSON")
    ap.add_argument("--json", "-j", default=default_json,
                    help="Input JSON describing VGG / requests (default: %(default)s)")
    ap.add_argument("--out", "-o", default=default_out,
                    help="Output workload file (default: %(default)s)")
    args = ap.parse_args()

    if not os.path.exists(args.json):
        print(f"ERROR: input JSON not found: {args.json}")
        sys.exit(2)

    with open(args.json, "r") as jf:
        data = json.load(jf)

    reqs = make_requests_from_json(data)
    if len(reqs) == 0:
        print("WARNING: no requests found in JSON. Output will be empty.")
    write_workload(reqs, args.out)
    print(f"Wrote {len(reqs)} requests to {args.out}")


if __name__ == "__main__":
    main()
