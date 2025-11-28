#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "config.h"
#include "memory.h"
#include "tile_filler.h"
#include <string>

class AcceleratorController {
public:
    enum State { S_IDLE, S_CONFIG, S_DRAIN, S_RUN_WAIT_DATA, S_RUN_COMPUTE, S_DONE };

    AcceleratorController(MemorySystem* mem, std::string mode);
    
    // Modified signature: accept modeled config cycles and prefetcher pointer
    void load_layer(const LayerConfig& layer, uint64_t cfg_cycles_modeled,
                    uint64_t cfg_cycles_overlapped, uint64_t residual_cfg_cycles,
                    TileFiller* prefetcher);
    void tick();
    bool is_done();
    LayerStats get_stats();
    void reset();

private:
    State state;
    std::string sim_mode;
    MemorySystem* mem_system;
    
    TileFiller active_filler; // HIGH priority filler for current layer

    LayerConfig current_layer;
    LayerStats stats;

    uint64_t reconfig_timer;
    uint64_t drain_timer;
    uint64_t compute_timer;
    uint64_t data_stall_timer;
};

#endif // CONTROLLER_H
