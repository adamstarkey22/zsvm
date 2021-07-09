#ifndef zsvm_program_h
#define zsvm_program_h

#include <stdint.h>

#include "value.h"
#include "zsvm.h"

typedef enum {
	_ZSVM_OP_RETURN,
	_ZSVM_OP_ADD,
	_ZSVM_OP_SUBTRACT,
	_ZSVM_OP_MULTIPLY,
	_ZSVM_OP_DIVIDE,
	_ZSVM_OP_NEGATE,
	_ZSVM_OP_CONSTANT,
} _ZSVMopcode;

void _zsvmWriteProgramByte(ZSVMprogram* program, uint8_t byte);
size_t _zsvmWriteProgramConstant(ZSVMprogram* program, _ZSVMvalue constant);

struct ZSVMprogram {
	size_t capacity;
	size_t count;
	uint8_t* data;

	_ZSVMvaluearray constants;
};

#endif