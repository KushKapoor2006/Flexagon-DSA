// rtl/harness/verilator_harness.cpp
// Harness: drives AXI-lite config writes, flip handshake, and DRAM req/resp
// Designed to work with the provided RTL (flexpipe_top + config_manager + prefetch_dma)
// Only the harness changed; all RTL and scripts remain unchanged.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <algorithm>
#include <functional>

#include "Vflexpipe_top.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

using namespace std;

// ---------------------------
// Config (adjust if typedefs differ)
static const int MEM_RESP_DATA_BITS = 256;
static const int MEM_RESP_ID_BITS   = 16;
static const int MEM_RESP_EPOCH_BITS= 8;
static const int MEM_RESP_LAST_BITS = 1;
static const int MEM_RESP_ERR_BITS  = 1;

static const int REQ_LEN_BITS = 32;
static const int REQ_ADDR_BITS = 32;
static const int REQ_ID_BITS = 16;
static const int REQ_EPOCH_BITS = 8;

static const int REQ_STABLE_CYCLES = 4;
static const int BETWEEN = 8;
static const size_t DEFAULT_MEM_RESP_WORDS = 9;
static const size_t DEFAULT_REQ_WORDS = 4;
static const size_t DEFAULT_ACTIVE_CFG_WORDS = 7;

// ---------------------------
// helpers for WData arrays
static inline uint32_t get_word(unsigned int *arr, size_t words, size_t idx) {
    if (!arr || idx >= words) return 0u;
    return arr[idx];
}
static inline void set_word(unsigned int *arr, size_t words, size_t idx, uint32_t val) {
    if (!arr || idx >= words) return;
    arr[idx] = val;
}

void write_bits_u64(unsigned int *arr, size_t words, uint64_t value, size_t bit_offset, size_t width) {
    if (!arr || width == 0) return;
    if (width > 64) width = 64;
    size_t bit = bit_offset;
    size_t remaining = width;
    size_t word_idx = bit / 32;
    size_t word_bit = bit % 32;
    uint64_t val = value;
    while (remaining > 0 && word_idx < words) {
        size_t can_take = std::min<size_t>(remaining, 32 - word_bit);
        uint32_t mask = (can_take == 32) ? 0xFFFFFFFFu : ((1u << can_take) - 1u);
        uint32_t chunk = (uint32_t)(val & mask);
        uint32_t base = get_word(arr, words, word_idx);
        base &= ~(mask << word_bit);
        base |= (chunk << word_bit);
        set_word(arr, words, word_idx, base);
        remaining -= can_take;
        val >>= can_take;
        word_idx++;
        word_bit = 0;
    }
}

void write_bits_hex(unsigned int *arr, size_t words, const std::string &hexstr, size_t bit_offset, size_t width) {
    if (!arr || width == 0) return;
    string h = hexstr;
    if (h.rfind("0x", 0) == 0 || h.rfind("0X", 0) == 0) h = h.substr(2);
    if (h.size() % 2) h = "0" + h;
    vector<uint8_t> bytes;
    for (size_t i = 0; i < h.size(); i += 2) {
        string bhex = h.substr(i, 2);
        uint8_t b = (uint8_t) strtoul(bhex.c_str(), nullptr, 16);
        bytes.push_back(b);
    }
    size_t total_bytes_needed = (width + 7) / 8;
    vector<uint8_t> le(total_bytes_needed, 0);
    for (size_t i = 0; i < bytes.size() && i < total_bytes_needed; ++i) {
        size_t src_idx = (bytes.size() > i) ? bytes.size() - 1 - i : 0;
        le[i] = bytes[src_idx];
    }
    size_t bit = bit_offset;
    size_t remaining_bits = width;
    size_t byte_idx = 0;
    while (remaining_bits > 0) {
        size_t word_idx = bit / 32;
        size_t word_bit = bit % 32;
        if (word_idx >= words) break;
        size_t can_take = std::min<size_t>(remaining_bits, 32 - word_bit);
        uint64_t chunk_val = 0;
        for (size_t b = 0; b < 8 && (byte_idx + b) < le.size(); ++b) {
            chunk_val |= ((uint64_t)le[byte_idx + b]) << (8*b);
        }
        uint32_t mask = (can_take == 32) ? 0xFFFFFFFFu : ((1u << can_take) - 1u);
        uint32_t base = get_word(arr, words, word_idx);
        base &= ~(mask << word_bit);
        base |= ((uint32_t)(chunk_val & mask) << word_bit);
        set_word(arr, words, word_idx, base);
        size_t consumed_bytes = (can_take + 7) / 8;
        byte_idx += consumed_bytes;
        remaining_bits -= can_take;
        bit += can_take;
    }
}

uint64_t read_bits_u64(unsigned int *arr, size_t words, size_t bit_offset, size_t width) {
    if (!arr || width == 0) return 0;
    if (width > 64) width = 64;
    uint64_t result = 0;
    size_t bit = bit_offset;
    size_t remaining = width;
    size_t out_shift = 0;
    while (remaining > 0) {
        size_t word_idx = bit / 32;
        size_t word_bit = bit % 32;
        if (word_idx >= words) break;
        size_t can_take = std::min<size_t>(remaining, 32 - word_bit);
        uint32_t w = get_word(arr, words, word_idx);
        uint32_t masked = (w >> word_bit) & ((can_take == 32) ? 0xFFFFFFFFu : ((1u << can_take) - 1u));
        result |= (uint64_t)masked << out_shift;
        remaining -= can_take;
        out_shift += can_take;
        bit += can_take;
    }
    return result;
}

string words_to_hex(unsigned int *arr, size_t words) {
    if (!arr || words == 0) return string("0");
    ostringstream oss;
    for (size_t i = 0; i < words; ++i) {
        size_t idx = words - 1 - i;
        oss << hex << setw(8) << setfill('0') << ((uint64_t)arr[idx]);
    }
    oss << dec;
    return oss.str();
}

void dbg_print_words(const char *name, unsigned int *arr, size_t words) {
    if (!arr) {
        cerr << "dbg: " << name << " arr==nullptr\n";
        return;
    }
    cerr << "dbg: " << name << " words=" << words << " :";
    for (size_t w = 0; w < words; ++w) {
        cerr << " " << hex << setw(8) << setfill('0') << (uint64_t)arr[w];
    }
    cerr << dec << "\n";
}

// ---------------------------
// workload parsing
struct WorkReq { uint32_t id; uint32_t epoch; uint64_t addr; uint32_t len; string data_hex; };
vector<WorkReq> load_workload(const string &path) {
    vector<WorkReq> out;
    ifstream ifs(path);
    if (!ifs.good()) return out;
    string line;
    while (getline(ifs, line)) {
        if (line.empty() || line[0] == '#') continue;
        stringstream ss(line);
        WorkReq r; string addr_str;
        if (!(ss >> r.id >> r.epoch >> addr_str >> r.len)) continue;
        r.addr = strtoull(addr_str.c_str(), nullptr, 0);
        if (ss >> r.data_hex) {} else r.data_hex = "";
        out.push_back(r);
    }
    return out;
}

// ---------------------------
// main
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    string outdir = "outputs";
    system(("mkdir -p " + outdir).c_str());
    string write_debug = outdir + "/write_debug.log";

    string workload = "inputs/workload.txt";
    string json_input = "../simulator/inputs/vgg16_tuned.json";
    if (argc >= 2) workload = argv[1];
    if (argc >= 3) json_input = argv[2];
    auto requests = load_workload(workload);

    Vflexpipe_top *top = new Vflexpipe_top;
    VerilatedVcdC* tfp = nullptr;
    Verilated::traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open((outdir + "/flexpipe.vcd").c_str());

    // detect word sizes (if Verilator exposes arrays)
    size_t mem_resp_words = 0, req_words = 0, active_cfg_words = 0;
    // try sizeof-based detection (guarded if arrays not visible)
    #ifdef HAS_MEM_MASTER_RESP
    mem_resp_words = sizeof(top->mem_master_resp) / sizeof(top->mem_master_resp[0]);
    #endif
    #ifdef HAS_MEM_MASTER_REQ
    req_words = sizeof(top->mem_master_req) / sizeof(top->mem_master_req[0]);
    #endif
    #ifdef HAS_ACTIVE_CONFIG
    active_cfg_words = sizeof(top->active_config) / sizeof(top->active_config[0]);
    #endif
    if (mem_resp_words == 0) mem_resp_words = DEFAULT_MEM_RESP_WORDS;
    if (req_words == 0) req_words = DEFAULT_REQ_WORDS;
    if (active_cfg_words == 0) active_cfg_words = DEFAULT_ACTIVE_CFG_WORDS;

    cerr << "Detected words: mem_resp_words=" << mem_resp_words << " req_words=" << req_words << " active_cfg_words=" << active_cfg_words << endl;

    // clear model arrays
    memset(&top->mem_master_resp[0], 0, mem_resp_words * sizeof(unsigned int));
    memset(&top->mem_master_req[0], 0, req_words * sizeof(unsigned int));
    memset(&top->active_config[0], 0, active_cfg_words * sizeof(unsigned int));

    // debug port initial state
    top->dbg_mode = 0;

    // initialize AXI-lite & control signals from harness
    top->s_axi_awaddr = 0;
    top->s_axi_awvalid = 0;
    top->s_axi_wdata = 0;
    top->s_axi_wstrb = 0;
    top->s_axi_wvalid = 0;
    top->s_axi_bready = 0;

    top->core_idle = 1;
    top->core_pipeline_drained = 1;
    top->flip_ack = 0;
    top->core_mem_req_valid = 0;

    ofstream runlog_fs(outdir + "/verilator_run_log.txt");
    ofstream write_dbg_fs(write_debug);
    ofstream percycle_fs(outdir + "/per_cycle_log.csv");
    ofstream dbgout(write_debug + ".debug");

    runlog_fs << "JSON: " << json_input << "\n";
    runlog_fs << "workload: " << workload << " requests=" << requests.size() << "\n";

    // CSV header
    percycle_fs << "cycle,clk,rst_n";
    for (size_t i=0;i<req_words;++i) percycle_fs << ",pre_req_w" << i;
    for (size_t i=0;i<req_words;++i) percycle_fs << ",post_req_w" << i;
    for (size_t i=0;i<mem_resp_words;++i) percycle_fs << ",pre_resp_w" << i;
    for (size_t i=0;i<mem_resp_words;++i) percycle_fs << ",post_resp_w" << i;
    for (size_t i=0;i<active_cfg_words;++i) percycle_fs << ",pre_act_w" << i;
    for (size_t i=0;i<active_cfg_words;++i) percycle_fs << ",post_act_w" << i;
    percycle_fs << ",active_compute_cycles\n";

    vluint64_t main_time = 0;
    auto do_dump_post = [&](vluint64_t t, VerilatedVcdC* tfp_ptr) {
        static vluint64_t last = (vluint64_t)-1;
        if (!tfp_ptr) return;
        if (last == (vluint64_t)-1 || t > last) { tfp_ptr->dump(t); last = t; }
    };

    // log snapshots (pre -> eval -> post)
    auto log_cycle_with_snapshots = [&](int cycles=1) {
        ios oldstate(nullptr);
        for (int c=0;c<cycles;++c) {
            main_time++;
            top->clk = !top->clk;

            // PRE
            percycle_fs << main_time << "," << (top->clk ? 1 : 0) << "," << (top->rst_n ? 1 : 0);
            for (size_t w=0; w<req_words; ++w) percycle_fs << "," << hex << setw(8) << setfill('0') << (uint64_t)top->mem_master_req[w] << dec;
            for (size_t w=0; w<req_words; ++w) percycle_fs << "," << hex << setw(8) << setfill('0') << (uint64_t)top->mem_master_req[w] << dec;
            for (size_t w=0; w<mem_resp_words; ++w) percycle_fs << "," << hex << setw(8) << setfill('0') << (uint64_t)top->mem_master_resp[w] << dec;
            for (size_t w=0; w<mem_resp_words; ++w) percycle_fs << "," << hex << setw(8) << setfill('0') << (uint64_t)top->mem_master_resp[w] << dec;
            for (size_t w=0; w<active_cfg_words; ++w) percycle_fs << "," << hex << setw(8) << setfill('0') << (uint64_t)top->active_config[w] << dec;
            for (size_t w=0; w<active_cfg_words; ++w) percycle_fs << "," << hex << setw(8) << setfill('0') << (uint64_t)top->active_config[w] << dec;
            uint64_t act_pre = read_bits_u64(&top->active_config[0], active_cfg_words, 0, 32);
            percycle_fs << "," << act_pre << "\n";
            percycle_fs.flush();

            // eval + VCD
            top->eval();
            do_dump_post(main_time, tfp);

            // POST
            percycle_fs << "POST," << main_time << "," << (top->clk ? 1 : 0) << "," << (top->rst_n ? 1 : 0);
            for (size_t w=0; w<req_words; ++w) percycle_fs << "," << hex << setw(8) << setfill('0') << (uint64_t)top->mem_master_req[w] << dec;
            for (size_t w=0; w<mem_resp_words; ++w) percycle_fs << "," << hex << setw(8) << setfill('0') << (uint64_t)top->mem_master_resp[w] << dec;
            for (size_t w=0; w<active_cfg_words; ++w) percycle_fs << "," << hex << setw(8) << setfill('0') << (uint64_t)top->active_config[w] << dec;
            uint64_t act_post = read_bits_u64(&top->active_config[0], active_cfg_words, 0, 32);
            percycle_fs << "," << act_post << "\n";
            percycle_fs.flush();

            std::this_thread::sleep_for(std::chrono::microseconds(0));
        }
    };

    // Reset sequence
    top->clk = 0; top->rst_n = 0; top->eval(); do_dump_post(main_time, tfp);
    log_cycle_with_snapshots(1);
    for (int i=0;i<8;++i) log_cycle_with_snapshots(1);
    top->rst_n = 1; top->eval(); log_cycle_with_snapshots(1);

    // Main run: for each workload entry, perform AXI writes to load shadow_config, flip,
    // then issue mem request + response waveforms so RTL paths see activity.
    vector<uint64_t> issue_time(requests.size(),0), resp_time(requests.size(),0);
    uint64_t start_cycle = (uint64_t)-1, end_cycle = 0;

    size_t issued = 0, responses = 0;

    // Iterate workload (treat each workload line as a layer-config + subsequent DRAM request)
    for (size_t idx=0; idx<requests.size(); ++idx) {
        const auto &r = requests[idx];

        // prepare config values (use JSON path or heuristics; here basic mapping using length)
        uint32_t compute_cycles_v = (uint32_t)(r.len * 2);
        uint32_t config_bits_v   = (uint32_t)(r.len * 200);
        uint32_t pointer_walks_v = (uint32_t)64;
        uint32_t data_size_v     = (uint32_t) r.len;

        // Ensure AXI signals default
        top->s_axi_awvalid = 0;
        top->s_axi_wvalid  = 0;
        top->s_axi_bready  = 0;
        top->s_axi_wstrb   = 0xF;

        // Helper lambda: perform a single AXI write word (wdata) and wait for bvalid
        auto perform_axi_write = [&](uint32_t wdata) {
            top->s_axi_awaddr = 0ULL;
            top->s_axi_awvalid = 1;
            top->s_axi_wdata = wdata;
            top->s_axi_wstrb = 0xF;
            top->s_axi_wvalid = 1;

            // One cycle with aw/w valid
            log_cycle_with_snapshots(1);

            // Deassert aw/w
            top->s_axi_awvalid = 0;
            top->s_axi_wvalid = 0;

            // Wait until bvalid; bounded timeout avoids infinite hangs
            int timeout = 500;
            while (timeout-- > 0) {
                log_cycle_with_snapshots(1);
                // If config_manager asserts bvalid, respond
                if (top->s_axi_bvalid) {
                    top->s_axi_bready = 1;
                    log_cycle_with_snapshots(1);
                    top->s_axi_bready = 0;
                    break;
                }
            }
            // Clear bready
            top->s_axi_bready = 0;
        };

        // Do 4 writes corresponding to mapping in config_manager
        perform_axi_write(compute_cycles_v);
        perform_axi_write(config_bits_v);
        perform_axi_write(pointer_walks_v);
        perform_axi_write(data_size_v);

        // After writing, wait for request_flip and then ack it (flip_ack)
        int flip_timeout = 500;
        while (flip_timeout-- > 0) {
            log_cycle_with_snapshots(1);
            if (top->request_flip) {
                top->flip_ack = 1;
                log_cycle_with_snapshots(1);
                top->flip_ack = 0;
                break;
            }
        }

        // Now issue DRAM request via top->mem_master_req so arbiter/dram see it
        if (start_cycle == (uint64_t)-1) {
            start_cycle = main_time;
            top->dbg_mode = 1; // enable debug counter during measured interval
        }

        for (size_t w=0; w<req_words; ++w) top->mem_master_req[w] = 0;
        write_bits_u64(&top->mem_master_req[0], req_words, r.len, 0, REQ_LEN_BITS);
        write_bits_u64(&top->mem_master_req[0], req_words, r.addr, REQ_LEN_BITS, REQ_ADDR_BITS);
        write_bits_u64(&top->mem_master_req[0], req_words, r.id, REQ_LEN_BITS + REQ_ADDR_BITS, REQ_ID_BITS);
        write_bits_u64(&top->mem_master_req[0], req_words, r.epoch, REQ_LEN_BITS + REQ_ADDR_BITS + REQ_ID_BITS, REQ_EPOCH_BITS);

        dbgout << "WRITE_REQ["<<idx<<"] t="<<main_time<<" id="<<r.id<<" addr=0x"<<hex<<r.addr<<dec<<" words=";
        for (size_t w=0; w<req_words;++w) dbgout << " " << hex << setw(8) << setfill('0') << (uint64_t)top->mem_master_req[w];
        dbgout << "\n"; dbgout.flush();
        dbg_print_words("mem_master_req(after write)", &top->mem_master_req[0], req_words);

        issue_time[idx] = main_time;
        issued++;

        // Hold request stable so arbiter/dram sample it
        for (int s=0; s<REQ_STABLE_CYCLES; ++s) log_cycle_with_snapshots(1);
        log_cycle_with_snapshots(BETWEEN);

        // Provide response on mem_master_resp (simulate DRAM response)
        for (size_t w=0; w<mem_resp_words; ++w) top->mem_master_resp[w] = 0;
        if (!r.data_hex.empty()) write_bits_hex(&top->mem_master_resp[0], mem_resp_words, r.data_hex, 0, MEM_RESP_DATA_BITS);
        else write_bits_u64(&top->mem_master_resp[0], mem_resp_words, r.id, 0, 32);
        write_bits_u64(&top->mem_master_resp[0], mem_resp_words, r.id, MEM_RESP_DATA_BITS, MEM_RESP_ID_BITS);
        write_bits_u64(&top->mem_master_resp[0], mem_resp_words, r.epoch, MEM_RESP_DATA_BITS + MEM_RESP_ID_BITS, MEM_RESP_EPOCH_BITS);
        write_bits_u64(&top->mem_master_resp[0], mem_resp_words, 1, MEM_RESP_DATA_BITS + MEM_RESP_ID_BITS + MEM_RESP_EPOCH_BITS, MEM_RESP_LAST_BITS);
        write_bits_u64(&top->mem_master_resp[0], mem_resp_words, 0, MEM_RESP_DATA_BITS + MEM_RESP_ID_BITS + MEM_RESP_EPOCH_BITS + MEM_RESP_LAST_BITS, MEM_RESP_ERR_BITS);

        dbgout << "WRITE_RESP["<<idx<<"] t="<<main_time<<" words="<< words_to_hex(&top->mem_master_resp[0], mem_resp_words) << "\n"; dbgout.flush();
        dbg_print_words("mem_master_resp(after write)", &top->mem_master_resp[0], mem_resp_words);

        // Hold response a couple cycles for arbiter/dram
        log_cycle_with_snapshots(2);

        resp_time[idx] = main_time;
        responses++;

        runlog_fs << "REQ["<<idx<<"] id="<<r.id<<" issued_t="<<issue_time[idx]<<" resp_t="<<resp_time[idx]<<"\n";
    }

    // stop debug counter
    top->dbg_mode = 0;

    // drain a bit
    for (int i=0;i<100;++i) log_cycle_with_snapshots(1);

    // metrics
    if (start_cycle != (uint64_t)-1 && responses == requests.size()) {
        end_cycle = resp_time.back();
        uint64_t work_cycles = (end_cycle > start_cycle) ? (end_cycle - start_cycle) : 0;
        runlog_fs << "Measured work cycles (first issue -> last response): " << work_cycles << "\n";
        cout << "Measured work cycles: " << work_cycles << "\n";
    }

    uint32_t rtl_dbg_cycles = (uint32_t) top->dbg_cycle_count;
    runlog_fs << "RTL dbg_cycle_count = " << rtl_dbg_cycles << "\n";
    cout << "RTL dbg_cycle_count = " << rtl_dbg_cycles << "\n";

    uint64_t compute_cycles_val = read_bits_u64(&top->active_config[0], active_cfg_words, 0, 32);
    runlog_fs << "active_config.compute_cycles (raw) = " << compute_cycles_val << "\n";

    runlog_fs << "Issued " << issued << " Responses " << responses << " total_cycles " << main_time << "\n";
    cout << "Issued " << issued << " Responses " << responses << " total_cycles " << main_time << "\n";

    if (tfp) { tfp->close(); delete tfp; }
    top->final();
    delete top;

    runlog_fs.close();
    percycle_fs.close();
    write_dbg_fs.close();
    dbgout.close();

    cout << "Done. Logs: outputs/verilator_run_log.txt, outputs/per_cycle_log.csv, outputs/write_debug.log\n";
    return 0;
}
