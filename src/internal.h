#ifndef _ZSVM_INTERNAL_H_
#define _ZSVM_INTERNAL_H_

#include <stddef.h>

#include "../include/zsvm/zsvm.h"

#define _Z_OP_RETURN 0x00
#define _Z_OP_CONSTANT 0x01

#define _Z_MEMORY_GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define _Z_MEMORY_GROW_ARRAY(type, pointer, old_capacity, new_capacity) \
    (type *)_z_memory_realloc(pointer, sizeof(type) * (old_capacity), sizeof(type) * (new_capacity))

#define _Z_MEMORY_FREE_ARRAY(type, pointer, old_capacity) \
    _z_memory_realloc(pointer, sizeof(type) * (old_capacity), 0)

typedef unsigned char _ZByte;
typedef double _ZValue; // NOTE this may need to be moved to the public header later on

typedef struct _ZValueArray _ZValueArray;
typedef struct _ZLine _ZLine;
typedef struct _ZLineArray _ZLineArray;
typedef struct _ZChunk _ZChunk;

struct _ZValueArray {
    int count;
    int capacity;
    _ZValue *values;
};

struct _ZLine {
    int line;
    int offset;
};

struct _ZLineArray {
    int count;
    int capacity;
    _ZLine *lines;
};

struct _ZChunk {
    int count;
    int capacity;
    _ZByte *code;
    _ZLineArray lines;
    _ZValueArray constants;
};

void _z_chunk_init(_ZChunk *chunk);
void _z_chunk_free(_ZChunk *chunk);
void _z_chunk_write(_ZChunk *chunk, _ZByte byte, int line);
int _z_chunk_add_constant(_ZChunk *chunk, _ZValue value);

void _z_value_array_init(_ZValueArray *array);
void _z_value_array_free(_ZValueArray *array);
void _z_value_array_write(_ZValueArray *array, _ZValue value);

void _z_value_print(_ZValue value);

void _z_debug_disassemble_chunk(_ZChunk *chunk, const char *name);
int _z_debug_disassemble_instruction(_ZChunk *chunk, int offset);

void *_z_memory_realloc(void *pointer, size_t old_size, size_t new_size);

#endif
