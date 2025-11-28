#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
RTL="$ROOT/rtl"
OUT="$ROOT/outputs"
SCRIPT="$ROOT/synth.ys"

mkdir -p "$RTL"
mkdir -p "$OUT"

echo "=== Synthesis starting ==="
echo "RTL dir: $RTL"
echo "OUT dir: $OUT"

# Run yosys and capture stdout/stderr
yosys -s "$SCRIPT" 2>&1 | tee "$OUT/yosys_run.log"

echo "=== Synthesis finished ==="
echo "Outputs in: $OUT"
