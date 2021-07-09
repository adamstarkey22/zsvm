#include <stdio.h>

#include "zsvm.h"
#include "compiler.h"
#include "debug.h"
#include "program.h"

int main(int argc, const char* argv[]) {
	ZSVMvirtualmachine* vm = zsvmCreateVirtualMachine();
	ZSVMprogram* program = zsvmCreateProgram();

	uint8_t a = (uint8_t)_zsvmWriteProgramConstant(program, 3);
	uint8_t b = (uint8_t)_zsvmWriteProgramConstant(program, 4);
	uint8_t c = (uint8_t)_zsvmWriteProgramConstant(program, 5);

	_zsvmWriteProgramByte(program, _OP_CONSTANT);
	_zsvmWriteProgramByte(program, a);

	_zsvmWriteProgramByte(program, _OP_CONSTANT);
	_zsvmWriteProgramByte(program, b);
	
	_zsvmWriteProgramByte(program, _OP_CONSTANT);
	_zsvmWriteProgramByte(program, c);

	_zsvmWriteProgramByte(program, _OP_MULTIPLY);
	_zsvmWriteProgramByte(program, _OP_ADD);

	_zsvmWriteProgramByte(program, _OP_RETURN);

	_zsvmCompileProgram(program, "{}.#this is a \ncomment");
	_zsvmDisassembleProgram(program, "TEST PROGRAM");

	ZSVMresult result = zsvmRunProgram(vm, program);
	switch (result) {
		case ZSVM_OK:            printf("VM returned ZSVM_OK\n"); break;
		case ZSVM_COMPILE_ERROR: printf("VM returned ZSVM_COMPILE_ERROR\n"); break;
		case ZSVM_RUNTIME_ERROR: printf("VM returned ZSVM_RUNTIME_ERROR\n"); break;
		default: break;
	}

	zsvmDeleteVirtualMachine(vm);
	zsvmDeleteProgram(program);
	return 0;
}