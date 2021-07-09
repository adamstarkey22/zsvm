#ifndef zsvm_zsvm_h
#define zsvm_zsvm_h

typedef struct ZSVMvirtualmachine ZSVMvirtualmachine;
typedef struct ZSVMprogram ZSVMprogram;

typedef enum {
	ZSVM_OK,
	ZSVM_COMPILE_ERROR,
	ZSVM_RUNTIME_ERROR
} ZSVMresult;

ZSVMvirtualmachine* zsvmCreateVirtualMachine();
void zsvmDeleteVirtualMachine(ZSVMvirtualmachine* vm);
ZSVMresult zsvmRunProgram(ZSVMvirtualmachine* vm, ZSVMprogram* program);

ZSVMprogram* zsvmCreateProgram();
void zsvmDeleteProgram(ZSVMprogram* program);

ZSVMresult zsvmCompileProgram(ZSVMprogram* program, const char* source);

#endif