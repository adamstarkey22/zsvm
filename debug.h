#ifndef zsvm_debug_h
#define zsvm_debug_h

#include "zsvm.h"

void _zsvmDisassembleProgram(ZSVMprogram* program, const char* label);
size_t _zsvmDisassembleInstruction(ZSVMprogram* program, size_t offset);

#endif