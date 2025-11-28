#include "tile_filler.h"
#include <iostream>
#include <algorithm>

TileFiller::TileFiller(MemorySystem* mem, MemoryRequest::Priority prio)
    : state(IDLE), mem_system(mem), priority(prio), walks_remaining(0), data_fetched(0), next_addr(1000) {}

void TileFiller::reset() {
    state = IDLE;
    walks_remaining = 0;
    data_fetched = 0;
    current_layer = {};
}

void TileFiller::start(const LayerConfig& layer) {
    if (state != IDLE) {
        return;
    }
    current_layer = layer;
    walks_remaining = layer.pointer_walks;
    data_fetched = 0;
    state = POINTER_WALK;
}

bool TileFiller::is_done() {
    return state == DONE;
}

bool TileFiller::has_startup_ready() const {
    return data_fetched >= std::min(STARTUP_WINDOW_BYTES, current_layer.data_size);
}

void TileFiller::tick() {
    if (state == STALLED || state == IDLE || state == DONE) {
        return;
    }

    if (state == POINTER_WALK) {
        if (walks_remaining > 0) {
            state = STALLED; // Wait for memory
            mem_system->push_request(MemoryRequest(next_addr, 64, priority, this)); // 64B for a pointer
            next_addr += 64;
        } else {
            state = FETCH_DATA; // Done with pointers
        }
    } 
    
    if (state == FETCH_DATA) {
        if (data_fetched < current_layer.data_size) {
            state = STALLED; // Wait for memory
            uint64_t remaining_data = current_layer.data_size - data_fetched;
            uint32_t size_to_fetch = static_cast<uint32_t>(std::min<uint64_t>(256, remaining_data)); // Fetch 256B chunks
            mem_system->push_request(MemoryRequest(next_addr, size_to_fetch, priority, this));
            next_addr += size_to_fetch;
        } else {
            state = DONE; // Done fetching data
        }
    }
}

void TileFiller::memory_callback(MemoryRequest req) {
    if (state != STALLED) {
        return;
    }
    
    if (walks_remaining > 0) {
        walks_remaining--;
        state = POINTER_WALK;
    } else {
        data_fetched += req.size;
        state = FETCH_DATA;
    }
}
