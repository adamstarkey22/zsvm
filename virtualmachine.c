#include <stdarg.h>
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

static void runtimeError(ZSVMvirtualmachine* vm, const char* format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fputs("\n", stderr);

	size_t instruction = vm->IP - vm->program->data - 1;
	int line = -1;
	fprintf(stderr, "[line %d] in script\n", line);
}

static void push(ZSVMvirtualmachine* vm, _ZSVMvalue value) {
	*vm->SP = value;
	vm->SP++;
}

static _ZSVMvalue pop(ZSVMvirtualmachine* vm) {
	vm->SP--;
	return *vm->SP;
}

static _ZSVMvalue peek(ZSVMvirtualmachine* vm, int distance) {
	return vm->SP[-1 - distance];
}

static bool isFalsey(_ZSVMvalue value) {
	return _ZSVM_IS_NULL(value) || (_ZSVM_IS_BOOL(value) && !_ZSVM_AS_BOOL(value));
}

static ZSVMresult run(ZSVMvirtualmachine* vm) {
#define READ_BYTE() (*vm->IP++)
#define READ_CONSTANT() (vm->program->constants.data[READ_BYTE()])
#define BINARY_OP(valueType, op) \
	do { \
		if (!_ZSVM_IS_NUMBER(peek(vm, 0)) || !_ZSVM_IS_NUMBER(peek(vm, 1))) { \
			runtimeError(vm, "Operands must be numbers."); \
			return ZSVM_RUNTIME_ERROR; \
		} \
		double b = _ZSVM_AS_NUMBER(pop(vm)); \
		double a = _ZSVM_AS_NUMBER(pop(vm)); \
		push(vm, valueType(a op b)); \
	} while (0)

	for (;;) {
		//_zsvmDisassembleInstruction(vm->program, (int)(vm->IP - vm->program->data));
		uint8_t instruction = READ_BYTE();
		switch (instruction) {
			case _OP_RETURN:
				_zsvmPrintValue(pop(vm));
				printf("\n");
				return ZSVM_OK;

			case _OP_CONSTANT: {
				_ZSVMvalue constant = READ_CONSTANT();
				push(vm, constant);
				break;
			}
			case _OP_NULL:     push(vm, _ZSVM_NULL_VAL); break;
			case _OP_TRUE:     push(vm, _ZSVM_BOOL_VAL(true)); break;
			case _OP_FALSE:    push(vm, _ZSVM_BOOL_VAL(false)); break;

			case _OP_ADD:      BINARY_OP(_ZSVM_NUMBER_VAL, +); break;
			case _OP_SUBTRACT: BINARY_OP(_ZSVM_NUMBER_VAL, -); break; 
			case _OP_MULTIPLY: BINARY_OP(_ZSVM_NUMBER_VAL, *); break; 
			case _OP_DIVIDE:   BINARY_OP(_ZSVM_NUMBER_VAL, /); break;

			case _OP_EQUAL: {
				_ZSVMvalue b = pop(vm);
				_ZSVMvalue a = pop(vm);
				push(vm, _ZSVM_BOOL_VAL(_zsvmValuesEqual(a, b)));
				break;
			}
			case _OP_NOT:
				push(vm, _ZSVM_BOOL_VAL(isFalsey(pop(vm))));
				break;
			case _OP_NEGATE:
				if (!_ZSVM_IS_NUMBER(peek(vm, 0))) {
					runtimeError(vm, "Operand must be a number.");
					return ZSVM_RUNTIME_ERROR;
				}
				push(vm, _ZSVM_NUMBER_VAL(-_ZSVM_AS_NUMBER(pop(vm))));
				break;
			case _OP_GREATER: BINARY_OP(_ZSVM_BOOL_VAL, >); break;
			case _OP_LESS: BINARY_OP(_ZSVM_BOOL_VAL, >); break;
		}
		//_zsvmPrintStack(vm);
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