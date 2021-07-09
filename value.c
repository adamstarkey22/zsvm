#include <stdio.h>

#include "memory.h"
#include "value.h"

void _zsvmInitValueArray(_ZSVMvaluearray* array) {
	array->capacity = 0;
	array->count = 0;
	array->data = NULL;
}

void _zsvmFreeValueArray(_ZSVMvaluearray* array) {
	_ZSVM_FREE_ARRAY(array->data);
}

void _zsvmWriteValueArray(_ZSVMvaluearray* array, _ZSVMvalue value) {
	if (array->count == array->capacity) {
		array->capacity = _ZSVM_GROW_CAPACITY(array->capacity);
		array->data = _ZSVM_GROW_ARRAY(_ZSVMvalue, array->data, array->capacity);
	}

	array->data[array->count] = value;
	array->count++;
}

void _zsvmPrintValue(_ZSVMvalue value) {
	switch (value.type) {
	case _VALUE_BOOL:
		printf(_ZSVM_AS_BOOL(value) ? "true" : "false");
		break;
	case _VALUE_NULL:
		printf("null");
		break;
	case _VALUE_NUMBER:
		printf("%g", _ZSVM_AS_NUMBER(value));
		break;
	}
}

bool _zsvmValuesEqual(_ZSVMvalue a, _ZSVMvalue b) {
	if (a.type != b.type) return false;
	switch (a.type) {
		case _VALUE_BOOL:   return _ZSVM_AS_BOOL(a) == _ZSVM_AS_BOOL(b);
		case _VALUE_NULL:   return true;
		case _VALUE_NUMBER: return _ZSVM_AS_NUMBER(a) == _ZSVM_AS_NUMBER(b);
		default: return; // Unreachable
	}
}