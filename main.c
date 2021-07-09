#include <stdio.h>

#include "zsvm.h"
#include "compiler.h"
#include "debug.h"
#include "program.h"

int main(int argc, const char* argv[]) {
	ZSVMvirtualmachine* vm = zsvmCreateVirtualMachine();
	ZSVMprogram* program = zsvmCreateProgram();

	ZSVMresult result;
	
	result = _zsvmCompileProgram(program, "(5+4)*3");
	switch (result) {
	case ZSVM_OK:            printf("Compiler returned ZSVM_OK\n"); break;
	case ZSVM_COMPILE_ERROR: printf("Compiler returned ZSVM_COMPILE_ERROR\n"); break;
	case ZSVM_RUNTIME_ERROR: printf("Compiler returned ZSVM_RUNTIME_ERROR\n"); break;
	default: break;
	}

	_zsvmDisassembleProgram(program, "TEST PROGRAM");

	result = zsvmRunProgram(vm, program);
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