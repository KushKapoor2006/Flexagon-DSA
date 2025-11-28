// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vflexpipe_top.h for the primary calling header

#include "Vflexpipe_top.h"
#include "Vflexpipe_top__Syms.h"

//==========

VL_CTOR_IMP(Vflexpipe_top) {
    Vflexpipe_top__Syms* __restrict vlSymsp = __VlSymsp = new Vflexpipe_top__Syms(this, name());
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vflexpipe_top::__Vconfigure(Vflexpipe_top__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vflexpipe_top::~Vflexpipe_top() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vflexpipe_top::_initial__TOP__1(Vflexpipe_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::_initial__TOP__1\n"); );
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->s_axi_bresp = 0U;
    vlTOPp->core_mem_resp_valid = 0U;
    vlTOPp->flexpipe_top__DOT__prefetch_req[0U] = 0U;
    vlTOPp->flexpipe_top__DOT__prefetch_req[1U] = 0U;
    vlTOPp->flexpipe_top__DOT__prefetch_req[2U] = 0U;
    vlTOPp->flexpipe_top__DOT__prefetch_req[3U] = 0U;
    vlTOPp->flexpipe_top__DOT__prefetch_resp_valid = 0U;
    vlTOPp->flexpipe_top__DOT__prefetch_req_valid = 0U;
}

void Vflexpipe_top::_settle__TOP__3(Vflexpipe_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::_settle__TOP__3\n"); );
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->core_mem_resp[0U] = vlTOPp->mem_master_resp[0U];
    vlTOPp->core_mem_resp[1U] = vlTOPp->mem_master_resp[1U];
    vlTOPp->core_mem_resp[2U] = vlTOPp->mem_master_resp[2U];
    vlTOPp->core_mem_resp[3U] = vlTOPp->mem_master_resp[3U];
    vlTOPp->core_mem_resp[4U] = vlTOPp->mem_master_resp[4U];
    vlTOPp->core_mem_resp[5U] = vlTOPp->mem_master_resp[5U];
    vlTOPp->core_mem_resp[6U] = vlTOPp->mem_master_resp[6U];
    vlTOPp->core_mem_resp[7U] = vlTOPp->mem_master_resp[7U];
    vlTOPp->core_mem_resp[8U] = vlTOPp->mem_master_resp[8U];
    vlTOPp->core_mem_resp_valid = 0U;
    if (vlTOPp->mem_master_resp_valid) {
        if ((1U & (~ (vlTOPp->mem_master_resp[0U] >> 0xbU)))) {
            vlTOPp->core_mem_resp_valid = 1U;
        }
    }
    vlTOPp->core_mem_req_ready = 0U;
    if (vlTOPp->core_mem_req_valid) {
        vlTOPp->core_mem_req_ready = vlTOPp->mem_master_ready;
    }
    vlTOPp->flexpipe_top__DOT__prefetch_resp_valid = 0U;
    if (vlTOPp->mem_master_resp_valid) {
        if ((0x800U & vlTOPp->mem_master_resp[0U])) {
            vlTOPp->flexpipe_top__DOT__prefetch_resp_valid = 1U;
        }
    }
    vlTOPp->dbg_cycle_count = vlTOPp->flexpipe_top__DOT__dbg_cycle_count_r;
    vlTOPp->s_axi_awready = (0U == (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__axi_state));
    vlTOPp->s_axi_wready = (1U == (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__axi_state));
    vlTOPp->request_flip = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__request_flip_r;
    vlTOPp->active_config[0U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[0U];
    vlTOPp->active_config[1U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[1U];
    vlTOPp->active_config[2U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[2U];
    vlTOPp->active_config[3U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[3U];
    vlTOPp->active_config[4U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[4U];
    vlTOPp->active_config[5U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[5U];
    vlTOPp->active_config[6U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[6U];
    if ((1U & (~ ((IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))) {
            if ((1U & (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))) {
                vlTOPp->flexpipe_top__DOT__prefetch_req[1U] 
                    = ((0x1fffU & vlTOPp->flexpipe_top__DOT__prefetch_req[1U]) 
                       | (0xffffe000U & ((IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__fetched_data_addr) 
                                         << 0xdU)));
                vlTOPp->flexpipe_top__DOT__prefetch_req[2U] 
                    = ((0x1fffU & ((IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__fetched_data_addr) 
                                   >> 0x13U)) | (0xffffe000U 
                                                 & ((IData)(
                                                            (vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__fetched_data_addr 
                                                             >> 0x20U)) 
                                                    << 0xdU)));
                vlTOPp->flexpipe_top__DOT__prefetch_req[3U] 
                    = (0x1fffU & ((IData)((vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__fetched_data_addr 
                                           >> 0x20U)) 
                                  >> 0x13U));
                vlTOPp->flexpipe_top__DOT__prefetch_req[0U] 
                    = (0x40000U | (0x1fffU & vlTOPp->flexpipe_top__DOT__prefetch_req[0U]));
                vlTOPp->flexpipe_top__DOT__prefetch_req[1U] 
                    = (0xffffe000U & vlTOPp->flexpipe_top__DOT__prefetch_req[1U]);
                vlTOPp->flexpipe_top__DOT__prefetch_req[0U] 
                    = (4U | (0xfffffff9U & vlTOPp->flexpipe_top__DOT__prefetch_req[0U]));
                vlTOPp->flexpipe_top__DOT__prefetch_req[0U] 
                    = (0xfffffffeU & vlTOPp->flexpipe_top__DOT__prefetch_req[0U]);
                vlTOPp->flexpipe_top__DOT__prefetch_req[0U] 
                    = ((0xffffff87U & vlTOPp->flexpipe_top__DOT__prefetch_req[0U]) 
                       | (0xfffffff8U & ((IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch) 
                                         << 3U)));
            }
        } else {
            if ((1U & (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))) {
                vlTOPp->flexpipe_top__DOT__prefetch_req[1U] 
                    = ((0x1fffU & vlTOPp->flexpipe_top__DOT__prefetch_req[1U]) 
                       | (0xffffe000U & ((IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr) 
                                         << 0xdU)));
                vlTOPp->flexpipe_top__DOT__prefetch_req[2U] 
                    = ((0x1fffU & ((IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr) 
                                   >> 0x13U)) | (0xffffe000U 
                                                 & ((IData)(
                                                            (vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr 
                                                             >> 0x20U)) 
                                                    << 0xdU)));
                vlTOPp->flexpipe_top__DOT__prefetch_req[3U] 
                    = (0x1fffU & ((IData)((vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr 
                                           >> 0x20U)) 
                                  >> 0x13U));
                vlTOPp->flexpipe_top__DOT__prefetch_req[0U] 
                    = (0x10000U | (0x1fffU & vlTOPp->flexpipe_top__DOT__prefetch_req[0U]));
                vlTOPp->flexpipe_top__DOT__prefetch_req[1U] 
                    = (0xffffe000U & vlTOPp->flexpipe_top__DOT__prefetch_req[1U]);
                vlTOPp->flexpipe_top__DOT__prefetch_req[0U] 
                    = (2U | (0xfffffff9U & vlTOPp->flexpipe_top__DOT__prefetch_req[0U]));
                vlTOPp->flexpipe_top__DOT__prefetch_req[0U] 
                    = (0xfffffffeU & vlTOPp->flexpipe_top__DOT__prefetch_req[0U]);
                vlTOPp->flexpipe_top__DOT__prefetch_req[0U] 
                    = ((0xffffff87U & vlTOPp->flexpipe_top__DOT__prefetch_req[0U]) 
                       | (0xfffffff8U & ((IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch) 
                                         << 3U)));
            }
        }
    }
    vlTOPp->flexpipe_top__DOT__prefetch_req_valid = 0U;
    if ((1U & (~ ((IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state) 
                  >> 2U)))) {
        if ((2U & (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))) {
            if ((1U & (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))) {
                vlTOPp->flexpipe_top__DOT__prefetch_req_valid = 1U;
            }
        } else {
            if ((1U & (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))) {
                vlTOPp->flexpipe_top__DOT__prefetch_req_valid = 1U;
            }
        }
    }
    vlTOPp->flexpipe_top__DOT__arb_to_dram_valid = 0U;
    if (vlTOPp->core_mem_req_valid) {
        vlTOPp->flexpipe_top__DOT__arb_to_dram_valid = 1U;
    } else {
        if (vlTOPp->flexpipe_top__DOT__prefetch_req_valid) {
            vlTOPp->flexpipe_top__DOT__arb_to_dram_valid = 1U;
        }
    }
    vlTOPp->flexpipe_top__DOT__arb_to_dram_req[0U] = 0U;
    vlTOPp->flexpipe_top__DOT__arb_to_dram_req[1U] = 0U;
    vlTOPp->flexpipe_top__DOT__arb_to_dram_req[2U] = 0U;
    vlTOPp->flexpipe_top__DOT__arb_to_dram_req[3U] = 0U;
    if (vlTOPp->core_mem_req_valid) {
        vlTOPp->flexpipe_top__DOT__arb_to_dram_req[0U] 
            = vlTOPp->core_mem_req[0U];
        vlTOPp->flexpipe_top__DOT__arb_to_dram_req[1U] 
            = vlTOPp->core_mem_req[1U];
        vlTOPp->flexpipe_top__DOT__arb_to_dram_req[2U] 
            = vlTOPp->core_mem_req[2U];
        vlTOPp->flexpipe_top__DOT__arb_to_dram_req[3U] 
            = vlTOPp->core_mem_req[3U];
        vlTOPp->flexpipe_top__DOT__arb_to_dram_req[0U] 
            = (0xffffefffU & vlTOPp->flexpipe_top__DOT__arb_to_dram_req[0U]);
    } else {
        if (vlTOPp->flexpipe_top__DOT__prefetch_req_valid) {
            vlTOPp->flexpipe_top__DOT__arb_to_dram_req[0U] 
                = vlTOPp->flexpipe_top__DOT__prefetch_req[0U];
            vlTOPp->flexpipe_top__DOT__arb_to_dram_req[1U] 
                = vlTOPp->flexpipe_top__DOT__prefetch_req[1U];
            vlTOPp->flexpipe_top__DOT__arb_to_dram_req[2U] 
                = vlTOPp->flexpipe_top__DOT__prefetch_req[2U];
            vlTOPp->flexpipe_top__DOT__arb_to_dram_req[3U] 
                = vlTOPp->flexpipe_top__DOT__prefetch_req[3U];
            vlTOPp->flexpipe_top__DOT__arb_to_dram_req[0U] 
                = (0x1000U | vlTOPp->flexpipe_top__DOT__arb_to_dram_req[0U]);
        }
    }
    vlTOPp->flexpipe_top__DOT__prefetch_req_ready = 0U;
    if ((1U & (~ (IData)(vlTOPp->core_mem_req_valid)))) {
        if (vlTOPp->flexpipe_top__DOT__prefetch_req_valid) {
            vlTOPp->flexpipe_top__DOT__prefetch_req_ready 
                = vlTOPp->mem_master_ready;
        }
    }
    vlTOPp->mem_master_valid = vlTOPp->flexpipe_top__DOT__arb_to_dram_valid;
    vlTOPp->mem_master_req[0U] = vlTOPp->flexpipe_top__DOT__arb_to_dram_req[0U];
    vlTOPp->mem_master_req[1U] = vlTOPp->flexpipe_top__DOT__arb_to_dram_req[1U];
    vlTOPp->mem_master_req[2U] = vlTOPp->flexpipe_top__DOT__arb_to_dram_req[2U];
    vlTOPp->mem_master_req[3U] = vlTOPp->flexpipe_top__DOT__arb_to_dram_req[3U];
    vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__next_state 
        = vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state;
    if ((4U & (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))) {
        if ((2U & (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))) {
            vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__next_state = 0U;
        } else {
            if ((1U & (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))) {
                if ((1U & (~ (vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U] 
                              >> 4U)))) {
                    vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__next_state = 0U;
                }
            } else {
                if (((((IData)(vlTOPp->flexpipe_top__DOT__prefetch_resp_valid) 
                       & ((0xfU & ((vlTOPp->mem_master_resp[1U] 
                                    << 0x1eU) | (vlTOPp->mem_master_resp[0U] 
                                                 >> 2U))) 
                          == (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch))) 
                      & (0U == (0x3fU & ((vlTOPp->mem_master_resp[1U] 
                                          << 0x1aU) 
                                         | (vlTOPp->mem_master_resp[0U] 
                                            >> 6U))))) 
                     & (vlTOPp->mem_master_resp[0U] 
                        >> 1U))) {
                    vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__next_state 
                        = ((0U < vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining)
                            ? 1U : 5U);
                }
            }
        }
    } else {
        if ((2U & (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))) {
            if ((1U & (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))) {
                if (vlTOPp->flexpipe_top__DOT__prefetch_req_ready) {
                    vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__next_state = 4U;
                }
            } else {
                if ((((IData)(vlTOPp->flexpipe_top__DOT__prefetch_resp_valid) 
                      & ((0xfU & ((vlTOPp->mem_master_resp[1U] 
                                   << 0x1eU) | (vlTOPp->mem_master_resp[0U] 
                                                >> 2U))) 
                         == (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch))) 
                     & (0U == (0x3fU & ((vlTOPp->mem_master_resp[1U] 
                                         << 0x1aU) 
                                        | (vlTOPp->mem_master_resp[0U] 
                                           >> 6U)))))) {
                    vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__next_state = 3U;
                }
            }
        } else {
            if ((1U & (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))) {
                if (vlTOPp->flexpipe_top__DOT__prefetch_req_ready) {
                    vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__next_state = 2U;
                }
            } else {
                if ((1U & (((vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U] 
                             >> 4U) & (~ ((0U != (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state)) 
                                          & (5U != (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))))) 
                           & (vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U] 
                              >> 4U)))) {
                    vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__next_state = 1U;
                }
            }
        }
    }
}

void Vflexpipe_top::_eval_initial(Vflexpipe_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::_eval_initial\n"); );
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_initial__TOP__1(vlSymsp);
    vlTOPp->__Vm_traceActivity[2U] = 1U;
    vlTOPp->__Vm_traceActivity[1U] = 1U;
    vlTOPp->__Vm_traceActivity[0U] = 1U;
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
    vlTOPp->__Vclklast__TOP__rst_n = vlTOPp->rst_n;
}

void Vflexpipe_top::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::final\n"); );
    // Variables
    Vflexpipe_top__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vflexpipe_top::_eval_settle(Vflexpipe_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::_eval_settle\n"); );
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__3(vlSymsp);
    vlTOPp->__Vm_traceActivity[2U] = 1U;
    vlTOPp->__Vm_traceActivity[1U] = 1U;
    vlTOPp->__Vm_traceActivity[0U] = 1U;
}

void Vflexpipe_top::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::_ctor_var_reset\n"); );
    // Body
    clk = VL_RAND_RESET_I(1);
    rst_n = VL_RAND_RESET_I(1);
    dbg_mode = VL_RAND_RESET_I(1);
    dbg_cycle_count = VL_RAND_RESET_I(32);
    s_axi_awaddr = VL_RAND_RESET_Q(64);
    s_axi_awvalid = VL_RAND_RESET_I(1);
    s_axi_awready = VL_RAND_RESET_I(1);
    s_axi_wdata = VL_RAND_RESET_I(32);
    s_axi_wstrb = VL_RAND_RESET_I(4);
    s_axi_wvalid = VL_RAND_RESET_I(1);
    s_axi_wready = VL_RAND_RESET_I(1);
    s_axi_bresp = VL_RAND_RESET_I(2);
    s_axi_bvalid = VL_RAND_RESET_I(1);
    s_axi_bready = VL_RAND_RESET_I(1);
    core_idle = VL_RAND_RESET_I(1);
    core_pipeline_drained = VL_RAND_RESET_I(1);
    flip_ack = VL_RAND_RESET_I(1);
    request_flip = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(197, active_config);
    layer_start_pulse = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(109, core_mem_req);
    core_mem_req_valid = VL_RAND_RESET_I(1);
    core_mem_req_ready = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(268, core_mem_resp);
    core_mem_resp_valid = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(109, mem_master_req);
    mem_master_valid = VL_RAND_RESET_I(1);
    mem_master_ready = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(268, mem_master_resp);
    mem_master_resp_valid = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(109, flexpipe_top__DOT__prefetch_req);
    flexpipe_top__DOT__prefetch_req_valid = VL_RAND_RESET_I(1);
    flexpipe_top__DOT__prefetch_req_ready = VL_RAND_RESET_I(1);
    flexpipe_top__DOT__prefetch_resp_valid = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(109, flexpipe_top__DOT__arb_to_dram_req);
    flexpipe_top__DOT__arb_to_dram_valid = VL_RAND_RESET_I(1);
    flexpipe_top__DOT__dbg_cycle_count_r = VL_RAND_RESET_I(32);
    VL_RAND_RESET_W(197, flexpipe_top__DOT__u_config_manager__DOT__active_config_r);
    VL_RAND_RESET_W(197, flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r);
    flexpipe_top__DOT__u_config_manager__DOT__shadow_full = VL_RAND_RESET_I(1);
    flexpipe_top__DOT__u_config_manager__DOT__epoch_counter = VL_RAND_RESET_I(4);
    flexpipe_top__DOT__u_config_manager__DOT__request_flip_r = VL_RAND_RESET_I(1);
    flexpipe_top__DOT__u_config_manager__DOT__write_idx = VL_RAND_RESET_I(3);
    flexpipe_top__DOT__u_config_manager__DOT__axi_state = VL_RAND_RESET_I(2);
    flexpipe_top__DOT__u_prefetch_dma__DOT__state = VL_RAND_RESET_I(3);
    flexpipe_top__DOT__u_prefetch_dma__DOT__next_state = VL_RAND_RESET_I(3);
    flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr = VL_RAND_RESET_Q(64);
    flexpipe_top__DOT__u_prefetch_dma__DOT__fetched_data_addr = VL_RAND_RESET_Q(64);
    flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining = VL_RAND_RESET_I(32);
    flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch = VL_RAND_RESET_I(4);
    flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum = VL_RAND_RESET_I(32);
    flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt = VL_RAND_RESET_I(8);
    flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt = VL_RAND_RESET_I(8);
    { int __Vi0=0; for (; __Vi0<3; ++__Vi0) {
            __Vm_traceActivity[__Vi0] = VL_RAND_RESET_I(1);
    }}
}
