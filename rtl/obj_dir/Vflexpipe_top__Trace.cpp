// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vflexpipe_top__Syms.h"


void Vflexpipe_top::traceChgTop0(void* userp, VerilatedVcd* tracep) {
    Vflexpipe_top__Syms* __restrict vlSymsp = static_cast<Vflexpipe_top__Syms*>(userp);
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    {
        vlTOPp->traceChgSub0(userp, tracep);
    }
}

void Vflexpipe_top::traceChgSub0(void* userp, VerilatedVcd* tracep) {
    Vflexpipe_top__Syms* __restrict vlSymsp = static_cast<Vflexpipe_top__Syms*>(userp);
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    vluint32_t* const oldp = tracep->oldp(vlSymsp->__Vm_baseCode + 1);
    if (false && oldp) {}  // Prevent unused
    // Body
    {
        if (VL_UNLIKELY(vlTOPp->__Vm_traceActivity[1U])) {
            tracep->chgBit(oldp+0,(vlTOPp->flexpipe_top__DOT__prefetch_req_ready));
            tracep->chgBit(oldp+1,(vlTOPp->flexpipe_top__DOT__prefetch_resp_valid));
            tracep->chgWData(oldp+2,(vlTOPp->flexpipe_top__DOT__arb_to_dram_req),109);
            tracep->chgBit(oldp+6,(vlTOPp->flexpipe_top__DOT__arb_to_dram_valid));
            tracep->chgCData(oldp+7,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__next_state),3);
        }
        if (VL_UNLIKELY(vlTOPp->__Vm_traceActivity[2U])) {
            tracep->chgWData(oldp+8,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r),197);
            tracep->chgCData(oldp+15,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__epoch_counter),4);
            tracep->chgBit(oldp+16,(((0U != (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state)) 
                                     & (5U != (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state)))));
            tracep->chgIData(oldp+17,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum),32);
            tracep->chgWData(oldp+18,(vlTOPp->flexpipe_top__DOT__prefetch_req),109);
            tracep->chgBit(oldp+22,(vlTOPp->flexpipe_top__DOT__prefetch_req_valid));
            tracep->chgBit(oldp+23,((0U == (IData)(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt))));
            tracep->chgIData(oldp+24,(vlTOPp->flexpipe_top__DOT__dbg_cycle_count_r),32);
            tracep->chgBit(oldp+25,((1U & ((vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U] 
                                            >> 4U) 
                                           & (~ ((0U 
                                                  != (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state)) 
                                                 & (5U 
                                                    != (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))))))));
            tracep->chgWData(oldp+26,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r),197);
            tracep->chgBit(oldp+33,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_full));
            tracep->chgBit(oldp+34,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__request_flip_r));
            tracep->chgCData(oldp+35,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__write_idx),3);
            tracep->chgCData(oldp+36,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__axi_state),2);
            tracep->chgCData(oldp+37,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state),3);
            tracep->chgQData(oldp+38,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr),64);
            tracep->chgQData(oldp+40,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__fetched_data_addr),64);
            tracep->chgIData(oldp+42,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining),32);
            tracep->chgCData(oldp+43,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch),4);
            tracep->chgCData(oldp+44,(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt),8);
            tracep->chgCData(oldp+45,(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt),8);
        }
        tracep->chgBit(oldp+46,(vlTOPp->clk));
        tracep->chgBit(oldp+47,(vlTOPp->rst_n));
        tracep->chgBit(oldp+48,(vlTOPp->dbg_mode));
        tracep->chgIData(oldp+49,(vlTOPp->dbg_cycle_count),32);
        tracep->chgQData(oldp+50,(vlTOPp->s_axi_awaddr),64);
        tracep->chgBit(oldp+52,(vlTOPp->s_axi_awvalid));
        tracep->chgBit(oldp+53,(vlTOPp->s_axi_awready));
        tracep->chgIData(oldp+54,(vlTOPp->s_axi_wdata),32);
        tracep->chgCData(oldp+55,(vlTOPp->s_axi_wstrb),4);
        tracep->chgBit(oldp+56,(vlTOPp->s_axi_wvalid));
        tracep->chgBit(oldp+57,(vlTOPp->s_axi_wready));
        tracep->chgCData(oldp+58,(vlTOPp->s_axi_bresp),2);
        tracep->chgBit(oldp+59,(vlTOPp->s_axi_bvalid));
        tracep->chgBit(oldp+60,(vlTOPp->s_axi_bready));
        tracep->chgBit(oldp+61,(vlTOPp->core_idle));
        tracep->chgBit(oldp+62,(vlTOPp->core_pipeline_drained));
        tracep->chgBit(oldp+63,(vlTOPp->flip_ack));
        tracep->chgBit(oldp+64,(vlTOPp->request_flip));
        tracep->chgWData(oldp+65,(vlTOPp->active_config),197);
        tracep->chgBit(oldp+72,(vlTOPp->layer_start_pulse));
        tracep->chgWData(oldp+73,(vlTOPp->core_mem_req),109);
        tracep->chgBit(oldp+77,(vlTOPp->core_mem_req_valid));
        tracep->chgBit(oldp+78,(vlTOPp->core_mem_req_ready));
        tracep->chgWData(oldp+79,(vlTOPp->core_mem_resp),268);
        tracep->chgBit(oldp+88,(vlTOPp->core_mem_resp_valid));
        tracep->chgWData(oldp+89,(vlTOPp->mem_master_req),109);
        tracep->chgBit(oldp+93,(vlTOPp->mem_master_valid));
        tracep->chgBit(oldp+94,(vlTOPp->mem_master_ready));
        tracep->chgWData(oldp+95,(vlTOPp->mem_master_resp),268);
        tracep->chgBit(oldp+104,(vlTOPp->mem_master_resp_valid));
        tracep->chgBit(oldp+105,(((IData)(vlTOPp->core_idle) 
                                  & (IData)(vlTOPp->core_pipeline_drained))));
    }
}

void Vflexpipe_top::traceCleanup(void* userp, VerilatedVcd* /*unused*/) {
    Vflexpipe_top__Syms* __restrict vlSymsp = static_cast<Vflexpipe_top__Syms*>(userp);
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlSymsp->__Vm_activity = false;
        vlTOPp->__Vm_traceActivity[0U] = 0U;
        vlTOPp->__Vm_traceActivity[1U] = 0U;
        vlTOPp->__Vm_traceActivity[2U] = 0U;
    }
}
