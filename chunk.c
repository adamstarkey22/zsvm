#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->data = NULL;
	initValueArray(&chunk->constants);
	initLineArray(&chunk->lines);
}

void freeChunk(Chunk* chunk) {
	FREE_ARRAY(uint8_t, chunk->data, chunk->capacity);
	freeValueArray(&chunk->constants);
	freeLineArray(&chunk->lines);
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

int addConstant(Chunk* chunk, Value value) {
	writeValueArray(&chunk->constants, value);
	return chunk->constants.count - 1;
}

void initLineArray(LineArray* array) {
	array->count = 0;
	array->capacity = 0;
	array->data = NULL;
}

void freeLineArray(LineArray* array) {
	FREE_ARRAY(Line, array->data, array->capacity);
	initLineArray(array);
}

void writeLine(Chunk* chunk, int line) {
	if (chunk->lines.count == chunk->lines.capacity) {
		int oldCapacity = chunk->lines.capacity;
		chunk->lines.capacity = GROW_CAPACITY(oldCapacity);
		chunk->lines.data = GROW_ARRAY(Line, chunk->lines.data, oldCapacity, chunk->lines.capacity);
	}
	
	// Set the next line's offset to the offset of the chunk's next byte
	chunk->lines.data[chunk->lines.count].number = line;
	chunk->lines.data[chunk->lines.count].offset = chunk->count;
	chunk->lines.count++;
}

int decodeLine(LineArray* lines, int offset) {
	int line = -1;
	for (int i = 0; i < lines->count; ++i) {
		if (lines->data[i].offset > offset) break;
		line = lines->data[i].number;
	}
	return line;
}