#ifndef zsvm_virtualmachine_h
#define zsvm_virtualmachine_h

#include "zsvm.h"

#define _ZSVM_STACK_LIMIT 64

struct ZSVMvirtualmachine {
	ZSVMprogram* program;
	uint8_t* IP;
	_ZSVMvalue* SP;
	_ZSVMvalue stack[_ZSVM_STACK_LIMIT];
};

#endif