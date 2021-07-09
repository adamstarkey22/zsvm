#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "compiler.h"
#include "debug.h"
#include "program.h"
#include "scanner.h"

typedef struct {
	_ZSVMscanner scanner;
	_ZSVMtoken current;
	_ZSVMtoken previous;
	ZSVMprogram* program;
	bool hadError;
	bool panicMode;
} Parser;

typedef enum {
	PREC_NONE,
	PREC_ASSIGNMENT, // set
	PREC_OR,		 // or
	PREC_AND,		 // and
	PREC_EQUALITY,	 // == !=
	PREC_COMPARISON, // < > <= >=
	PREC_TERM,		 // + -
	PREC_FACTOR,	 // * /
	PREC_UNARY,		 // ! -
	PREC_CALL,		 // . ()
	PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)();

typedef struct {
	ParseFn prefix;
	ParseFn infix;
	Precedence precedence;
} ParseRule;

static void errorAt(Parser* parser, _ZSVMtoken* token, const char* message) {
	if (parser->panicMode) return;
	parser->panicMode = true;
	fprintf(stderr, "[line %d] Error", token->line);

	if (token->type == _TOKEN_EOF) {
		fprintf(stderr, " at end");
	}
	else if (token->type == _TOKEN_ERROR) {
		// Nothing
	}
	else {
		fprintf(stderr, " at '%.*s'", token->length, token->start);
	}

	fprintf(stderr, ": %s\n", message);
	parser->hadError = true;
}

static void error(Parser* parser, const char* message) {
	errorAt(parser, &parser->previous, message);
}

static void errorAtCurrent(Parser* parser, const char* message) {
	errorAt(parser, &parser->current, message);
}

static void advance(Parser* parser) {
	parser->previous = parser->current;

	for (;;) {
		parser->current = _zsvmScanToken(&parser->scanner);
		if (parser->current.type != _TOKEN_ERROR) break;

		errorAtCurrent(parser, parser->current.start);
	}
}

static void consume(Parser* parser, _ZSVMtokentype type, const char* message) {
	if (parser->current.type == type) {
		advance(parser);
		return;
	}
	errorAtCurrent(parser, message);
}

static void emitByte(Parser* parser, uint8_t byte) {
	_zsvmWriteProgramByte(parser->program, byte);
}

static void emitBytes(Parser* parser, uint8_t byte1, uint8_t byte2) {
	emitByte(parser, byte1);
	emitByte(parser, byte2);
}

static uint8_t makeConstant(Parser* parser, _ZSVMvalue value) {
	int location = (int)_zsvmWriteProgramConstant(parser->program, value);

	if (location > UINT8_MAX) {
		error(parser, "Maximum constant pool size exceeded.");
		return 0;
	}
	return location;
}

static void emitConstant(Parser* parser, _ZSVMvalue value) {
	emitBytes(parser, _OP_CONSTANT, makeConstant(parser, value));
}

static void endCompiler(Parser* parser) {
	emitByte(parser, _OP_RETURN);
}

static void expression(Parser* parser);
static ParseRule* getRule(_ZSVMtokentype type);
static void parsePrecedence(Parser* parser, Precedence precedence);

static void binary(Parser* parser) {
	_ZSVMtokentype operatorType = parser->previous.type;
	ParseRule* rule = getRule(operatorType);
	parsePrecedence(parser, (Precedence)(rule->precedence + 1));

	switch (operatorType) {
		case _TOKEN_PLUS:  emitByte(parser, _OP_ADD); break;
		case _TOKEN_MINUS: emitByte(parser, _OP_SUBTRACT); break;
		case _TOKEN_STAR:  emitByte(parser, _OP_MULTIPLY); break;
		case _TOKEN_SLASH: emitByte(parser, _OP_DIVIDE); break;
		default: return; // Unreachable
	}
}

static void grouping(Parser* parser) {
	expression(parser);
	consume(parser, _TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void number(Parser* parser) {
	double value = strtod(parser->previous.start, NULL);
	emitConstant(parser, value);
}

static void unary(Parser* parser) {
	_ZSVMtokentype operatorType = parser->previous.type;

	parsePrecedence(parser, PREC_UNARY);

	switch (operatorType) {
		case _TOKEN_MINUS: emitByte(parser, _OP_NEGATE); break;
		default: return; // Unreachable
	}
}

ParseRule rules[] = {
	[_TOKEN_LEFT_PAREN]    = {grouping,    NULL,        PREC_NONE},
	[_TOKEN_RIGHT_PAREN]   = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_LEFT_BRACE]    = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_RIGHT_BRACE]   = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_COLON]         = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_DOT]           = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_PLUS]          = {NULL,        binary,      PREC_TERM},
	[_TOKEN_MINUS]         = {unary,       binary,      PREC_TERM},
	[_TOKEN_STAR]          = {NULL,        binary,      PREC_FACTOR},
	[_TOKEN_SLASH]         = {NULL,        binary,      PREC_FACTOR},
	[_TOKEN_LINE_END]      = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_EQUAL_EQUAL]   = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_BANG]          = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_BANG_EQUAL]    = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_GREATER]       = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_GREATER_EQUAL] = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_LESS]          = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_LESS_EQUAL]    = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_STRING]        = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_NUMBER]        = {number,      NULL,        PREC_NONE},
	[_TOKEN_IDENTIFIER]    = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_ADD]           = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_AND]           = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_BREAK]         = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_DEF]           = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_DIV]           = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_ELSE]          = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_FALSE]         = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_FUN]           = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_IF]            = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_MUL]           = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_OR]            = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_RETURN]        = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_SET]           = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_SUB]           = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_TRUE]          = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_WHILE]         = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_ERROR]         = {NULL,        NULL,        PREC_NONE},
	[_TOKEN_EOF]           = {NULL,        NULL,        PREC_NONE},
};

static void parsePrecedence(Parser* parser, Precedence precedence) {
	advance(parser);
	ParseFn prefixRule = getRule(parser->previous.type)->prefix;
	if (prefixRule == NULL) {
		error(parser, "Expect expression.");
		return;
	}
	prefixRule(parser);

	while (precedence <= getRule(parser->current.type)->precedence) {
		advance(parser);
		ParseFn infixRule = getRule(parser->previous.type)->infix;
		infixRule(parser);
	}
}

static ParseRule* getRule(_ZSVMtokentype type) {
	return &rules[type];
}

static void expression(Parser* parser) {
	parsePrecedence(parser, PREC_ASSIGNMENT);
}

ZSVMresult zsvmCompileProgram(ZSVMprogram* program, const char* source) {
	/*_ZSVMscanner debugScanner;
	_zsvmInitScanner(&debugScanner, source);
	for (;;) {
		_ZSVMtoken token = _zsvmScanToken(&debugScanner);

		printf("%4d %-16s %.*s\n", token.line, _ZSVMtokenstrings[token.type], token.length, token.start);

		if (token.type == _TOKEN_EOF) break;
	}
	printf("\n");*/

	Parser parser;
	parser.hadError = false;
	parser.panicMode = false;
	parser.program = program;
	_zsvmInitScanner(&parser.scanner, source);

	advance(&parser);
	expression(&parser);
	consume(&parser, _TOKEN_EOF, "Expect end of expression.");
	endCompiler(&parser);

	if (parser.hadError) return ZSVM_COMPILE_ERROR;
	else return ZSVM_OK;
}