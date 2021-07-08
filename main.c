#include "zsvm.h"

int main(int argc, const char* argv[]) {
	ZSVMvirtualmachine* vm = zsvmCreateVirtualMachine();
	ZSVMprogram* program = zsvmCreateProgram();

	zsvmDeleteVirtualMachine(vm);
	zsvmDeleteProgram(program);
	return 0;
}