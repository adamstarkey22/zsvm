#include <stdio.h>

#include "internal.h"

void _z_value_array_init(_ZValueArray *array)
{
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void _z_value_array_free(_ZValueArray *array)
{
    _Z_MEMORY_FREE_ARRAY(_ZValue, array->values, array->capacity);
    _z_value_array_init(array);
}

void _z_value_array_write(_ZValueArray *array, _ZValue value)
{
    if (array->count >= array->capacity) {
        int old_capacity = array->capacity;
        array->capacity = _Z_MEMORY_GROW_CAPACITY(old_capacity);
        array->values = _Z_MEMORY_GROW_ARRAY(_ZValue, array->values, old_capacity, array->capacity);
    }

    array->values[array->count++] = value;
}

void _z_value_print(_ZValue value)
{
    printf("%g", value);
}
