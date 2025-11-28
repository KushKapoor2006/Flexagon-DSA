// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vflexpipe_top.h for the primary calling header

#include "Vflexpipe_top.h"
#include "Vflexpipe_top__Syms.h"

//==========

void Vflexpipe_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vflexpipe_top::eval\n"); );
    Vflexpipe_top__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        vlSymsp->__Vm_activity = true;
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("src/flexpipe_top.sv", 4, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vflexpipe_top::_eval_initial_loop(Vflexpipe_top__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    vlSymsp->__Vm_activity = true;
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        _eval_settle(vlSymsp);
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("src/flexpipe_top.sv", 4, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vflexpipe_top::_combo__TOP__2(Vflexpipe_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::_combo__TOP__2\n"); );
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
}

VL_INLINE_OPT void Vflexpipe_top::_sequent__TOP__4(Vflexpipe_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::_sequent__TOP__4\n"); );
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*1:0*/ __Vdly__flexpipe_top__DOT__u_config_manager__DOT__axi_state;
    CData/*2:0*/ __Vdly__flexpipe_top__DOT__u_config_manager__DOT__write_idx;
    CData/*0:0*/ __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_full;
    CData/*0:0*/ __Vdly__flexpipe_top__DOT__u_config_manager__DOT__request_flip_r;
    CData/*3:0*/ __Vdly__flexpipe_top__DOT__u_config_manager__DOT__epoch_counter;
    CData/*2:0*/ __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__state;
    CData/*3:0*/ __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch;
    CData/*7:0*/ __Vdly__flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt;
    CData/*7:0*/ __Vdly__flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt;
    IData/*31:0*/ __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining;
    IData/*31:0*/ __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum;
    WData/*196:0*/ __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[7];
    QData/*63:0*/ __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr;
    // Body
    __Vdly__flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt 
        = vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt;
    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__epoch_counter 
        = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__epoch_counter;
    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_full 
        = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_full;
    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__write_idx 
        = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__write_idx;
    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__request_flip_r 
        = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__request_flip_r;
    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__axi_state 
        = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__axi_state;
    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U] 
        = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U];
    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[1U] 
        = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[1U];
    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[2U] 
        = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[2U];
    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[3U] 
        = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[3U];
    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[4U] 
        = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[4U];
    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[5U] 
        = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[5U];
    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[6U] 
        = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[6U];
    __Vdly__flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt 
        = vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt;
    __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum 
        = vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum;
    __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining 
        = vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining;
    __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr 
        = vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr;
    __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch 
        = vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch;
    __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__state 
        = vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state;
    if (vlTOPp->rst_n) {
        if (((IData)(vlTOPp->core_mem_req_valid) & (IData)(vlTOPp->core_mem_req_ready))) {
            __Vdly__flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt 
                = (0xffU & ((IData)(1U) + (IData)(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt)));
        }
        if (((IData)(vlTOPp->mem_master_resp_valid) 
             & (vlTOPp->mem_master_resp[0U] >> 1U))) {
            if (((~ (vlTOPp->mem_master_resp[0U] >> 0xbU)) 
                 & (0U < (IData)(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt)))) {
                __Vdly__flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt 
                    = (0xffU & ((IData)(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt) 
                                - (IData)(1U)));
            }
        }
    } else {
        __Vdly__flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->dbg_mode) {
            vlTOPp->flexpipe_top__DOT__dbg_cycle_count_r 
                = ((IData)(1U) + vlTOPp->flexpipe_top__DOT__dbg_cycle_count_r);
        }
    } else {
        vlTOPp->flexpipe_top__DOT__dbg_cycle_count_r = 0U;
    }
    if (vlTOPp->rst_n) {
        if ((0U == (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__axi_state))) {
            vlTOPp->s_axi_bvalid = 0U;
            if (vlTOPp->s_axi_awvalid) {
                __Vdly__flexpipe_top__DOT__u_config_manager__DOT__axi_state = 1U;
            }
        } else {
            if ((1U == (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__axi_state))) {
                if (vlTOPp->s_axi_wvalid) {
                    if ((0U == (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__write_idx))) {
                        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[5U] 
                            = ((0x1fU & __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[5U]) 
                               | (0xffffffe0U & (vlTOPp->s_axi_wdata 
                                                 << 5U)));
                        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[6U] 
                            = (0x1fU & (vlTOPp->s_axi_wdata 
                                        >> 0x1bU));
                    } else {
                        if ((1U == (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__write_idx))) {
                            __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[4U] 
                                = ((0x1fU & __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[4U]) 
                                   | (0xffffffe0U & 
                                      (vlTOPp->s_axi_wdata 
                                       << 5U)));
                            __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[5U] 
                                = ((0xffffffe0U & __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[5U]) 
                                   | (0x1fU & (vlTOPp->s_axi_wdata 
                                               >> 0x1bU)));
                        } else {
                            if ((2U == (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__write_idx))) {
                                __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[3U] 
                                    = ((0x1fU & __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[3U]) 
                                       | (0xffffffe0U 
                                          & (vlTOPp->s_axi_wdata 
                                             << 5U)));
                                __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[4U] 
                                    = ((0xffffffe0U 
                                        & __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[4U]) 
                                       | (0x1fU & (vlTOPp->s_axi_wdata 
                                                   >> 0x1bU)));
                            } else {
                                if ((3U == (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__write_idx))) {
                                    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[2U] 
                                        = ((0x1fU & 
                                            __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[2U]) 
                                           | (0xffffffe0U 
                                              & (vlTOPp->s_axi_wdata 
                                                 << 5U)));
                                    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[3U] 
                                        = ((0xffffffe0U 
                                            & __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[3U]) 
                                           | (0x1fU 
                                              & (vlTOPp->s_axi_wdata 
                                                 >> 0x1bU)));
                                }
                            }
                        }
                    }
                    vlTOPp->s_axi_bvalid = 1U;
                    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__axi_state = 2U;
                    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__write_idx 
                        = (7U & ((IData)(1U) + (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__write_idx)));
                    if ((3U == (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__write_idx))) {
                        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_full = 1U;
                        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U] 
                            = (0x10U | __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U]);
                        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__write_idx = 0U;
                    }
                }
            } else {
                if ((2U == (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__axi_state))) {
                    if (vlTOPp->s_axi_bready) {
                        vlTOPp->s_axi_bvalid = 0U;
                        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__axi_state = 0U;
                    }
                } else {
                    __Vdly__flexpipe_top__DOT__u_config_manager__DOT__axi_state = 0U;
                }
            }
        }
        vlTOPp->layer_start_pulse = 0U;
        if (((((IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_full) 
               & ((IData)(vlTOPp->core_idle) & (IData)(vlTOPp->core_pipeline_drained))) 
              & (0U == (IData)(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt))) 
             & (~ (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__request_flip_r)))) {
            __Vdly__flexpipe_top__DOT__u_config_manager__DOT__request_flip_r = 1U;
        }
        if (((IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__request_flip_r) 
             & (IData)(vlTOPp->flip_ack))) {
            vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[0U] 
                = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U];
            vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[1U] 
                = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[1U];
            vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[2U] 
                = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[2U];
            vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[3U] 
                = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[3U];
            vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[4U] 
                = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[4U];
            vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[5U] 
                = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[5U];
            vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[6U] 
                = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[6U];
            __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_full = 0U;
            __Vdly__flexpipe_top__DOT__u_config_manager__DOT__request_flip_r = 0U;
            vlTOPp->layer_start_pulse = 1U;
            __Vdly__flexpipe_top__DOT__u_config_manager__DOT__epoch_counter 
                = (0xfU & ((IData)(1U) + (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__epoch_counter)));
            vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[0U] 
                = ((0xfffffff0U & vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[0U]) 
                   | (0xfU & ((IData)(1U) + (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__epoch_counter))));
            vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[0U] 
                = (0x10U | vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[0U]);
            __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U] 
                = (0xffffffefU & __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U]);
        }
    } else {
        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_full = 0U;
        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U] = 0U;
        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[1U] = 0U;
        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[2U] = 0U;
        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[3U] = 0U;
        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[4U] = 0U;
        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[5U] = 0U;
        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[6U] = 0U;
        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__axi_state = 0U;
        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__write_idx = 0U;
        vlTOPp->s_axi_bvalid = 0U;
        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__epoch_counter = 0U;
        __Vdly__flexpipe_top__DOT__u_config_manager__DOT__request_flip_r = 0U;
        vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[0U] = 0U;
        vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[1U] = 0U;
        vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[2U] = 0U;
        vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[3U] = 0U;
        vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[4U] = 0U;
        vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[5U] = 0U;
        vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[6U] = 0U;
        vlTOPp->layer_start_pulse = 0U;
    }
    if (vlTOPp->rst_n) {
        if (((IData)(vlTOPp->flexpipe_top__DOT__prefetch_req_valid) 
             & (IData)(vlTOPp->flexpipe_top__DOT__prefetch_req_ready))) {
            __Vdly__flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt 
                = (0xffU & ((IData)(1U) + (IData)(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt)));
        }
        if (((IData)(vlTOPp->mem_master_resp_valid) 
             & (vlTOPp->mem_master_resp[0U] >> 1U))) {
            if (((vlTOPp->mem_master_resp[0U] >> 0xbU) 
                 & (0U < (IData)(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt)))) {
                __Vdly__flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt 
                    = (0xffU & ((IData)(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt) 
                                - (IData)(1U)));
            }
        }
    } else {
        __Vdly__flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt = 0U;
    }
    if (vlTOPp->rst_n) {
        __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__state 
            = vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__next_state;
        if (((0U == (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state)) 
             & (1U == (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__next_state)))) {
            __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr 
                = (((QData)((IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[2U])) 
                    << 0x3bU) | (((QData)((IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[1U])) 
                                  << 0x1bU) | ((QData)((IData)(
                                                               vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U])) 
                                               >> 5U)));
            __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining 
                = ((vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[4U] 
                    << 0x1bU) | (vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[3U] 
                                 >> 5U));
            __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch 
                = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__epoch_counter;
            __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum = 0U;
        }
        if ((((2U == (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state)) 
              & (IData)(vlTOPp->flexpipe_top__DOT__prefetch_resp_valid)) 
             & ((0xfU & ((vlTOPp->mem_master_resp[1U] 
                          << 0x1eU) | (vlTOPp->mem_master_resp[0U] 
                                       >> 2U))) == (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch)))) {
            vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__fetched_data_addr 
                = (((QData)((IData)(vlTOPp->mem_master_resp[2U])) 
                    << 0x34U) | (((QData)((IData)(vlTOPp->mem_master_resp[1U])) 
                                  << 0x14U) | ((QData)((IData)(
                                                               vlTOPp->mem_master_resp[0U])) 
                                               >> 0xcU)));
        }
        if (((((4U == (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state)) 
               & (IData)(vlTOPp->flexpipe_top__DOT__prefetch_resp_valid)) 
              & ((0xfU & ((vlTOPp->mem_master_resp[1U] 
                           << 0x1eU) | (vlTOPp->mem_master_resp[0U] 
                                        >> 2U))) == (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch))) 
             & (vlTOPp->mem_master_resp[0U] >> 1U))) {
            __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum 
                = ((IData)(0x20U) + vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum);
            if ((0U < vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining)) {
                __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining 
                    = (vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining 
                       - (IData)(1U));
                __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr 
                    = (8ULL + vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr);
            }
        }
        if (((0U != (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state)) 
             & ((IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch) 
                != (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__epoch_counter)))) {
            __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__state = 0U;
        }
    } else {
        __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr = 0ULL;
        __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining = 0U;
        __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum = 0U;
        __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__state = 0U;
        vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__fetched_data_addr = 0ULL;
        __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch = 0U;
    }
    vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__write_idx 
        = __Vdly__flexpipe_top__DOT__u_config_manager__DOT__write_idx;
    vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_full 
        = __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_full;
    vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt 
        = __Vdly__flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt;
    vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__request_flip_r 
        = __Vdly__flexpipe_top__DOT__u_config_manager__DOT__request_flip_r;
    vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__axi_state 
        = __Vdly__flexpipe_top__DOT__u_config_manager__DOT__axi_state;
    vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt 
        = __Vdly__flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt;
    vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__epoch_counter 
        = __Vdly__flexpipe_top__DOT__u_config_manager__DOT__epoch_counter;
    vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum 
        = __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum;
    vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U] 
        = __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U];
    vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[1U] 
        = __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[1U];
    vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[2U] 
        = __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[2U];
    vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[3U] 
        = __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[3U];
    vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[4U] 
        = __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[4U];
    vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[5U] 
        = __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[5U];
    vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[6U] 
        = __Vdly__flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[6U];
    vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining 
        = __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining;
    vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr 
        = __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr;
    vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch 
        = __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch;
    vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state 
        = __Vdly__flexpipe_top__DOT__u_prefetch_dma__DOT__state;
    vlTOPp->dbg_cycle_count = vlTOPp->flexpipe_top__DOT__dbg_cycle_count_r;
    vlTOPp->request_flip = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__request_flip_r;
    vlTOPp->active_config[0U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[0U];
    vlTOPp->active_config[1U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[1U];
    vlTOPp->active_config[2U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[2U];
    vlTOPp->active_config[3U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[3U];
    vlTOPp->active_config[4U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[4U];
    vlTOPp->active_config[5U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[5U];
    vlTOPp->active_config[6U] = vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r[6U];
    vlTOPp->s_axi_awready = (0U == (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__axi_state));
    vlTOPp->s_axi_wready = (1U == (IData)(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__axi_state));
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
}

VL_INLINE_OPT void Vflexpipe_top::_combo__TOP__5(Vflexpipe_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::_combo__TOP__5\n"); );
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
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

void Vflexpipe_top::_eval(Vflexpipe_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::_eval\n"); );
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__2(vlSymsp);
    vlTOPp->__Vm_traceActivity[1U] = 1U;
    if ((((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk))) 
         | ((~ (IData)(vlTOPp->rst_n)) & (IData)(vlTOPp->__Vclklast__TOP__rst_n)))) {
        vlTOPp->_sequent__TOP__4(vlSymsp);
        vlTOPp->__Vm_traceActivity[2U] = 1U;
    }
    vlTOPp->_combo__TOP__5(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
    vlTOPp->__Vclklast__TOP__rst_n = vlTOPp->rst_n;
}

VL_INLINE_OPT QData Vflexpipe_top::_change_request(Vflexpipe_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::_change_request\n"); );
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vflexpipe_top::_change_request_1(Vflexpipe_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::_change_request_1\n"); );
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vflexpipe_top::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vflexpipe_top::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((rst_n & 0xfeU))) {
        Verilated::overWidthError("rst_n");}
    if (VL_UNLIKELY((dbg_mode & 0xfeU))) {
        Verilated::overWidthError("dbg_mode");}
    if (VL_UNLIKELY((s_axi_awvalid & 0xfeU))) {
        Verilated::overWidthError("s_axi_awvalid");}
    if (VL_UNLIKELY((s_axi_wstrb & 0xf0U))) {
        Verilated::overWidthError("s_axi_wstrb");}
    if (VL_UNLIKELY((s_axi_wvalid & 0xfeU))) {
        Verilated::overWidthError("s_axi_wvalid");}
    if (VL_UNLIKELY((s_axi_bready & 0xfeU))) {
        Verilated::overWidthError("s_axi_bready");}
    if (VL_UNLIKELY((core_idle & 0xfeU))) {
        Verilated::overWidthError("core_idle");}
    if (VL_UNLIKELY((core_pipeline_drained & 0xfeU))) {
        Verilated::overWidthError("core_pipeline_drained");}
    if (VL_UNLIKELY((flip_ack & 0xfeU))) {
        Verilated::overWidthError("flip_ack");}
    if (VL_UNLIKELY((core_mem_req_valid & 0xfeU))) {
        Verilated::overWidthError("core_mem_req_valid");}
    if (VL_UNLIKELY((mem_master_ready & 0xfeU))) {
        Verilated::overWidthError("mem_master_ready");}
    if (VL_UNLIKELY((mem_master_resp_valid & 0xfeU))) {
        Verilated::overWidthError("mem_master_resp_valid");}
}
#endif  // VL_DEBUG
