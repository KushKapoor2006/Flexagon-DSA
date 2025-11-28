// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VFLEXPIPE_TOP_H_
#define _VFLEXPIPE_TOP_H_  // guard

#include "verilated.h"

//==========

class Vflexpipe_top__Syms;
class Vflexpipe_top_VerilatedVcd;


//----------

VL_MODULE(Vflexpipe_top) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(dbg_mode,0,0);
    VL_IN8(s_axi_awvalid,0,0);
    VL_OUT8(s_axi_awready,0,0);
    VL_IN8(s_axi_wstrb,3,0);
    VL_IN8(s_axi_wvalid,0,0);
    VL_OUT8(s_axi_wready,0,0);
    VL_OUT8(s_axi_bresp,1,0);
    VL_OUT8(s_axi_bvalid,0,0);
    VL_IN8(s_axi_bready,0,0);
    VL_IN8(core_idle,0,0);
    VL_IN8(core_pipeline_drained,0,0);
    VL_IN8(flip_ack,0,0);
    VL_OUT8(request_flip,0,0);
    VL_OUT8(layer_start_pulse,0,0);
    VL_IN8(core_mem_req_valid,0,0);
    VL_OUT8(core_mem_req_ready,0,0);
    VL_OUT8(core_mem_resp_valid,0,0);
    VL_OUT8(mem_master_valid,0,0);
    VL_IN8(mem_master_ready,0,0);
    VL_IN8(mem_master_resp_valid,0,0);
    VL_OUT(dbg_cycle_count,31,0);
    VL_IN(s_axi_wdata,31,0);
    VL_IN64(s_axi_awaddr,63,0);
    VL_OUTW(active_config,196,0,7);
    VL_INW(core_mem_req,108,0,4);
    VL_OUTW(core_mem_resp,267,0,9);
    VL_OUTW(mem_master_req,108,0,4);
    VL_INW(mem_master_resp,267,0,9);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*0:0*/ flexpipe_top__DOT__prefetch_req_valid;
    CData/*0:0*/ flexpipe_top__DOT__prefetch_req_ready;
    CData/*0:0*/ flexpipe_top__DOT__prefetch_resp_valid;
    CData/*0:0*/ flexpipe_top__DOT__arb_to_dram_valid;
    CData/*0:0*/ flexpipe_top__DOT__u_config_manager__DOT__shadow_full;
    CData/*3:0*/ flexpipe_top__DOT__u_config_manager__DOT__epoch_counter;
    CData/*0:0*/ flexpipe_top__DOT__u_config_manager__DOT__request_flip_r;
    CData/*2:0*/ flexpipe_top__DOT__u_config_manager__DOT__write_idx;
    CData/*1:0*/ flexpipe_top__DOT__u_config_manager__DOT__axi_state;
    CData/*2:0*/ flexpipe_top__DOT__u_prefetch_dma__DOT__state;
    CData/*2:0*/ flexpipe_top__DOT__u_prefetch_dma__DOT__next_state;
    CData/*3:0*/ flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch;
    CData/*7:0*/ flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt;
    CData/*7:0*/ flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt;
    IData/*31:0*/ flexpipe_top__DOT__dbg_cycle_count_r;
    IData/*31:0*/ flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining;
    IData/*31:0*/ flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum;
    WData/*108:0*/ flexpipe_top__DOT__prefetch_req[4];
    WData/*108:0*/ flexpipe_top__DOT__arb_to_dram_req[4];
    WData/*196:0*/ flexpipe_top__DOT__u_config_manager__DOT__active_config_r[7];
    WData/*196:0*/ flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[7];
    QData/*63:0*/ flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr;
    QData/*63:0*/ flexpipe_top__DOT__u_prefetch_dma__DOT__fetched_data_addr;
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vclklast__TOP__clk;
    CData/*0:0*/ __Vclklast__TOP__rst_n;
    CData/*0:0*/ __Vm_traceActivity[3];
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vflexpipe_top__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vflexpipe_top);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vflexpipe_top(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vflexpipe_top();
    /// Trace signals in the model; called by application code
    void trace(VerilatedVcdC* tfp, int levels, int options = 0);
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vflexpipe_top__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vflexpipe_top__Syms* symsp, bool first);
  private:
    static QData _change_request(Vflexpipe_top__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vflexpipe_top__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__2(Vflexpipe_top__Syms* __restrict vlSymsp);
    static void _combo__TOP__5(Vflexpipe_top__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vflexpipe_top__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vflexpipe_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vflexpipe_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _initial__TOP__1(Vflexpipe_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__4(Vflexpipe_top__Syms* __restrict vlSymsp);
    static void _settle__TOP__3(Vflexpipe_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
  private:
    static void traceChgSub0(void* userp, VerilatedVcd* tracep);
    static void traceChgTop0(void* userp, VerilatedVcd* tracep);
    static void traceCleanup(void* userp, VerilatedVcd* /*unused*/);
    static void traceFullSub0(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceFullTop0(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceInitSub0(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceInitTop(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    void traceRegister(VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceInit(void* userp, VerilatedVcd* tracep, uint32_t code) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
