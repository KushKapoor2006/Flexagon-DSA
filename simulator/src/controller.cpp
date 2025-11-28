#include "controller.h"
#include <iostream>

AcceleratorController::AcceleratorController(MemorySystem* mem, std::string mode)
    : state(S_IDLE), sim_mode(mode), mem_system(mem), 
      active_filler(mem, MemoryRequest::HIGH),
      reconfig_timer(0), drain_timer(0), compute_timer(0), data_stall_timer(0) {}

void AcceleratorController::reset() {
    state = S_IDLE;
    // DO NOT reset active_filler here; it will be set on load_layer as needed
    stats = LayerStats();
    reconfig_timer = 0;
    drain_timer = 0;
    compute_timer = 0;
    data_stall_timer = 0;
}

void AcceleratorController::load_layer(const LayerConfig& layer, uint64_t cfg_cycles_modeled,
                                       uint64_t cfg_cycles_overlapped, uint64_t residual_cfg_cycles,
                                       TileFiller* prefetcher) {
    reset();
    current_layer = layer;
    stats.layer_name = layer.name;

    stats.config_cycles_modeled = cfg_cycles_modeled;
    stats.config_cycles_overlapped = cfg_cycles_overlapped;
    stats.residual_config_cycles = residual_cfg_cycles;

    // If there's a running prefetcher, copy its state to active_filler (to simulate preloaded data)
    if (sim_mode == "pipelined_prefetch" && prefetcher != nullptr && prefetcher->is_running()) {
        active_filler = *prefetcher;         // copy state
        active_filler.set_priority(MemoryRequest::HIGH); // upgrade priority
    } else {
        active_filler.reset();
    }

    if (sim_mode == "baseline") {
        reconfig_timer = cfg_cycles_modeled;
        state = S_CONFIG;
    } else {
        if (residual_cfg_cycles > 0) {
            reconfig_timer = residual_cfg_cycles;
            state = S_CONFIG;
        } else {
            state = S_DRAIN;
            drain_timer = MRN_DRAIN_CYCLES;
        }
    }
}

bool AcceleratorController::is_done() {
    return state == S_DONE;
}

LayerStats AcceleratorController::get_stats() {
    stats.total_cycles = stats.config_stall_cycles + stats.drain_stall_cycles + 
                         stats.compute_cycles + stats.data_stall_cycles;
    return stats;
}

void AcceleratorController::tick() {
    data_stall_timer = 0;

    switch (state) {
        case S_IDLE:
            break;
        case S_CONFIG:
            if (reconfig_timer > 0) {
                reconfig_timer--;
                stats.config_stall_cycles++;
            } else {
                state = S_DRAIN;
                drain_timer = MRN_DRAIN_CYCLES;
            }
            break;
        case S_DRAIN:
            if (drain_timer > 0) {
                drain_timer--;
                stats.drain_stall_cycles++;
            } else {
                state = S_RUN_WAIT_DATA;
                if (!active_filler.is_running()) {
                    active_filler.start(current_layer);
                }
            }
            break;
        case S_RUN_WAIT_DATA:
            if (active_filler.is_done() || active_filler.has_startup_ready()) {
                state = S_RUN_COMPUTE;
                compute_timer = current_layer.compute_cycles;
            } else {
                active_filler.tick();
                stats.data_stall_cycles++;
            }
            break;
        case S_RUN_COMPUTE:
            if (compute_timer > 0) {
                compute_timer--;
                stats.compute_cycles++;
            } else {
                state = S_DONE;
            }
            if (!active_filler.is_done()) {
                active_filler.tick();
            }
            break;
        case S_DONE:
            break;
    }
}
