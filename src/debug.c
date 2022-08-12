#include <stdio.h>

#include "internal.h"

void _z_debug_disassemble_chunk(_ZChunk *chunk, const char *name)
{
    printf("== %s ==\n", name);
/*
    for (int i = 0; i < chunk->lines.count; ++i) {
        printf("[line %4d @ offset %04d]\n", chunk->lines.lines[i].line, chunk->lines.lines[i].offset);
    } // temporary line info debugging
*/
    for (int offset = 0; offset < chunk->count;) {
        offset = _z_debug_disassemble_instruction(chunk, offset);
    }
}

static int constant_instruction(const char *name, _ZChunk *chunk, int offset)
{
    _ZByte constant = chunk->code[offset + 1];
    printf("%-16s %04d -> '", name, constant);
    _z_value_print(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

static int simple_instruction(const char *name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

int _z_debug_disassemble_instruction(_ZChunk *chunk, int offset)
{
    int line = 0;
    int line_offset = 0;

    for (int i = 0; i < chunk->lines.count; ++i) {
        if (chunk->lines.lines[i].offset > offset) {
            break;
        }

        line = chunk->lines.lines[i].line;
        line_offset = chunk->lines.lines[i].offset;
    }

    if (line_offset == offset) {
       printf("%4d ", line);
    } else {
        printf("   | ");
    } 

    printf("%04d ", offset);
    
    _ZByte instruction = chunk->code[offset];
    switch (instruction) {
        case _Z_OP_CONSTANT:
            return constant_instruction("CONSTANT", chunk, offset);
        case _Z_OP_RETURN:
            return simple_instruction("RETURN", offset);
        default:
            printf("Invalid Opcode: 0x%02X\n", instruction);
            return offset + 1;
    }
}
