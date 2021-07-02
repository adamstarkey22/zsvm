#include "common.h"
#include "chunk.h"
#include "debug.h"

int main() {
	Chunk chunk;
	initChunk(&chunk);
	writeByte(&chunk, OP_RETURN);
	disassembleChunk(&chunk, "Test Chunk");
	freeChunk(&chunk);
	return 0;
}