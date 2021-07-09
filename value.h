#ifndef zsvm_value_h
#define zsvm_value_h

typedef double _ZSVMvalue;

typedef struct {
	size_t capacity;
	size_t count;
	_ZSVMvalue* data;
} _ZSVMvaluearray;

void _zsvmInitValueArray(_ZSVMvaluearray* array);
void _zsvmFreeValueArray(_ZSVMvaluearray* array);
void _zsvmWriteValueArray(_ZSVMvaluearray* array, _ZSVMvalue value);
void _zsvmPrintValue(_ZSVMvalue value);

#endif