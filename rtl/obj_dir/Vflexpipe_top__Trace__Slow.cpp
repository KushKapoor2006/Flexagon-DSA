// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vflexpipe_top__Syms.h"


//======================

void Vflexpipe_top::trace(VerilatedVcdC* tfp, int, int) {
    tfp->spTrace()->addInitCb(&traceInit, __VlSymsp);
    traceRegister(tfp->spTrace());
}

void Vflexpipe_top::traceInit(void* userp, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vflexpipe_top__Syms* __restrict vlSymsp = static_cast<Vflexpipe_top__Syms*>(userp);
    if (!Verilated::calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
                        "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->module(vlSymsp->name());
    tracep->scopeEscape(' ');
    Vflexpipe_top::traceInitTop(vlSymsp, tracep);
    tracep->scopeEscape('.');
}

//======================


void Vflexpipe_top::traceInitTop(void* userp, VerilatedVcd* tracep) {
    Vflexpipe_top__Syms* __restrict vlSymsp = static_cast<Vflexpipe_top__Syms*>(userp);
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlTOPp->traceInitSub0(userp, tracep);
    }
}

void Vflexpipe_top::traceInitSub0(void* userp, VerilatedVcd* tracep) {
    Vflexpipe_top__Syms* __restrict vlSymsp = static_cast<Vflexpipe_top__Syms*>(userp);
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    const int c = vlSymsp->__Vm_baseCode;
    if (false && tracep && c) {}  // Prevent unused
    // Body
    {
        tracep->declBit(c+47,"clk", false,-1);
        tracep->declBit(c+48,"rst_n", false,-1);
        tracep->declBit(c+49,"dbg_mode", false,-1);
        tracep->declBus(c+50,"dbg_cycle_count", false,-1, 31,0);
        tracep->declQuad(c+51,"s_axi_awaddr", false,-1, 63,0);
        tracep->declBit(c+53,"s_axi_awvalid", false,-1);
        tracep->declBit(c+54,"s_axi_awready", false,-1);
        tracep->declBus(c+55,"s_axi_wdata", false,-1, 31,0);
        tracep->declBus(c+56,"s_axi_wstrb", false,-1, 3,0);
        tracep->declBit(c+57,"s_axi_wvalid", false,-1);
        tracep->declBit(c+58,"s_axi_wready", false,-1);
        tracep->declBus(c+59,"s_axi_bresp", false,-1, 1,0);
        tracep->declBit(c+60,"s_axi_bvalid", false,-1);
        tracep->declBit(c+61,"s_axi_bready", false,-1);
        tracep->declBit(c+62,"core_idle", false,-1);
        tracep->declBit(c+63,"core_pipeline_drained", false,-1);
        tracep->declBit(c+64,"flip_ack", false,-1);
        tracep->declBit(c+65,"request_flip", false,-1);
        tracep->declArray(c+66,"active_config", false,-1, 196,0);
        tracep->declBit(c+73,"layer_start_pulse", false,-1);
        tracep->declArray(c+74,"core_mem_req", false,-1, 108,0);
        tracep->declBit(c+78,"core_mem_req_valid", false,-1);
        tracep->declBit(c+79,"core_mem_req_ready", false,-1);
        tracep->declArray(c+80,"core_mem_resp", false,-1, 267,0);
        tracep->declBit(c+89,"core_mem_resp_valid", false,-1);
        tracep->declArray(c+90,"mem_master_req", false,-1, 108,0);
        tracep->declBit(c+94,"mem_master_valid", false,-1);
        tracep->declBit(c+95,"mem_master_ready", false,-1);
        tracep->declArray(c+96,"mem_master_resp", false,-1, 267,0);
        tracep->declBit(c+105,"mem_master_resp_valid", false,-1);
        tracep->declBit(c+47,"flexpipe_top clk", false,-1);
        tracep->declBit(c+48,"flexpipe_top rst_n", false,-1);
        tracep->declBit(c+49,"flexpipe_top dbg_mode", false,-1);
        tracep->declBus(c+50,"flexpipe_top dbg_cycle_count", false,-1, 31,0);
        tracep->declQuad(c+51,"flexpipe_top s_axi_awaddr", false,-1, 63,0);
        tracep->declBit(c+53,"flexpipe_top s_axi_awvalid", false,-1);
        tracep->declBit(c+54,"flexpipe_top s_axi_awready", false,-1);
        tracep->declBus(c+55,"flexpipe_top s_axi_wdata", false,-1, 31,0);
        tracep->declBus(c+56,"flexpipe_top s_axi_wstrb", false,-1, 3,0);
        tracep->declBit(c+57,"flexpipe_top s_axi_wvalid", false,-1);
        tracep->declBit(c+58,"flexpipe_top s_axi_wready", false,-1);
        tracep->declBus(c+59,"flexpipe_top s_axi_bresp", false,-1, 1,0);
        tracep->declBit(c+60,"flexpipe_top s_axi_bvalid", false,-1);
        tracep->declBit(c+61,"flexpipe_top s_axi_bready", false,-1);
        tracep->declBit(c+62,"flexpipe_top core_idle", false,-1);
        tracep->declBit(c+63,"flexpipe_top core_pipeline_drained", false,-1);
        tracep->declBit(c+64,"flexpipe_top flip_ack", false,-1);
        tracep->declBit(c+65,"flexpipe_top request_flip", false,-1);
        tracep->declArray(c+66,"flexpipe_top active_config", false,-1, 196,0);
        tracep->declBit(c+73,"flexpipe_top layer_start_pulse", false,-1);
        tracep->declArray(c+74,"flexpipe_top core_mem_req", false,-1, 108,0);
        tracep->declBit(c+78,"flexpipe_top core_mem_req_valid", false,-1);
        tracep->declBit(c+79,"flexpipe_top core_mem_req_ready", false,-1);
        tracep->declArray(c+80,"flexpipe_top core_mem_resp", false,-1, 267,0);
        tracep->declBit(c+89,"flexpipe_top core_mem_resp_valid", false,-1);
        tracep->declArray(c+90,"flexpipe_top mem_master_req", false,-1, 108,0);
        tracep->declBit(c+94,"flexpipe_top mem_master_valid", false,-1);
        tracep->declBit(c+95,"flexpipe_top mem_master_ready", false,-1);
        tracep->declArray(c+96,"flexpipe_top mem_master_resp", false,-1, 267,0);
        tracep->declBit(c+105,"flexpipe_top mem_master_resp_valid", false,-1);
        tracep->declArray(c+9,"flexpipe_top shadow_config", false,-1, 196,0);
        tracep->declBus(c+16,"flexpipe_top current_epoch", false,-1, 3,0);
        tracep->declBit(c+17,"flexpipe_top prefetch_busy", false,-1);
        tracep->declBus(c+18,"flexpipe_top bytes_fetched_w", false,-1, 31,0);
        tracep->declArray(c+19,"flexpipe_top prefetch_req", false,-1, 108,0);
        tracep->declBit(c+23,"flexpipe_top prefetch_req_valid", false,-1);
        tracep->declBit(c+1,"flexpipe_top prefetch_req_ready", false,-1);
        tracep->declArray(c+96,"flexpipe_top prefetch_resp", false,-1, 267,0);
        tracep->declBit(c+2,"flexpipe_top prefetch_resp_valid", false,-1);
        tracep->declArray(c+3,"flexpipe_top arb_to_dram_req", false,-1, 108,0);
        tracep->declBit(c+7,"flexpipe_top arb_to_dram_valid", false,-1);
        tracep->declBit(c+24,"flexpipe_top no_outstanding_active", false,-1);
        tracep->declBus(c+25,"flexpipe_top dbg_cycle_count_r", false,-1, 31,0);
        tracep->declBit(c+26,"flexpipe_top prefetch_start", false,-1);
        tracep->declBit(c+47,"flexpipe_top u_config_manager clk", false,-1);
        tracep->declBit(c+48,"flexpipe_top u_config_manager rst_n", false,-1);
        tracep->declQuad(c+51,"flexpipe_top u_config_manager s_axi_awaddr", false,-1, 63,0);
        tracep->declBit(c+53,"flexpipe_top u_config_manager s_axi_awvalid", false,-1);
        tracep->declBit(c+54,"flexpipe_top u_config_manager s_axi_awready", false,-1);
        tracep->declBus(c+55,"flexpipe_top u_config_manager s_axi_wdata", false,-1, 31,0);
        tracep->declBus(c+56,"flexpipe_top u_config_manager s_axi_wstrb", false,-1, 3,0);
        tracep->declBit(c+57,"flexpipe_top u_config_manager s_axi_wvalid", false,-1);
        tracep->declBit(c+58,"flexpipe_top u_config_manager s_axi_wready", false,-1);
        tracep->declBus(c+59,"flexpipe_top u_config_manager s_axi_bresp", false,-1, 1,0);
        tracep->declBit(c+60,"flexpipe_top u_config_manager s_axi_bvalid", false,-1);
        tracep->declBit(c+61,"flexpipe_top u_config_manager s_axi_bready", false,-1);
        tracep->declBit(c+106,"flexpipe_top u_config_manager core_safe_to_flip", false,-1);
        tracep->declBit(c+24,"flexpipe_top u_config_manager no_outstanding_active", false,-1);
        tracep->declBit(c+65,"flexpipe_top u_config_manager request_flip", false,-1);
        tracep->declBit(c+64,"flexpipe_top u_config_manager flip_ack", false,-1);
        tracep->declArray(c+66,"flexpipe_top u_config_manager active_config", false,-1, 196,0);
        tracep->declArray(c+9,"flexpipe_top u_config_manager shadow_config", false,-1, 196,0);
        tracep->declBit(c+73,"flexpipe_top u_config_manager layer_start_pulse", false,-1);
        tracep->declBus(c+16,"flexpipe_top u_config_manager current_epoch", false,-1, 3,0);
        tracep->declArray(c+27,"flexpipe_top u_config_manager active_config_r", false,-1, 196,0);
        tracep->declArray(c+9,"flexpipe_top u_config_manager shadow_config_r", false,-1, 196,0);
        tracep->declBit(c+34,"flexpipe_top u_config_manager shadow_full", false,-1);
        tracep->declBus(c+16,"flexpipe_top u_config_manager epoch_counter", false,-1, 3,0);
        tracep->declBit(c+35,"flexpipe_top u_config_manager request_flip_r", false,-1);
        tracep->declBus(c+36,"flexpipe_top u_config_manager write_idx", false,-1, 2,0);
        tracep->declBus(c+37,"flexpipe_top u_config_manager axi_state", false,-1, 1,0);
        tracep->declBit(c+47,"flexpipe_top u_prefetch_dma clk", false,-1);
        tracep->declBit(c+48,"flexpipe_top u_prefetch_dma rst_n", false,-1);
        tracep->declArray(c+9,"flexpipe_top u_prefetch_dma shadow_config", false,-1, 196,0);
        tracep->declBus(c+16,"flexpipe_top u_prefetch_dma current_epoch", false,-1, 3,0);
        tracep->declBit(c+26,"flexpipe_top u_prefetch_dma start_prefetch", false,-1);
        tracep->declBit(c+17,"flexpipe_top u_prefetch_dma busy", false,-1);
        tracep->declBus(c+18,"flexpipe_top u_prefetch_dma bytes_fetched", false,-1, 31,0);
        tracep->declArray(c+19,"flexpipe_top u_prefetch_dma mem_req", false,-1, 108,0);
        tracep->declBit(c+23,"flexpipe_top u_prefetch_dma mem_req_valid", false,-1);
        tracep->declBit(c+1,"flexpipe_top u_prefetch_dma mem_req_ready", false,-1);
        tracep->declArray(c+96,"flexpipe_top u_prefetch_dma mem_resp", false,-1, 267,0);
        tracep->declBit(c+2,"flexpipe_top u_prefetch_dma mem_resp_valid", false,-1);
        tracep->declBus(c+38,"flexpipe_top u_prefetch_dma state", false,-1, 2,0);
        tracep->declBus(c+8,"flexpipe_top u_prefetch_dma next_state", false,-1, 2,0);
        tracep->declQuad(c+39,"flexpipe_top u_prefetch_dma current_ptr_addr", false,-1, 63,0);
        tracep->declQuad(c+41,"flexpipe_top u_prefetch_dma fetched_data_addr", false,-1, 63,0);
        tracep->declBus(c+43,"flexpipe_top u_prefetch_dma walks_remaining", false,-1, 31,0);
        tracep->declBus(c+44,"flexpipe_top u_prefetch_dma my_epoch", false,-1, 3,0);
        tracep->declBus(c+18,"flexpipe_top u_prefetch_dma bytes_accum", false,-1, 31,0);
        tracep->declBit(c+47,"flexpipe_top u_memory_arbiter clk", false,-1);
        tracep->declBit(c+48,"flexpipe_top u_memory_arbiter rst_n", false,-1);
        tracep->declArray(c+74,"flexpipe_top u_memory_arbiter active_req", false,-1, 108,0);
        tracep->declBit(c+78,"flexpipe_top u_memory_arbiter active_req_valid", false,-1);
        tracep->declBit(c+79,"flexpipe_top u_memory_arbiter active_req_ready", false,-1);
        tracep->declArray(c+80,"flexpipe_top u_memory_arbiter active_resp", false,-1, 267,0);
        tracep->declBit(c+89,"flexpipe_top u_memory_arbiter active_resp_valid", false,-1);
        tracep->declArray(c+19,"flexpipe_top u_memory_arbiter prefetch_req", false,-1, 108,0);
        tracep->declBit(c+23,"flexpipe_top u_memory_arbiter prefetch_req_valid", false,-1);
        tracep->declBit(c+1,"flexpipe_top u_memory_arbiter prefetch_req_ready", false,-1);
        tracep->declArray(c+96,"flexpipe_top u_memory_arbiter prefetch_resp", false,-1, 267,0);
        tracep->declBit(c+2,"flexpipe_top u_memory_arbiter prefetch_resp_valid", false,-1);
        tracep->declArray(c+3,"flexpipe_top u_memory_arbiter mem_master_req", false,-1, 108,0);
        tracep->declBit(c+7,"flexpipe_top u_memory_arbiter mem_master_valid", false,-1);
        tracep->declBit(c+95,"flexpipe_top u_memory_arbiter mem_master_ready", false,-1);
        tracep->declArray(c+96,"flexpipe_top u_memory_arbiter mem_master_resp", false,-1, 267,0);
        tracep->declBit(c+105,"flexpipe_top u_memory_arbiter mem_master_resp_valid", false,-1);
        tracep->declBit(c+24,"flexpipe_top u_memory_arbiter no_outstanding_active", false,-1);
        tracep->declBus(c+45,"flexpipe_top u_memory_arbiter active_cnt", false,-1, 7,0);
        tracep->declBus(c+46,"flexpipe_top u_memory_arbiter prefetch_cnt", false,-1, 7,0);
        tracep->declBus(c+107,"flexpipe_top u_memory_arbiter ID_SRC_BIT", false,-1, 31,0);
        tracep->declBus(c+108,"flexpipe_pkg CONFIG_BUS_WIDTH_BYTES", false,-1, 31,0);
        tracep->declBus(c+109,"flexpipe_pkg CONFIG_BUS_WIDTH_BITS", false,-1, 31,0);
        tracep->declBus(c+110,"flexpipe_pkg ADDR_WIDTH", false,-1, 31,0);
        tracep->declBus(c+111,"flexpipe_pkg DATA_WIDTH", false,-1, 31,0);
        tracep->declBus(c+112,"flexpipe_pkg MRN_DRAIN_CYCLES", false,-1, 31,0);
        tracep->declBus(c+113,"flexpipe_pkg MAX_OUTSTANDING_REQS", false,-1, 31,0);
        tracep->declBus(c+114,"flexpipe_pkg REQ_ID_WIDTH", false,-1, 31,0);
        tracep->declBus(c+108,"flexpipe_pkg EPOCH_WIDTH", false,-1, 31,0);
    }
}

void Vflexpipe_top::traceRegister(VerilatedVcd* tracep) {
    // Body
    {
        tracep->addFullCb(&traceFullTop0, __VlSymsp);
        tracep->addChgCb(&traceChgTop0, __VlSymsp);
        tracep->addCleanupCb(&traceCleanup, __VlSymsp);
    }
}

void Vflexpipe_top::traceFullTop0(void* userp, VerilatedVcd* tracep) {
    Vflexpipe_top__Syms* __restrict vlSymsp = static_cast<Vflexpipe_top__Syms*>(userp);
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlTOPp->traceFullSub0(userp, tracep);
    }
}

void Vflexpipe_top::traceFullSub0(void* userp, VerilatedVcd* tracep) {
    Vflexpipe_top__Syms* __restrict vlSymsp = static_cast<Vflexpipe_top__Syms*>(userp);
    Vflexpipe_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    vluint32_t* const oldp = tracep->oldp(vlSymsp->__Vm_baseCode);
    if (false && oldp) {}  // Prevent unused
    // Body
    {
        tracep->fullBit(oldp+1,(vlTOPp->flexpipe_top__DOT__prefetch_req_ready));
        tracep->fullBit(oldp+2,(vlTOPp->flexpipe_top__DOT__prefetch_resp_valid));
        tracep->fullWData(oldp+3,(vlTOPp->flexpipe_top__DOT__arb_to_dram_req),109);
        tracep->fullBit(oldp+7,(vlTOPp->flexpipe_top__DOT__arb_to_dram_valid));
        tracep->fullCData(oldp+8,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__next_state),3);
        tracep->fullWData(oldp+9,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r),197);
        tracep->fullCData(oldp+16,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__epoch_counter),4);
        tracep->fullBit(oldp+17,(((0U != (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state)) 
                                  & (5U != (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state)))));
        tracep->fullIData(oldp+18,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__bytes_accum),32);
        tracep->fullWData(oldp+19,(vlTOPp->flexpipe_top__DOT__prefetch_req),109);
        tracep->fullBit(oldp+23,(vlTOPp->flexpipe_top__DOT__prefetch_req_valid));
        tracep->fullBit(oldp+24,((0U == (IData)(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt))));
        tracep->fullIData(oldp+25,(vlTOPp->flexpipe_top__DOT__dbg_cycle_count_r),32);
        tracep->fullBit(oldp+26,((1U & ((vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_config_r[0U] 
                                         >> 4U) & (~ 
                                                   ((0U 
                                                     != (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state)) 
                                                    & (5U 
                                                       != (IData)(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state))))))));
        tracep->fullWData(oldp+27,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__active_config_r),197);
        tracep->fullBit(oldp+34,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__shadow_full));
        tracep->fullBit(oldp+35,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__request_flip_r));
        tracep->fullCData(oldp+36,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__write_idx),3);
        tracep->fullCData(oldp+37,(vlTOPp->flexpipe_top__DOT__u_config_manager__DOT__axi_state),2);
        tracep->fullCData(oldp+38,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__state),3);
        tracep->fullQData(oldp+39,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__current_ptr_addr),64);
        tracep->fullQData(oldp+41,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__fetched_data_addr),64);
        tracep->fullIData(oldp+43,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__walks_remaining),32);
        tracep->fullCData(oldp+44,(vlTOPp->flexpipe_top__DOT__u_prefetch_dma__DOT__my_epoch),4);
        tracep->fullCData(oldp+45,(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__active_cnt),8);
        tracep->fullCData(oldp+46,(vlTOPp->flexpipe_top__DOT__u_memory_arbiter__DOT__prefetch_cnt),8);
        tracep->fullBit(oldp+47,(vlTOPp->clk));
        tracep->fullBit(oldp+48,(vlTOPp->rst_n));
        tracep->fullBit(oldp+49,(vlTOPp->dbg_mode));
        tracep->fullIData(oldp+50,(vlTOPp->dbg_cycle_count),32);
        tracep->fullQData(oldp+51,(vlTOPp->s_axi_awaddr),64);
        tracep->fullBit(oldp+53,(vlTOPp->s_axi_awvalid));
        tracep->fullBit(oldp+54,(vlTOPp->s_axi_awready));
        tracep->fullIData(oldp+55,(vlTOPp->s_axi_wdata),32);
        tracep->fullCData(oldp+56,(vlTOPp->s_axi_wstrb),4);
        tracep->fullBit(oldp+57,(vlTOPp->s_axi_wvalid));
        tracep->fullBit(oldp+58,(vlTOPp->s_axi_wready));
        tracep->fullCData(oldp+59,(vlTOPp->s_axi_bresp),2);
        tracep->fullBit(oldp+60,(vlTOPp->s_axi_bvalid));
        tracep->fullBit(oldp+61,(vlTOPp->s_axi_bready));
        tracep->fullBit(oldp+62,(vlTOPp->core_idle));
        tracep->fullBit(oldp+63,(vlTOPp->core_pipeline_drained));
        tracep->fullBit(oldp+64,(vlTOPp->flip_ack));
        tracep->fullBit(oldp+65,(vlTOPp->request_flip));
        tracep->fullWData(oldp+66,(vlTOPp->active_config),197);
        tracep->fullBit(oldp+73,(vlTOPp->layer_start_pulse));
        tracep->fullWData(oldp+74,(vlTOPp->core_mem_req),109);
        tracep->fullBit(oldp+78,(vlTOPp->core_mem_req_valid));
        tracep->fullBit(oldp+79,(vlTOPp->core_mem_req_ready));
        tracep->fullWData(oldp+80,(vlTOPp->core_mem_resp),268);
        tracep->fullBit(oldp+89,(vlTOPp->core_mem_resp_valid));
        tracep->fullWData(oldp+90,(vlTOPp->mem_master_req),109);
        tracep->fullBit(oldp+94,(vlTOPp->mem_master_valid));
        tracep->fullBit(oldp+95,(vlTOPp->mem_master_ready));
        tracep->fullWData(oldp+96,(vlTOPp->mem_master_resp),268);
        tracep->fullBit(oldp+105,(vlTOPp->mem_master_resp_valid));
        tracep->fullBit(oldp+106,(((IData)(vlTOPp->core_idle) 
                                   & (IData)(vlTOPp->core_pipeline_drained))));
        tracep->fullIData(oldp+107,(5U),32);
        tracep->fullIData(oldp+108,(4U),32);
        tracep->fullIData(oldp+109,(0x20U),32);
        tracep->fullIData(oldp+110,(0x40U),32);
        tracep->fullIData(oldp+111,(0x100U),32);
        tracep->fullIData(oldp+112,(0x32U),32);
        tracep->fullIData(oldp+113,(0x10U),32);
        tracep->fullIData(oldp+114,(6U),32);
    }
}
