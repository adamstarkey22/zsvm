#include <stdio.h>

#include "debug.h"
#include "program.h"

static size_t simpleInstruction(const char* name, size_t offset) {
	printf("%s\n", name);
	return offset + 1;
}

static size_t constantInstruction(const char* name, ZSVMprogram* program, size_t offset) {
	uint8_t location = program->data[offset + 1];
	printf("%s %3d -> \"", name, location);
	_zsvmPrintValue(program->constants.data[location]);
	printf("\"\n");
	return offset + 2;
}

void _zsvmDisassembleProgram(ZSVMprogram* program, const char* label) {
	printf("========== %s ==========\n", label);
	for (size_t offset = 0; offset < program->count;) {
		offset = _zsvmDisassembleInstruction(program, offset);
	}
	printf("\n");
}

size_t _zsvmDisassembleInstruction(ZSVMprogram* program, size_t offset) {
	printf("%4d ", (int)offset);

	uint8_t instruction = program->data[offset];
	switch (instruction) {
		case _OP_RETURN:   return simpleInstruction("RETURN", offset);
		case _OP_ADD:      return simpleInstruction("ADD", offset);
		case _OP_SUBTRACT: return simpleInstruction("SUBTRACT", offset);
		case _OP_MULTIPLY: return simpleInstruction("MULTIPLY", offset);
		case _OP_DIVIDE:   return simpleInstruction("DIVIDE", offset);
		case _OP_NEGATE:   return simpleInstruction("NEGATE", offset);

		case _OP_CONSTANT: return constantInstruction("CONSTANT", program, offset);

		default:
			printf("Invalid opcode: %d\n", instruction);
			return offset + 1;
	}
}
const char* const _ZSVMtokenstrings[] = {
	[_TOKEN_LEFT_PAREN]    = "LEFT_PAREN",
	[_TOKEN_RIGHT_PAREN]   = "RIGHT_PAREN",
	[_TOKEN_LEFT_BRACE]    = "LEFT_BRACE",
	[_TOKEN_RIGHT_BRACE]   = "RIGHT_BRACE",
	[_TOKEN_PLUS]          = "PLUS",
	[_TOKEN_MINUS]         = "MINUS",
	[_TOKEN_STAR]          = "STAR",
	[_TOKEN_SLASH]         = "SLASH",
	[_TOKEN_COLON]         = "COLON",
	[_TOKEN_DOT]           = "DOT",
	[_TOKEN_LINE_END]      = "LINE_END",
	[_TOKEN_BANG_EQUAL]    = "BANG_EQUAL",
	[_TOKEN_BANG]          = "BANG",
	[_TOKEN_GREATER_EQUAL] = "GREATER_EQUAL",
	[_TOKEN_GREATER]       = "GREATER",
	[_TOKEN_LESS_EQUAL]    = "LESS_EQUAL",
	[_TOKEN_LESS]          = "LESS",
	[_TOKEN_EQUAL_EQUAL]   = "EQUAL_EQUAL",
	[_TOKEN_STRING]        = "STRING",
	[_TOKEN_NUMBER]        = "NUMBER",
	[_TOKEN_IDENTIFIER]    = "IDENTIFIER",
	[_TOKEN_ADD]           = "ADD",
	[_TOKEN_AND]           = "AND",
	[_TOKEN_BREAK]         = "BREAK",
	[_TOKEN_DEF]           = "DEF",
	[_TOKEN_DIV]           = "DIV",
	[_TOKEN_ELSE]          = "ELSE",
	[_TOKEN_FALSE]         = "FALSE",
	[_TOKEN_FUN]           = "FUN",
	[_TOKEN_IF]            = "IF",
	[_TOKEN_MUL]           = "MUL",
	[_TOKEN_OR]            = "OR",
	[_TOKEN_RETURN]        = "RETURN",
	[_TOKEN_SET]           = "SET",
	[_TOKEN_SUB]           = "SUB",
	[_TOKEN_TRUE]          = "TRUE",
	[_TOKEN_WHILE]         = "WHILE",
	[_TOKEN_ERROR]         = "ERROR",
	[_TOKEN_EOF]           = "EOF",
};