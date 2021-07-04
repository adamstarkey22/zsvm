#ifndef zsvm_scanner_h
#define zsvm_scanner_h

typedef enum {
	// Single-character tokens
	TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
	TOKEN_COLON, TOKEN_DOT,
	TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH,
	TOKEN_LINE_END,

	// One or two character tokens
	TOKEN_EQUAL_EQUAL,
	TOKEN_BANG, TOKEN_BANG_EQUAL,
	TOKEN_GREATER, TOKEN_GREATER_EQUAL,
	TOKEN_LESS, TOKEN_LESS_EQUAL,

	// Literals
	TOKEN_STRING,
	TOKEN_NUMBER,
	TOKEN_IDENTIFIER,

	// Keywords
	TOKEN_ADD,
	TOKEN_AND,
	TOKEN_BREAK,
	TOKEN_DEF,
	TOKEN_DIV,
	TOKEN_ELSE,
	TOKEN_FALSE,
	TOKEN_FUN,
	TOKEN_IF,
	TOKEN_MUL,
	TOKEN_OR,
	TOKEN_RETURN,
	TOKEN_SET,
	TOKEN_SUB,
	TOKEN_TRUE,
	TOKEN_WHILE,

	TOKEN_ERROR, TOKEN_EOF
} TokenType;

typedef struct {
	TokenType type;
	const char* start;
	int length;
	int line;
} Token;

typedef struct {
	const char* start;
	const char* current;
	int line;
} Scanner;

void initScanner(const char* source);
Token scanToken();

#endif