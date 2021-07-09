#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "program.h"

void _zsvmWriteProgramByte(ZSVMprogram* program, uint8_t byte) {
	if (program->count == program->capacity) {
		program->capacity = _ZSVM_GROW_CAPACITY(program->capacity);
		program->data = _ZSVM_GROW_ARRAY(uint8_t, program->data, program->capacity);
	}

	program->data[program->count] = byte;
	program->count++;
}

size_t _zsvmWriteProgramConstant(ZSVMprogram* program, _ZSVMvalue constant) {
	_zsvmWriteValueArray(&program->constants, constant);
	return program->constants.count - 1;
}

ZSVMprogram* zsvmCreateProgram() {
	ZSVMprogram* program = malloc(sizeof(*program));

	if (!program) {
		printf("Failed to allocate memory for program");
		exit(1);
	}

	program->capacity = 0;
	program->count = 0;
	program->data = NULL;
	_zsvmInitValueArray(&program->constants);

	return program;
}

void zsvmDeleteProgram(ZSVMprogram* program) {
	_ZSVM_FREE_ARRAY(program->data);
	_zsvmFreeValueArray(&program->constants);
	free(program);
}