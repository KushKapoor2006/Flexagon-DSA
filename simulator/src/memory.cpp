#include "memory.h"
#include "tile_filler.h"
#include <iostream>

// --- DramController ---

DramController::DramController() 
    : busy(false), cycles_until_free(0), current_req(0, 0, MemoryRequest::LOW, nullptr), busy_cycles(0) {}

bool DramController::is_busy() {
    return busy || !high_prio_queue.empty() || !low_prio_queue.empty();
}

void DramController::push_request(MemoryRequest req) {
    if (req.priority == MemoryRequest::HIGH) {
        high_prio_queue.push(req);
    } else {
        low_prio_queue.push(req);
    }
}

void DramController::tick() {
    static uint64_t cycle_count = 0; // For aging
    cycle_count++;

    if (busy) {
        // Count a busy cycle for each tick where DRAM was busy at start
        busy_cycles++;
        if (cycles_until_free > 0) cycles_until_free--;
        
        if (cycles_until_free == 0) {
            busy = false;
            // Notify owner
            if (current_req.callback_owner) {
                current_req.callback_owner->memory_callback(current_req);
            }
        }
        return; // DRAM is busy handling a request
    }

    // DRAM is free, check queues
    bool served = false;
    if (!high_prio_queue.empty()) {
        current_req = high_prio_queue.front();
        high_prio_queue.pop();
        served = true;
    } else if (!low_prio_queue.empty()) {
        // Add simple aging policy: let low prio in periodically
        if (high_prio_queue.empty() || (cycle_count % 64 == 0)) {
            current_req = low_prio_queue.front();
            low_prio_queue.pop();
            served = true;
        }
    }
    
    if (!served) {
        return; // No requests
    }

    // Start processing the new request
    busy = true;
    // Note: busy_cycles increment happens at the top of next tick to avoid double-counting

    // Use ceil_div and add L2 latency
    const uint64_t xfer = ceil_div<uint64_t>(current_req.size, DRAM_BANDWIDTH_B_PER_C);
    // Charge L2 + DRAM latency + transfer cycles (ensure at least 1 transfer cycle if non-zero size)
    cycles_until_free = L2_LATENCY_CYCLES + DRAM_LATENCY_CYCLES + (xfer ? xfer : 1);
}

// --- MemorySystem ---

MemorySystem::MemorySystem() {}

void MemorySystem::push_request(MemoryRequest req) {
    // Simplified: All L1 misses go to DRAM.
    dram.push_request(req);
}

void MemorySystem::tick() {
    dram.tick();
}

uint64_t MemorySystem::get_dram_busy_cycles() const {
    return dram.get_busy_cycles();
}
