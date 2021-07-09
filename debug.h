#ifndef zsvm_debug_h
#define zsvm_debug_h

#include "scanner.h"
#include "zsvm.h"

const char* const _ZSVMtokenstrings[];

void _zsvmPrintStack(ZSVMvirtualmachine* vm);
void _zsvmDisassembleProgram(ZSVMprogram* program, const char* label);
size_t _zsvmDisassembleInstruction(ZSVMprogram* program, size_t offset);

#endif