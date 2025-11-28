# Makefile for FlexPipe RTL simulation with Verilator (no synthesis)
# Run from rtl/:
#   make          # prepare inputs, build, run
#   make run      # same as above
#   make clean    # remove Verilator build
#   make distclean# also remove workload + logs

TOP        := flexpipe_top
OBJ_DIR    := obj_dir
OUT_DIR    := outputs
INPUTS_DIR := inputs

# JSON describing the VGG16 workload (relative to rtl/)
JSON       := ../simulator/inputs/vgg16_tuned.json
WORKLOAD   := $(INPUTS_DIR)/workload.txt

VERILATOR  := verilator

SV_SRCS := src/flexpipe_pkg.sv \
           src/config_manager.sv \
           src/prefetch_dma.sv \
           src/memory_arbiter.sv \
           src/dram_model.sv \
           src/dummy_core.sv \
           src/$(TOP).sv

HARNESS  := harness/verilator_harness.cpp
SIM_EXE  := $(OBJ_DIR)/V$(TOP)

.PHONY: all run verilate prepare_inputs clean distclean

all: run

# ---------------------------------------------------------------------
# Generate inputs/workload.txt from the JSON description
# ---------------------------------------------------------------------
prepare_inputs: $(WORKLOAD)

$(WORKLOAD): scripts/prepare_vgg_inputs.py $(JSON)
	@echo "Preparing workload from JSON ($(JSON))..."
	mkdir -p $(INPUTS_DIR)
	python3 scripts/prepare_vgg_inputs.py --json $(JSON) --out $(WORKLOAD)

# ---------------------------------------------------------------------
# Build Verilator C++ model + simulator
# ---------------------------------------------------------------------
verilate: $(SIM_EXE)

$(SIM_EXE): $(SV_SRCS) $(HARNESS)
	@echo "Building Verilator model in $(OBJ_DIR)..."
	mkdir -p $(OBJ_DIR)
	$(VERILATOR) -Wall -Wno-IMPORTSTAR -Wno-UNUSED \
	  --cc --trace \
	  --top-module $(TOP) \
	  $(SV_SRCS) \
	  --exe $(HARNESS) \
	  -Mdir $(OBJ_DIR) \
	  -CFLAGS "-std=c++17 -O2"
	# IMPORTANT: use $(MAKE) (expanded by make), not $$(MAKE)
	$(MAKE) -C $(OBJ_DIR) -f V$(TOP).mk V$(TOP)

# ---------------------------------------------------------------------
# Run simulation and collect logs / metrics in outputs/
# ---------------------------------------------------------------------
run: prepare_inputs verilate
	@echo "Running simulation..."
	mkdir -p $(OUT_DIR)
	./$(SIM_EXE) $(WORKLOAD) $(JSON) > $(OUT_DIR)/verilator_stdout.log
	@echo "Simulation complete. Key outputs in $(OUT_DIR):"
	@echo "  - verilator_run_log.txt   (per-request cycles, totals)"
	@echo "  - per_cycle_log.csv       (cycle-by-cycle trace)"
	@echo "  - write_debug.log         (mem req/resp contents)"
	@echo "  - flexpipe.vcd            (waveform for GTKWave)"
	@echo "  - verilator_stdout.log    (this run's console output)"

# ---------------------------------------------------------------------
# Cleanup
# ---------------------------------------------------------------------
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(OUT_DIR)

distclean: clean
	rm -f $(WORKLOAD)
	rm -f $(OUT_DIR)/verilator_run_log.txt \
	      $(OUT_DIR)/verilator_stdout.log \
	      $(OUT_DIR)/per_cycle_log.csv \
	      $(OUT_DIR)/write_debug.log \
	      $(OUT_DIR)/flexpipe.vcd
