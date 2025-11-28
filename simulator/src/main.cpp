#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>

#include "json.hpp"   // Place nlohmann/json single-header here (src/json.hpp)
#include "config.h"
#include "simulator.h"

using json = nlohmann::json;

// Local paper path (uploaded file) for your reference:
// /mnt/data/Flexagon_SparseMatrix.pdf

static void ensure_outputs_dir() {
    struct stat st{};
    if (stat("outputs", &st) != 0) {
#ifdef _WIN32
        _mkdir("outputs");
#else
        mkdir("outputs", 0755);
#endif
    }
}

std::string make_output_name(const std::string& input, const std::string& mode) {
    auto pos = input.find_last_of("/\\");
    std::string base = (pos == std::string::npos) ? input : input.substr(pos + 1);
    auto dot = base.find_last_of('.');
    std::string stem = (dot == std::string::npos) ? base : base.substr(0, dot);
    return "outputs/" + stem + "_" + mode + "_results.csv";
}

std::vector<LayerConfig> parse_config(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cerr << "Error: Could not open config file " << filename << std::endl;
        exit(1);
    }
    json data = json::parse(f);
    
    std::vector<LayerConfig> configs;
    for (const auto& item : data["layers"]) {
        configs.push_back({
            item["layer_name"],
            item["compute_cycles"],
            item["config_bits"],
            item["pointer_walks"],
            item["data_size"]
        });
    }
    return configs;
}

void write_results_csv(const std::string& filename, const std::vector<LayerStats>& all_stats, const LayerStats& total_stats) {
    std::ofstream f(filename);
    if (!f.is_open()) {
        std::cerr << "Error: Could not open output file " << filename << std::endl;
        return;
    }

    f << "Layer,Compute,ConfigModeled,ConfigOverlapped,ResidualConfig,ConfigStall,DrainStall,DataStall,PrefetchFetchedBytes,DramBusy,WallClock,Total,DRAMBusyPct\n";

    for (const auto& stats : all_stats) {
        double busy_pct = (stats.wall_clock_cycles == 0) ? 0 : 
            100.0 * static_cast<double>(stats.dram_busy_cycles) / stats.wall_clock_cycles;
        f << stats.layer_name << ","
          << stats.compute_cycles << ","
          << stats.config_cycles_modeled << ","
          << stats.config_cycles_overlapped << ","
          << stats.residual_config_cycles << ","
          << stats.config_stall_cycles << ","
          << stats.drain_stall_cycles << ","
          << stats.data_stall_cycles << ","
          << stats.prefetch_bytes_fetched << ","
          << stats.dram_busy_cycles << ","
          << stats.wall_clock_cycles << ","
          << stats.total_cycles << ","
          << std::fixed << std::setprecision(2) << busy_pct << "\n";
    }

    double total_busy_pct = (total_stats.wall_clock_cycles == 0) ? 0 :
        100.0 * static_cast<double>(total_stats.dram_busy_cycles) / total_stats.wall_clock_cycles;
    f << "TOTAL,"
      << total_stats.compute_cycles << ","
      << total_stats.config_cycles_modeled << ","
      << total_stats.config_cycles_overlapped << ","
      << total_stats.residual_config_cycles << ","
      << total_stats.config_stall_cycles << ","
      << total_stats.drain_stall_cycles << ","
      << total_stats.data_stall_cycles << ","
      << total_stats.prefetch_bytes_fetched << ","
      << total_stats.dram_busy_cycles << ","
      << total_stats.wall_clock_cycles << ","
      << total_stats.total_cycles << ","
      << std::fixed << std::setprecision(2) << total_busy_pct << "\n";

    f.close();
    std::cout << "Successfully wrote results to " << filename << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./flexpipe_sim <input_config_json> <mode>" << std::endl;
        std::cerr << "Modes: baseline, pipelined_control, pipelined_prefetch" << std::endl;
        return 1;
    }

    std::string config_file = argv[1];
    std::string sim_mode = argv[2];

    if (sim_mode != "baseline" && sim_mode != "pipelined_control" && sim_mode != "pipelined_prefetch") {
        std::cerr << "Error: Invalid mode '" << sim_mode << "'" << std::endl;
        return 1;
    }

    std::vector<LayerConfig> layers = parse_config(config_file);
    std::cout << "Loaded " << layers.size() << " layers from " << config_file << std::endl;
    std::cout << "Running simulation in '" << sim_mode << "' mode..." << std::endl;

    MemorySystem mem_system;
    AcceleratorController controller(&mem_system, sim_mode);
    
    TileFiller* background_prefetch = nullptr;
    if (sim_mode == "pipelined_prefetch") {
        background_prefetch = new TileFiller(&mem_system, MemoryRequest::LOW);
    }

    std::vector<LayerStats> all_stats;
    LayerStats total_stats;
    total_stats.layer_name = "TOTAL";
    
    uint64_t global_cycle_count = 0;
    uint64_t last_dram_busy_cycles = 0;
    uint64_t prev_layer_compute_cycles = 0;

    for (size_t i = 0; i < layers.size(); ++i) {
        const auto& layer_config = layers[i];

        const uint64_t cfg_bytes = ceil_div<uint64_t>(layer_config.config_bits, 8ULL);
        const uint64_t cfg_cycles_modeled = ceil_div<uint64_t>(cfg_bytes, CONFIG_BUS_WIDTH_BYTES);

        uint64_t cfg_cycles_overlapped = 0;
        uint64_t residual_cfg_cycles = cfg_cycles_modeled;
        if (sim_mode == "pipelined_control" || sim_mode == "pipelined_prefetch") {
            cfg_cycles_overlapped = (prev_layer_compute_cycles > 0) ? std::min<uint64_t>(cfg_cycles_modeled, prev_layer_compute_cycles) : 0;
            residual_cfg_cycles = (cfg_cycles_modeled > cfg_cycles_overlapped) ? (cfg_cycles_modeled - cfg_cycles_overlapped) : 0;
        }

        if (background_prefetch && (i + 1) < layers.size()) {
            background_prefetch->reset();
            background_prefetch->start(layers[i + 1]);
        }

        controller.load_layer(layer_config, cfg_cycles_modeled, cfg_cycles_overlapped, residual_cfg_cycles, background_prefetch);

        uint64_t layer_start_cycle = global_cycle_count;

        while (!controller.is_done()) {
            controller.tick();

            if (background_prefetch) {
                background_prefetch->tick();
            }

            mem_system.tick();
            global_cycle_count++;
        }

        uint64_t layer_end_cycle = global_cycle_count;
        LayerStats stats = controller.get_stats();
        stats.wall_clock_cycles = layer_end_cycle - layer_start_cycle;

        uint64_t current_dram_busy = mem_system.get_dram_busy_cycles();
        stats.dram_busy_cycles = current_dram_busy - last_dram_busy_cycles;
        last_dram_busy_cycles = current_dram_busy;

        if (background_prefetch && (i + 1) < layers.size()) {
            stats.prefetch_bytes_fetched = background_prefetch->get_fetched_bytes();
        } else {
            stats.prefetch_bytes_fetched = 0;
        }

        stats.config_cycles_modeled = cfg_cycles_modeled;
        stats.config_cycles_overlapped = cfg_cycles_overlapped;
        stats.residual_config_cycles = residual_cfg_cycles;
        stats.total_cycles = stats.config_stall_cycles + stats.drain_stall_cycles + stats.compute_cycles + stats.data_stall_cycles;

        all_stats.push_back(stats);

        total_stats.compute_cycles += stats.compute_cycles;
        total_stats.config_stall_cycles += stats.config_stall_cycles;
        total_stats.drain_stall_cycles += stats.drain_stall_cycles;
        total_stats.data_stall_cycles += stats.data_stall_cycles;
        total_stats.total_cycles += stats.total_cycles;
        total_stats.dram_busy_cycles += stats.dram_busy_cycles;
        total_stats.prefetch_bytes_fetched += stats.prefetch_bytes_fetched;
        total_stats.config_cycles_modeled += stats.config_cycles_modeled;
        total_stats.config_cycles_overlapped += stats.config_cycles_overlapped;
        total_stats.residual_config_cycles += stats.residual_config_cycles;

        prev_layer_compute_cycles = layer_config.compute_cycles;
    }
    
    total_stats.wall_clock_cycles = global_cycle_count;
    total_stats.layer_name = "TOTAL";

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Simulation Finished" << std::endl;
    std::cout << "Mode: " << sim_mode << std::endl;
    std::cout << "Total Cycles (wall-clock): " << total_stats.wall_clock_cycles << std::endl;
    std::cout << "  Compute:        " << total_stats.compute_cycles << std::endl;
    std::cout << "  Config Stall:   " << total_stats.config_stall_cycles << std::endl;
    std::cout << "  Drain Stall:    " << total_stats.drain_stall_cycles << std::endl;
    std::cout << "  Data Stall:     " << total_stats.data_stall_cycles << std::endl;
    std::cout << "  DRAM Busy:      " << total_stats.dram_busy_cycles << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    ensure_outputs_dir();
    std::string output_filename = make_output_name(config_file, sim_mode);
    write_results_csv(output_filename, all_stats, total_stats);

    if (background_prefetch) delete background_prefetch;

    return 0;
}
