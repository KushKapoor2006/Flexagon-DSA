#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <cstdint>
#include <vector>

// ### Core Architectural Parameters ###

// Bytes-per-cycle for config bus (explicit units)
const uint32_t CONFIG_BUS_WIDTH_BYTES = 4; 

// Pipeline drain cost (cycles)
const uint32_t MRN_DRAIN_CYCLES = 50;       

// Latencies
const uint32_t L2_LATENCY_CYCLES = 20;
const uint32_t DRAM_LATENCY_CYCLES = 100;

// DRAM bandwidth in bytes per cycle (example: 32 B/cycle)
const uint32_t DRAM_BANDWIDTH_B_PER_C = 32; 

// Startup window (how many bytes must be prefetched before compute can start)
const uint64_t STARTUP_WINDOW_BYTES = 8 * 1024; // 8KB

// Safe ceil-div helper
template <typename T>
static inline T ceil_div(T num, T den) {
    if (den == 0) return 0;
    return (num + den - 1) / den;
}

// Forward declarations
class TileFiller;
class AcceleratorController;

// Represents a single layer's properties
struct LayerConfig {
    std::string name;
    uint64_t compute_cycles;  // Cycles for PEs to run
    uint32_t config_bits;     // Total bits for MRN/PE/Mem config
    uint32_t pointer_walks;   // Number of pointer dereferences
    uint64_t data_size;       // Total data bytes for this layer
};

// Represents a memory request
struct MemoryRequest {
    enum Priority { LOW, HIGH };
    
    uint64_t address;
    uint32_t size;
    Priority priority;
    bool is_write;
    TileFiller* callback_owner; // Who to notify when done
    
    MemoryRequest(uint64_t a, uint32_t s, Priority p, TileFiller* o)
        : address(a), size(s), priority(p), is_write(false), callback_owner(o) {}
};

// Statistics collected per layer
struct LayerStats {
    std::string layer_name;

    // Input/derived config cycle numbers
    uint64_t config_cycles_modeled = 0;
    uint64_t config_cycles_overlapped = 0;
    uint64_t residual_config_cycles = 0;

    // Measured runtime breakdown (controller-view)
    uint64_t compute_cycles = 0;
    uint64_t config_stall_cycles = 0;
    uint64_t drain_stall_cycles = 0;
    uint64_t data_stall_cycles = 0;

    // Memory/Prefetch metrics
    uint64_t prefetch_bytes_fetched = 0;
    uint64_t dram_busy_cycles = 0;

    // Wall-clock and totals
    uint64_t wall_clock_cycles = 0;
    uint64_t total_cycles = 0;
};

#endif // CONFIG_H
