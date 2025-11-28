#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <queue>
#include <map>
#include "config.h"

class DramController {
public:
    DramController();
    void push_request(MemoryRequest req);
    void tick();
    bool is_busy();
    uint64_t get_busy_cycles() const { return busy_cycles; } // Stats getter

private:
    std::queue<MemoryRequest> high_prio_queue;
    std::queue<MemoryRequest> low_prio_queue;
    
    bool busy;
    uint64_t cycles_until_free;
    MemoryRequest current_req;
    uint64_t busy_cycles; // Stats counter
};

class MemorySystem {
public:
    MemorySystem();
    void push_request(MemoryRequest req);
    void tick();
    uint64_t get_dram_busy_cycles() const; // Stats passthrough

private:
    DramController dram;
    // L2 cache logic would go here
};

#endif // MEMORY_H
