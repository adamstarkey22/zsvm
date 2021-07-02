#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->data = NULL;
}

void freeChunk(Chunk* chunk) {
	FREE_ARRAY(uint8_t, chunk->data, chunk->capacity);
	initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte) {
	// Resize chunk if it is full
	if (chunk->count == chunk->capacity) {
		int oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(oldCapacity);
		chunk->data = GROW_ARRAY(uint8_t, chunk->data, oldCapacity, chunk->capacity);
	}

	// Append byte to end of chunk and increase count
	chunk->data[chunk->count] = byte;
	chunk->count++;
}