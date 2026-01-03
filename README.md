# Flexagon-DSA — Memory-Prefetch Extension of Flexagon NN Accelerator

> **Project (research internship / research assistant work)** — Engineering extension of the original **Flexagon** accelerator paper. This repository contains a synthesizable SystemVerilog memory-control extension ("FlexPipe") with a pointer-walking prefetch DMA, strict-priority arbiter, configuration manager, a cycle-accurate Verilator harness, a high-level C simulator, an analysis pipeline that produces publication-ready plots, and a synthesis (Yosys) flow with resource reporting.

**Primary contributions in this repo:**

* Implemented a prefetch-driven memory-control path that integrates with the Flexagon architecture.
* Built a reproducible evaluation pipeline (simulator → analysis → plots).
* Performed technology-independent synthesis and produced resource metrics.

**Status:** Implementation and evaluation complete; per-cycle CSV instrumentation in the harness is tracked and will be fixed (see Roadmap).

---

## Table of contents

1. [Overview & Motivation](#overview--motivation)
3. [Quickstart — reproduce figures & synthesis](#quickstart--reproduce-figures--synthesis)
4. [Detailed design & modules](#detailed-design--modules)
5. [Simulator & analysis pipeline (how results were produced)](#simulator--analysis-pipeline-how-results-were-produced)
6. [Instrumentation & per-request/per-cycle logs](#instrumentation--per-requestper-cycle-logs-notes-for-reviewers)
7. [Evaluation: numeric metrics & interpretation](#evaluation-numeric-metrics--interpretation)
8. [Synthesis (Yosys) results & what they mean](#synthesis-yosys-results--what-they-mean)
9. [File structure (expanded)](#file-structure-expanded)
10. [How to run everything — commands & tips](#how-to-run-everything--commands--tips)
11. [Roadmap & tracked issues](#roadmap--tracked-issues)
12. [Authorship & acknowledgement](#authorship--acknowledgement)
13. [License & contact](#license--contact)

---

## Overview & Motivation

The original Flexagon paper provided a flexible compute substrate for NN accelerators. In practice, many workloads — especially pointer-heavy layers and Transformer-style attention — are dominated by memory stalls (pointer-walks, irregular loads). The FlexPipe extension in this repo targets that gap by:

* adding a pointer-walking **prefetcher** that reads pointer addresses and issues data beats ahead of time,
* providing a **shadow/active flip** configuration manager to change layer contexts atomically,
* mediating memory traffic with a **strict-priority arbiter** so active compute requests retain priority while prefetchers opportunistically use spare DRAM bandwidth.

This is an **engineering extension** (not a new architecture claim) — the goal is to validate the benefit of compact prefetch logic when integrated with a flexible accelerator.

---

## What this repo contains (detailed)

* **RTL (SystemVerilog)**: synthesizable modules implementing the FlexPipe control path.

  * `rtl/src/` contains `flexpipe_top.sv`, `config_manager.sv`, `prefetch_dma.sv`, `memory_arbiter.sv`, `dram_model.sv`, `dummy_core.sv`, and `flexpipe_pkg.sv`.
  * `rtl/harness/` contains the Verilator C++ harness used to drive the RTL and capture logs/waves.

* **High-level simulator**: `simulator/` contains a C model that simulates workloads quickly, producing per-layer CSVs used by the analysis pipeline.

  * `simulator/src/` — C source (timing model, workload runner)
  * `simulator/inputs/` — JSON workload descriptions (e.g., `vgg16_tuned.json`)
  * `simulator/outputs/` — per-model CSVs consumed by the analysis script.

* **Analysis**: `analysis/analysis.py` converts per-model CSVs into plots (stacked runtime bars, normalized speedups, DRAM busy % and per-layer stall breakdowns). `analysis/results/` contains the generated PNG figures.

* **Synthesis**: `synthesis/` is the Yosys flow and includes `synth.ys` and a copy of the RTL for synthesis. `synthesis/outputs/` contains `flexpipe.json` and `yosys_report.json`.

* **Makefiles & scripts**: top-level convenience files to run the simulator, analysis, Verilator run and synthesis. See `How to run everything` below.

---

## Quickstart — reproduce figures & synthesis

> Note: the repo already includes generated figures (in `analysis/results/`) and Yosys outputs (in `synthesis/outputs/`). If you want to rerun locally, follow these commands.

### Requirements

* Python 3.8+; pip packages: `pandas`, `numpy`, `matplotlib`.
* Verilator (for RTL execution; optional to reproduce plots if you only want C-sim outputs).
* Yosys >= 0.59 recommended for SystemVerilog support.

Install python deps:

```bash
pip3 install pandas numpy matplotlib
```

### Recreate analysis plots (fast)

```bash
cd analysis
python3 analysis.py
# Output: analysis/results/*.png
```

### Run the high-level simulator

```bash
cd simulator
make run_all   # runs all included workloads, writes simulator/outputs/*.csv
```

This produces per-layer CSVs (including a `TOTAL` row) which `analysis.py` consumes.

### Run the Verilator harness (RTL simulation)

```bash
cd rtl
make run       # builds and runs the Verilator model
# outputs in rtl/outputs/: per_cycle_log.csv, verilator_run_log.txt, flexpipe.vcd
```

Note: `per_cycle_log.csv` deep snapshot may be zero-filled in current runs (see Instrumentation notes); per-request timing in `verilator_run_log.txt` is valid and included.

### Run synthesis (Yosys)

```bash
cd synthesis
./synth.sh     # runs Yosys flow and writes synthesis/outputs/flexpipe.json
make report    # generates analysis JSON outputs/yosys_report.json using analyzer
```

---

## Detailed design & modules

(Short summary plus key signals and behaviors. See RTL files for full details.)

### `flexpipe_pkg.sv`

* Global typedefs and parameters (ADDR_WIDTH, DATA_WIDTH, layer_config_t, mem_req_t, mem_resp_t). Keep this file first in synthesis/flows.

### `config_manager.sv`

* Small AXI-lite-style write FSM that assembles `shadow_config_r` then performs an atomic flip to `active_config_r` on `flip_ack` when `core_safe_to_flip` and `no_outstanding_active` are true.
* Fields: `compute_cycles`, `config_bits`, `pointer_walks`, `data_size`, `base_addr`, `valid`, `epoch`.

### `prefetch_dma.sv`

* FSM that reads pointer addresses (`PTR_REQ`), waits for pointer response (`PTR_WAIT`), issues data read (`DATA_REQ`), and counts bytes fetched. Supports pointer_walks iterations.
* Exports `busy` and `bytes_fetched` for top-level instrumentation.

### `memory_arbiter.sv`

* Strict-priority arbiter: active core requests forwarded first (ID MSB = 0), prefetch requests second (ID MSB = 1). Tracks outstanding counts and routes responses back.

### `dram_model.sv`

* Simple latency + transfer model. Accepts requests into a small FIFO, simulates fixed latency + transfer cycles, and emits single-beat responses (payload mocked).

### `flexpipe_top.sv`

* Connects modules, exposes `dbg_cycle_count` instrumentation, and wires the master DRAM interface.

### `harness/verilator_harness.cpp`

* Drives RTL clocks, flips, and writes per-request logs. Packs requests into `mem_master_req[]` words and reads `mem_master_resp[]`. Dumps `per_cycle_log.csv`, `verilator_run_log.txt`, and VCD.

---

## Simulator & analysis pipeline (how results were produced)

1. **Workload description** — JSON files (`simulator/inputs/`) describe the network layers (compute cycles, pointer_walks, data_size). `scripts/prepare_vgg_inputs.py` can convert JSON→workload.txt for the harness.

2. **High-level C-simulator** — runs faster evaluation over workloads, simulates stalls (config, drain, data) and DRAM busy fraction, and outputs per-layer CSVs in `simulator/outputs/`.

3. **Analysis** — `analysis/analysis.py` reads CSVs and produces four main plot families:

   * Baseline runtime breakdown (compute vs stalls) [log y-scale]
   * Normalized runtime/speedup plots
   * DRAM busy percentages across models/modes
   * Per-layer stall breakdowns (baseline vs FlexPipe)

All these outputs are included in `analysis/results/` in the repo.

---

## Instrumentation — per-request & per-cycle logs

### What is recorded and what is reliable

* **Per-request timestamps:** The Verilator harness logs reliable per-request issue/response timestamps in `rtl/outputs/verilator_run_log.txt`. Example excerpt included in repository:

```
REQ[0] id=0 issued_t=2016 resp_t=2030
...
Measured work cycles (first issue -> last response): 14708
RTL dbg_cycle_count = 7354
Issued 10 Responses 10 total_cycles 16824
```

These per-request metrics were used to validate timing and align the harness with the high-level model.

* **Per-cycle CSV snapshot (`per_cycle_log.csv`)**: an intended deep-dive trace with pre/post snapshots of `mem_master_req`, `mem_master_resp`, and `active_config`. In early runs this CSV contained zeros due to a harness-to-RTL snapshot timing mismatch (instrumentation race during reset/initialization). The harness, snapshot code, and RTL register names are tracked and will be adjusted; see Roadmap.

---

## Evaluation: numeric metrics & interpretation

### Synthesis / hardware footprint (Vivado: Xilinx Zynq-7020 at 256 MHz)

* **Top module**: `flexpipe_top`
* **Registers (FFs)**: **311**
* **LUTs**: **87**
* **Worst Negative Slack (WNS)**: **+0.107ns at 250MHz**
* **Total cells**: **634** (generic logic cells, yosys)
* **Wire bits**: **4,717** (yosys flattened)
* **Inferred memories**: 0 (small FIFO → flops)
* **On-Chip Power**: **377mW**

**Interpretation:** the FlexPipe control path is compact and easy to add to an accelerator die; the design is clearly hardware-feasible.

### Speedups & runtime metrics (from simulator + analysis plots)

* **VGG-16 (Tuned)**: **1.33× speedup** (FlexPipe vs baseline), significant stall reduction; DRAM busy % increased to **~51%** under FlexPipe, indicating prefetcher uses memory bandwidth to hide latency.
* **DistilBERT (Tuned)**: **1.10× speedup**; stall fraction reduced (baseline ~90.7% stalled → FlexPipe reduces this substantially); DRAM busy % increased to **~95%** in tuned FlexPipe run.

**Representative harness per-request measurements (verilator_run_log.txt)**

```
REQ[0] id=0 issued_t=2016 resp_t=2030
REQ[1] id=1 issued_t=4036 resp_t=4050
...
Measured work cycles (first issue -> last response): 14708
RTL dbg_cycle_count = 7354
Issued 10 Responses 10 total_cycles 16824
```

**How to read these numbers:** the simulator-derived results (CSV→plots) and the harness per-request timestamps are consistent and both indicate meaningful improvements when FlexPipe is enabled. Per-cycle snapshots are being fixed — they are included as-is in the repo for transparency.

---

## Synthesis (Yosys) results & what they mean

* We synthesized using Yosys (version used: 0.59+ recommended). The tool produced a JSON netlist (`synthesis/outputs/flexpipe.json`) which the repo contains.
* Counts: 311 FFs, 634 total cells, 4.7k wire bits — these numbers are technology-independent but indicate the design is small. Mapping to a real process (e.g., sky130) would allow area and timing estimates.

---

## File structure (expanded)

```
Flexagon-DSA/
├── analysis/
│   ├── analysis.py               # plotting & metrics aggregation (generates analysis/results/*.png)
│   └── results/                  # generated PNGs (check-in for reproduction)
├── simulator/
│   ├── src/                      # C source timing model + runner
│   │   ├── main.c
│   │   ├── timing_model.c
│   │   └── utils.c
│   ├── inputs/                   # JSON workload descriptions (vgg16_tuned.json, etc.)
│   └── outputs/                  # CSVs produced by runs: <model>_<mode>_results.csv
├── rtl/
│   ├── src/                      # SystemVerilog RTL
│   │   ├── flexpipe_pkg.sv
│   │   ├── flexpipe_top.sv
│   │   ├── config_manager.sv
│   │   ├── prefetch_dma.sv
│   │   ├── memory_arbiter.sv
│   │   ├── dram_model.sv
│   │   └── dummy_core.sv
│   └── harness/                  # Verilator harness
│       ├── verilator_harness.cpp
│       └── json.hpp
├── synthesis/
│   ├── rtl/                      # copy of RTL used for Yosys synth
│   ├── synth.ys                  # Yosys script
│   └── outputs/                  # flexpipe.json, yosys_report.json
├── scripts/                      # helpers (prepare_vgg_inputs.py)
├── Makefile                      # top-level run targets (optional)
├── Flexagon.pdf                  # Orignal Flexagon Architecture Paper
└── README.md                     # this file
```

**Important files to inspect for reproduction:**

* `simulator/inputs/vgg16_tuned.json` — workload parameters
* `simulator/outputs/*_results.csv` — raw CSVs used by analysis.py
* `analysis/analysis.py` — plotting code (adjustable)
* `rtl/harness/verilator_harness.cpp` — harness that packs mem_req / mem_resp words and writes logs
* `synthesis/outputs/flexpipe.json` — synthesized netlist for resource counting

---

## How to run everything — commands & tips

### Generate simulator outputs & plots (fast)

```bash
# run all simulator workloads
cd simulator
make run_all

# run analysis to convert CSV -> PNG
cd ../analysis
python3 analysis.py
```

### Run Verilator harness (for RTL verification)

```bash
cd rtl
make run
# artifacts: rtl/outputs/per_cycle_log.csv, rtl/outputs/verilator_run_log.txt, rtl/outputs/flexpipe.vcd
# view VCD in GTKWave: gtkwave rtl/outputs/flexpipe.vcd
```

### Synthesis (Yosys)

```bash
cd synthesis
./synth.sh
make report
# outputs: synthesis/outputs/flexpipe.json, synthesis/outputs/yosys_report.json
```

**Troubleshooting tips:**

* If Yosys errors on SystemVerilog constructs, use Yosys >= v0.59 or build from git to get improved SystemVerilog support.
* If `per_cycle_log.csv` is all zeros: the harness snapshot code may be capturing arrays before Verilator drives the RTL memory-mapped arrays; consult `harness/verilator_harness.cpp` and ensure the correct top-level signal names and array dimensions are used.

---

## Roadmap & tracked issues

1. **Fix `per_cycle_log.csv` instrumentation**: resolve harness vs RTL naming/ordering mismatch; add a test that validates a non-zero snapshot shortly after reset release.
2. **Connect to a realistic active core model**: replace `dummy_core` with a simple traffic generator to stress the arbiter.
3. **Map to a real cell library**: run liberty-based flow to extract area and timing.
4. **Adaptive prefetch policies**: evaluate dynamic pointer-walk lengths and backpressure-aware prefetch throttling.
5. **Scalability**: multi-stream prefetcher and fairness policies.

---

## Authorship & acknowledgement

* **Research Assistant** Kush Kapoor
* **Supervisor:** Prof Dr. Ajai Kumar Gautam (Delhi Technological University)

This work was performed as part of a research internship / RA assignment.

---

## License & contact

MIT License — see `LICENSE`.

**Email:** kushkapoor.kk1234@gmail.com

---

*End of README*
