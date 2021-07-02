#ifndef zsvm_chunk_h
#define zsvm_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
	OP_CONSTANT,
	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,
	OP_NEGATE,
	OP_RETURN,
} OpCode;

typedef struct {
	int number;
	int offset;
} Line;

typedef struct {
	int count;
	int capacity;
	Line* data;
} LineArray;

typedef struct {
	int count;
	int capacity;
	uint8_t* data;
	LineArray lines;
	ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);
int addConstant(Chunk* chunk, Value value);

void initLineArray(LineArray* array);
void freeLineArray(LineArray* array);
void writeLine(Chunk* chunk, int line);
int decodeLine(LineArray* lines, int offset);

#endif