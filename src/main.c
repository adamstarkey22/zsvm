#include <stdio.h>

#include "internal.h"

int main(int argc, char **argv)
{
    _ZChunk chunk;

    _z_chunk_init(&chunk);

    int constant_1 = _z_chunk_add_constant(&chunk, 1.2);
    int constant_2 = _z_chunk_add_constant(&chunk, 6.9);

    _z_chunk_write(&chunk, _Z_OP_CONSTANT, 1);
    _z_chunk_write(&chunk, constant_1, 1);
    _z_chunk_write(&chunk, _Z_OP_CONSTANT, 2);
    _z_chunk_write(&chunk, constant_2, 2);
    _z_chunk_write(&chunk, _Z_OP_RETURN, 2);

    _z_debug_disassemble_chunk(&chunk, "TEST CHUNK");

    _z_chunk_free(&chunk);

    return 0;
}
