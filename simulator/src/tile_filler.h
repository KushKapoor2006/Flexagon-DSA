#ifndef TILE_FILLER_H
#define TILE_FILLER_H

#include "config.h"
#include "memory.h"

class TileFiller {
public:
    enum State { IDLE, POINTER_WALK, FETCH_DATA, DONE, STALLED };

    TileFiller(MemorySystem* mem, MemoryRequest::Priority prio);
    
    void start(const LayerConfig& layer);
    void tick();
    bool is_done();
    void memory_callback(MemoryRequest req);
    void reset();

    // NEW: Getter functions
    uint64_t get_fetched_bytes() const { return data_fetched; }
    bool has_startup_ready() const;
    bool is_running() const { return state != IDLE && state != DONE; }
    void set_priority(MemoryRequest::Priority p) { priority = p; }

private:
    State state;
    MemorySystem* mem_system;
    MemoryRequest::Priority priority;
    
    LayerConfig current_layer;
    uint32_t walks_remaining;
    uint64_t data_fetched;
    uint64_t next_addr; // Mock address
};

#endif // TILE_FILLER_H
