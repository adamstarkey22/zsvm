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
	printf("%g", value);
}