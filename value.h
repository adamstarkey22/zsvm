#ifndef zsvm_value_h
#define zsvm_value_h

#include <stdbool.h>

typedef enum {
	_VALUE_BOOL,
	_VALUE_NULL,
	_VALUE_NUMBER,
} _ZSVMvaluetype;

typedef struct {
	_ZSVMvaluetype type;

	union {
		bool boolean;
		double number;
	} as;

} _ZSVMvalue;

#define _ZSVM_IS_BOOL(value)    ((value).type == _VALUE_BOOL)
#define _ZSVM_IS_NULL(value)    ((value).type == _VALUE_NULL)
#define _ZSVM_IS_NUMBER(value)  ((value).type == _VALUE_NUMBER)

#define _ZSVM_AS_BOOL(value)    ((value).as.boolean)
#define _ZSVM_AS_NUMBER(value)  ((value).as.number)

#define _ZSVM_BOOL_VAL(value)   ((_ZSVMvalue){_VALUE_BOOL, {.boolean = value}})
#define _ZSVM_NULL_VAL          ((_ZSVMvalue){_VALUE_NULL, {.number = 0}})
#define _ZSVM_NUMBER_VAL(value) ((_ZSVMvalue){_VALUE_NUMBER, {.number = value}})

typedef struct {
	size_t capacity;
	size_t count;
	_ZSVMvalue* data;
} _ZSVMvaluearray;

void _zsvmInitValueArray(_ZSVMvaluearray* array);
void _zsvmFreeValueArray(_ZSVMvaluearray* array);
void _zsvmWriteValueArray(_ZSVMvaluearray* array, _ZSVMvalue value);
void _zsvmPrintValue(_ZSVMvalue value);
bool _zsvmValuesEqual(_ZSVMvalue a, _ZSVMvalue b);

#endif