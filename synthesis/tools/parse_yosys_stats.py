#!/usr/bin/env python3
"""
analyze_yosys_json.py

Usage:
  python3 tools/analyze_yosys_json.py outputs/flexpipe.json > outputs/yosys_report.json

Reads a Yosys JSON netlist and computes:
  - wires_bits: total number of wire bits in top module
  - nets: number of named nets
  - regs: number of flip-flop cells (heuristic: DFF/$dff/$_DFF_* types)
  - cells_total: total number of cells in top
  - cells_by_type: breakdown by cell type
  - memories: number of memories, bits, words (if present)

This is independent of Yosys log text and works across versions.
"""

import sys
import json
from collections import Counter


def analyze_top(json_path: str, top_name: str = None):
    with open(json_path, "r") as f:
        data = json.load(f)

    modules = data.get("modules", {})
    if not modules:
        raise SystemExit("No modules found in JSON.")

    # Pick top module: if not specified, choose the one with 'attributes.top' or the last one
    if top_name is None:
        for name, mod in modules.items():
            attrs = mod.get("attributes", {})
            if attrs.get("top", "0") == "1":
                top_name = name
                break
        if top_name is None:
            # fallback: first module name
            top_name = sorted(modules.keys())[-1]

    top = modules[top_name]

    # --- Wires / nets ---
    netnames = top.get("netnames", {})
    nets = len(netnames)
    wire_bits = 0
    for net in netnames.values():
        bits = net.get("bits", [])
        wire_bits += len(bits)

    # --- Cells ---
    cells = top.get("cells", {})
    cells_total = len(cells)
    cell_types = Counter()
    reg_count = 0
    # Heuristic: treat DFF-like cells as regs
    def is_ff(cell_type: str) -> bool:
        t = cell_type.lower()
        return (
            t.startswith("$_dff_")
            or t.startswith("$dff")
            or "dff" in t
        )

    for cell in cells.values():
        ctype = cell.get("type", "")
        cell_types[ctype] += 1
        if is_ff(ctype):
            reg_count += 1

    # --- Memories ---
    memories = top.get("memories", {})
    mem_count = len(memories)
    mem_bits = 0
    mem_words = 0
    for mem in memories.values():
        width = mem.get("width", 0)
        size = mem.get("size", 0)
        mem_bits += width * size
        mem_words += size

    report = {
        "top_module": top_name,
        "nets": nets,
        "wire_bits": wire_bits,
        "regs": reg_count,
        "cells_total": cells_total,
        "cells_by_type": dict(cell_types),
        "memories": {
            "count": mem_count,
            "bits": mem_bits,
            "words": mem_words,
        },
    }
    return report


def main():
    if len(sys.argv) < 2:
        print("Usage: analyze_yosys_json.py <outputs/flexpipe.json>", file=sys.stderr)
        sys.exit(2)

    json_path = sys.argv[1]
    rep = analyze_top(json_path)
    print(json.dumps(rep, indent=2))


if __name__ == "__main__":
    main()
