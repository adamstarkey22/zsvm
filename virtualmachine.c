#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "program.h"
#include "virtualmachine.h"

static void reset(ZSVMvirtualmachine* vm) {
	vm->program = NULL;
	vm->IP = NULL;
	vm->SP = vm->stack;
}

static void push(ZSVMvirtualmachine* vm, double value) {
	*vm->SP = value;
	vm->SP++;
}

static _ZSVMvalue pop(ZSVMvirtualmachine* vm) {
	vm->SP--;
	return *vm->SP;
}

static ZSVMresult run(ZSVMvirtualmachine* vm) {
#define READ_BYTE() (*vm->IP++)
#define READ_CONSTANT() (vm->program->constants.data[READ_BYTE()])
#define BINARY_OP(op) \
	do { \
		_ZSVMvalue b = pop(vm); \
		_ZSVMvalue a = pop(vm); \
		push(vm, a op b); \
	} while (0)


	for (;;) {
		uint8_t instruction = READ_BYTE();
		switch (instruction) {
			case _OP_RETURN:
				_zsvmPrintValue(pop(vm));
				printf("\n");
				return ZSVM_OK;

			case _OP_ADD:      BINARY_OP(+); break;
			case _OP_SUBTRACT: BINARY_OP(-); break; 
			case _OP_MULTIPLY: BINARY_OP(*); break; 
			case _OP_DIVIDE:   BINARY_OP(/); break;
			case _OP_NEGATE:   push(vm, -pop(vm));  break;

			case _OP_CONSTANT: {
				_ZSVMvalue constant = READ_CONSTANT();
				push(vm, constant);
				break;
			}
		}
	}

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

ZSVMvirtualmachine* zsvmCreateVirtualMachine() {
	ZSVMvirtualmachine* vm = malloc(sizeof(*vm));

	if (!vm) {
		printf("Failed to allocate memory for virtual machine");
		exit(1);
	}

	reset(vm);
	return vm;
}

void zsvmDeleteVirtualMachine(ZSVMvirtualmachine* vm) {
	free(vm);
}

ZSVMresult zsvmRunProgram(ZSVMvirtualmachine* vm, ZSVMprogram* program) {
	reset(vm);
	vm->program = program;
	vm->IP = program->data;
	return run(vm);
}