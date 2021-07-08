#ifndef zsvm_zsvm_h
#define zsvm_zsvm_h

typedef struct ZSVMvirtualmachine ZSVMvirtualmachine;
typedef struct ZSVMprogram        ZSVMprogram;

ZSVMvirtualmachine* zsvmCreateVirtualMachine();
void zsvmDeleteVirtualMachine(ZSVMvirtualmachine* vm);

ZSVMprogram* zsvmCreateProgram();
void zsvmDeleteProgram();

#endif