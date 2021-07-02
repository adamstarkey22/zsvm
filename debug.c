#include <stdio.h>

#include "debug.h"

void disassembleChunk(Chunk* chunk, const char* name) {
	printf("==== %s ====\n", name);
	for (int offset = 0; offset < chunk->count;) {
		offset = disassembleInstruction(chunk, offset);
	}
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
	uint8_t constant = chunk->data[offset + 1];
	printf("%-16s %2d | ", name, constant);
	printValue(chunk->constants.data[constant]);
	printf("\n");
	return offset + 2;
}

static int simpleInstruction(const char* name, int offset) {
	printf("%s\n", name);
	return offset + 1;
}

int disassembleInstruction(Chunk* chunk, int offset) {
	int lineNumber = decodeLine(&chunk->lines, offset);
	int lastLineNumber = decodeLine(&chunk->lines, offset - 1);

	if (lineNumber == lastLineNumber) {
		printf("   | ");
	}
	else {
	printf("%4d ", lineNumber);
	}

	printf("%04d ", offset);

	uint8_t instruction = chunk->data[offset];
	switch (instruction) {
		case OP_CONSTANT:
			return constantInstruction("OP_CONSTANT", chunk, offset);
		case OP_ADD:
			return simpleInstruction("OP_ADD", offset);
		case OP_SUBTRACT:
			return simpleInstruction("OP_SUBTRACT", offset);
		case OP_MULTIPLY:
			return simpleInstruction("OP_MULTIPLY", offset);
		case OP_DIVIDE:
			return simpleInstruction("OP_DIVIDE", offset);
		case OP_NEGATE:
			return simpleInstruction("OP_NEGATE", offset);
		case OP_RETURN:
			return simpleInstruction("OP_RETURN", offset);
		default:
			printf("Unrecognised opcode %d\n", instruction);
			return offset + 1;
	}
}