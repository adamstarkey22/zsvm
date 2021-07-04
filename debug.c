#include <stdio.h>

#include "debug.h"


const char* tokenTypeAsString(TokenType type) {
#define AS_STRING(TOKEN_TYPE) #TOKEN_TYPE

	switch (type) {
		case TOKEN_ADD:			  return AS_STRING(TOKEN_ADD);
		case TOKEN_AND:			  return AS_STRING(TOKEN_AND);
		case TOKEN_BANG:		  return AS_STRING(TOKEN_BANG);
		case TOKEN_BANG_EQUAL:	  return AS_STRING(TOKEN_EQUAL);
		case TOKEN_BREAK:		  return AS_STRING(TOKEN_BREAK);
		case TOKEN_COLON:		  return AS_STRING(TOKEN_COLON);
		case TOKEN_DEF:			  return AS_STRING(TOKEN_DEF);
		case TOKEN_DIV:			  return AS_STRING(TOKEN_DIV);
		case TOKEN_DOT:			  return AS_STRING(TOKEN_DOT);
		case TOKEN_ELSE:		  return AS_STRING(TOKEN_ELSE);
		case TOKEN_EOF:			  return AS_STRING(TOKEN_EOF);
		case TOKEN_ERROR:		  return AS_STRING(TOKEN_ERROR);
		case TOKEN_EQUAL_EQUAL:	  return AS_STRING(TOKEN_EQUAL_EQUAL);
		case TOKEN_FALSE:		  return AS_STRING(TOKEN_FALSE);
		case TOKEN_FUN:			  return AS_STRING(TOKEN_FUN);
		case TOKEN_GREATER:		  return AS_STRING(TOKEN_GREATER);
		case TOKEN_GREATER_EQUAL: return AS_STRING(TOKEN_GREATER_EQUAL);
		case TOKEN_IDENTIFIER:	  return AS_STRING(TOKEN_IDENTIFIER);
		case TOKEN_IF:			  return AS_STRING(TOKEN_IF);
		case TOKEN_LEFT_BRACE:	  return AS_STRING(TOKEN_LEFT_BRACE);
		case TOKEN_LEFT_PAREN:	  return AS_STRING(TOKEN_LEFT_PAREN);
		case TOKEN_LESS:		  return AS_STRING(TOKEN_LESS);
		case TOKEN_LESS_EQUAL:	  return AS_STRING(TOKEN_LESS_EQUAL);
		case TOKEN_LINE_END:	  return AS_STRING(TOKEN_LINE_END);
		case TOKEN_MINUS:		  return AS_STRING(TOKEN_MINUS);
		case TOKEN_MUL:			  return AS_STRING(TOKEN_MUL);
		case TOKEN_NUMBER:		  return AS_STRING(TOKEN_NUMBER);
		case TOKEN_OR:			  return AS_STRING(TOKEN_OR);
		case TOKEN_PLUS:		  return AS_STRING(TOKEN_PLUS);
		case TOKEN_RETURN:		  return AS_STRING(TOKEN_RETURN);
		case TOKEN_RIGHT_BRACE:	  return AS_STRING(TOKEN_RIGHT_BRACE);
		case TOKEN_RIGHT_PAREN:	  return AS_STRING(TOKEN_RIGHT_PAREN);
		case TOKEN_SET:			  return AS_STRING(TOKEN_SET);
		case TOKEN_SLASH:		  return AS_STRING(TOKEN_SLASH);
		case TOKEN_STAR:		  return AS_STRING(TOKEN_STAR);
		case TOKEN_STRING:		  return AS_STRING(TOKEN_STRING);
		case TOKEN_SUB:			  return AS_STRING(TOKEN_SUB);
		case TOKEN_TRUE:		  return AS_STRING(TOKEN_TRUE);
		case TOKEN_WHILE:		  return AS_STRING(TOKEN_WHILE);
		default: break;
	}

	return "Unrecognised token";

#undef AS_STRING
}

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