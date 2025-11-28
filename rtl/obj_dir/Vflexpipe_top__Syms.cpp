// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vflexpipe_top__Syms.h"
#include "Vflexpipe_top.h"
#include "Vflexpipe_top___024unit.h"



// FUNCTIONS
Vflexpipe_top__Syms::Vflexpipe_top__Syms(Vflexpipe_top* topp, const char* namep)
    // Setup locals
    : __Vm_namep(namep)
    , __Vm_activity(false)
    , __Vm_baseCode(0)
    , __Vm_didInit(false)
    // Setup submodule names
{
    // Pointer to top level
    TOPp = topp;
    // Setup each module's pointers to their submodules
    // Setup each module's pointer back to symbol table (for public functions)
    TOPp->__Vconfigure(this, true);
}
