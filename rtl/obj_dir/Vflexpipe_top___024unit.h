// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vflexpipe_top.h for the primary calling header

#ifndef _VFLEXPIPE_TOP___024UNIT_H_
#define _VFLEXPIPE_TOP___024UNIT_H_  // guard

#include "verilated.h"

//==========

class Vflexpipe_top__Syms;
class Vflexpipe_top_VerilatedVcd;


//----------

VL_MODULE(Vflexpipe_top___024unit) {
  public:
    
    // INTERNAL VARIABLES
  private:
    Vflexpipe_top__Syms* __VlSymsp;  // Symbol table
  public:
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vflexpipe_top___024unit);  ///< Copying not allowed
  public:
    Vflexpipe_top___024unit(const char* name = "TOP");
    ~Vflexpipe_top___024unit();
    
    // INTERNAL METHODS
    void __Vconfigure(Vflexpipe_top__Syms* symsp, bool first);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
    static void traceInit(void* userp, VerilatedVcd* tracep, uint32_t code) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
