#ifndef zsvm_program_h
#define zsvm_program_h

#include <stdint.h>

#include "value.h"
#include "zsvm.h"

typedef enum {
	_OP_RETURN,
	_OP_ADD,
	_OP_SUBTRACT,
	_OP_MULTIPLY,
	_OP_DIVIDE,
	_OP_NEGATE,
	_OP_CONSTANT,
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