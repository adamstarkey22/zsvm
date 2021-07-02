#ifndef zsvm_value_h
#define zsvm_value_h

#include "common.h"

typedef double Value;

typedef struct {
	int count;
	int capacity;
	Value* data;
} ValueArray;

void initValueArray(ValueArray* array);
void freeValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void printValue(Value value);

#endif