#include <stddef.h>

#include "internal.h"

void line_array_init(_ZLineArray *array)
{
    array->count = 0;
    array->capacity = 0;
    array->lines = NULL;
}

void _z_chunk_init(_ZChunk *chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    line_array_init(&chunk->lines);
    _z_value_array_init(&chunk->constants);
}

void line_array_free(_ZLineArray *array)
{
    _Z_MEMORY_FREE_ARRAY(_ZLine, array->lines, array->capacity);
    line_array_init(array);
}

void _z_chunk_free(_ZChunk *chunk)
{
    _Z_MEMORY_FREE_ARRAY(_ZByte, chunk->code, chunk->capacity);
    _z_value_array_free(&chunk->constants);
    _z_chunk_init(chunk);
}

void line_array_write(_ZLineArray *array, int line, int offset)
{
    if (array->count > 0 && array->lines[array->count - 1].line == line) {
        return;
    }

    if (array->count >= array->capacity) {
        int old_capacity = array->capacity;
        array->capacity = _Z_MEMORY_GROW_CAPACITY(old_capacity);
        array->lines = _Z_MEMORY_GROW_ARRAY(_ZLine, array->lines, old_capacity, array->capacity);
    }

    array->lines[array->count].line = line;
    array->lines[array->count].offset = offset;
    ++array->count;
}

void _z_chunk_write(_ZChunk *chunk, _ZByte byte, int line)
{
    if (chunk->count >= chunk->capacity) {
        int old_capacity = chunk->capacity;
        chunk->capacity = _Z_MEMORY_GROW_CAPACITY(old_capacity);
        chunk->code = _Z_MEMORY_GROW_ARRAY(_ZByte, chunk->code, old_capacity, chunk->capacity);
    }

    chunk->code[chunk->count++] = byte;
    line_array_write(&chunk->lines, line, chunk->count - 1);
}

int _z_chunk_add_constant(_ZChunk *chunk, _ZValue value)
{
    _z_value_array_write(&chunk->constants, value);
    return chunk->constants.count - 1;
}
