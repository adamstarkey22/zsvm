#include <stdio.h>

#include "debug.h"
#include "program.h"

static size_t simpleInstruction(const char* name, size_t offset) {
	printf("%s\n", name);
	return offset + 1;
}

static size_t constantInstruction(const char* name, ZSVMprogram* program, size_t offset) {
	uint8_t location = program->data[offset + 1];
	printf("%s %3d -> \"", name, location);
	_zsvmPrintValue(program->constants.data[location]);
	printf("\"\n");
	return offset + 2;
}

void _zsvmDisassembleProgram(ZSVMprogram* program, const char* label) {
	printf("========== %s ==========\n", label);
	for (size_t offset = 0; offset < program->count;) {
		offset = _zsvmDisassembleInstruction(program, offset);
	}
	printf("\n");
}

size_t _zsvmDisassembleInstruction(ZSVMprogram* program, size_t offset) {
	printf("%4d ", (int)offset);

	uint8_t instruction = program->data[offset];
	switch (instruction) {
		case _ZSVM_OP_RETURN:   return simpleInstruction("RETURN", offset);
		case _ZSVM_OP_ADD:      return simpleInstruction("ADD", offset);
		case _ZSVM_OP_SUBTRACT: return simpleInstruction("SUBTRACT", offset);
		case _ZSVM_OP_MULTIPLY: return simpleInstruction("MULTIPLY", offset);
		case _ZSVM_OP_DIVIDE:   return simpleInstruction("DIVIDE", offset);
		case _ZSVM_OP_NEGATE:   return simpleInstruction("NEGATE", offset);

		case _ZSVM_OP_CONSTANT: return constantInstruction("CONSTANT", program, offset);

		default:
			printf("Invalid opcode: %d\n", instruction);
			return offset + 1;
	}
}