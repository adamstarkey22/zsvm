#ifndef zsvm_scanner_h
#define zsvm_scanner_h

typedef enum {
	// Single-character tokens
	_TOKEN_LEFT_PAREN,    _TOKEN_RIGHT_PAREN,
	_TOKEN_LEFT_BRACE,    _TOKEN_RIGHT_BRACE,
	_TOKEN_PLUS,
	_TOKEN_MINUS,
	_TOKEN_STAR,
	_TOKEN_SLASH,

	_TOKEN_COLON,
	_TOKEN_DOT,
	_TOKEN_LINE_END,

	_TOKEN_BANG_EQUAL,    _TOKEN_BANG,
	_TOKEN_GREATER_EQUAL, _TOKEN_GREATER,
	_TOKEN_LESS_EQUAL,    _TOKEN_LESS,
	_TOKEN_EQUAL_EQUAL,

	// Literals
	_TOKEN_STRING,
	_TOKEN_NUMBER,
	_TOKEN_IDENTIFIER,

	// Keywords
	_TOKEN_ADD,
	_TOKEN_AND,
	_TOKEN_BREAK,
	_TOKEN_DEF,
	_TOKEN_DIV,
	_TOKEN_ELSE,
	_TOKEN_FALSE,
	_TOKEN_FUN,
	_TOKEN_IF,
	_TOKEN_MUL,
	_TOKEN_OR,
	_TOKEN_RETURN,
	_TOKEN_SET,
	_TOKEN_SUB,
	_TOKEN_TRUE,
	_TOKEN_WHILE,

	_TOKEN_ERROR,
	_TOKEN_EOF
} _ZSVMtokentype;

typedef struct {
	_ZSVMtokentype type;
	const char* start;
	int length;
	int line;
} _ZSVMtoken;

typedef struct {
	const char* start;
	const char* current;
	int line;
} _ZSVMscanner;

void _zsvmInitScanner(_ZSVMscanner* scanner, const char* source);
_ZSVMtoken _zsvmScanToken(_ZSVMscanner* scanner);

#endif