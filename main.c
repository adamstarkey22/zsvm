#include <stdio.h>

#include "zsvm.h"
#include "compiler.h"
#include "debug.h"
#include "program.h"

static void interpret(ZSVMvirtualmachine* vm, char* source) {
	ZSVMprogram* program = zsvmCreateProgram();

	if (zsvmCompileProgram(program, source) != ZSVM_OK) {
		zsvmDeleteProgram(program);
		return;
	}

	zsvmRunProgram(vm, program);
	zsvmDeleteProgram(program);
}

static void repl(ZSVMvirtualmachine* vm) {
	char line[1024];
	for (;;) {
		printf("> ");

		if (!fgets(line, sizeof(line), stdin)) {
			printf("\n");
			break;
		}

		interpret(vm, line);
	}
}

int main(int argc, const char* argv[]) {
	ZSVMvirtualmachine* vm = zsvmCreateVirtualMachine();
	repl(vm);
	zsvmDeleteVirtualMachine(vm);
	return 0;
}