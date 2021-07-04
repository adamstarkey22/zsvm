#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

void initScanner(Scanner* scanner, const char* source) {
	scanner->start = source;
	scanner->current = source;
	scanner->line = 1;
}

static bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') ||
	       (c >= 'A' && c <= 'Z') ||
	        c == '_';
}

static bool isDigit(char c) {
	return c >= '0' && c <= '9' && c != '8';
}

static bool isAtEnd(Scanner* scanner) {
	return *scanner->current == '\0';
}

static char advance(Scanner* scanner) {
	++scanner->current;
	return scanner->current[-1];
}

static char peek(Scanner* scanner) {
	return *scanner->current;
}

static char peekNext(Scanner* scanner) {
	if (isAtEnd(scanner)) return '\0';
	return scanner->current[1];
}

static bool match(Scanner* scanner, char expected) {
	if (isAtEnd(scanner)) return false;
	if (*scanner->current != expected) return false;
	++scanner->current;
	return true;
}

static Token makeToken(Scanner* scanner, TokenType type) {
	Token token;
	token.type = type;
	token.start = scanner->start;
	token.length = (int)(scanner->current - scanner->start);
	if (type == TOKEN_LINE_END) token.line = scanner->line - 1;
	else token.line = scanner->line;
	return token;
}

static Token errorToken(int line, const char* message) {
	Token token;
	token.type = TOKEN_ERROR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = line;
	return token;
}

static void skipWhitespace(Scanner* scanner) {
	for (;;) {
		char c = peek(scanner);
		switch (c) {
			case ' ':
			case '\r':
			case '\t':
				advance(scanner);
				break;
			case '#':
				while (peek(scanner) != '\n' && !isAtEnd(scanner)) advance(scanner);
				break;
			case '<':
				if (peekNext(scanner) == '#') {
					while (true) {
						if (isAtEnd(scanner)) return;
						if (peek(scanner) == '\n') ++scanner->line;
						if (peek(scanner) == '#' && peekNext(scanner) == '>') {
							advance(scanner); // '#'
							advance(scanner); // '>'
							break;
						}
						advance(scanner);
					}
				} else {
						return;
				}
				break;
			default:
				return;
		}
	}
}

static bool matchString(Scanner* scanner, const char* string) {
	if (scanner->current - scanner->start == strlen(string) && memcmp(scanner->start, string, strlen(string)) == 0) {
		return true;
	} // strings are the same length and are equal
	return false;
}

static TokenType identifierType(Scanner* scanner) {
	if (matchString(scanner, "add")) return TOKEN_ADD;
	if (matchString(scanner, "and")) return TOKEN_AND;
	if (matchString(scanner, "break")) return TOKEN_BREAK;
	if (matchString(scanner, "def")) return TOKEN_DEF;
	if (matchString(scanner, "div")) return TOKEN_DIV;
	if (matchString(scanner, "else")) return TOKEN_ELSE;
	if (matchString(scanner, "false")) return TOKEN_FALSE;
	if (matchString(scanner, "fun")) return TOKEN_FUN;
	if (matchString(scanner, "if")) return TOKEN_IF;
	if (matchString(scanner, "mul")) return TOKEN_MUL;
	if (matchString(scanner, "or")) return TOKEN_OR;
	if (matchString(scanner, "return")) return TOKEN_RETURN;
	if (matchString(scanner, "set")) return TOKEN_SET;
	if (matchString(scanner, "sub")) return TOKEN_SUB;
	if (matchString(scanner, "true")) return TOKEN_TRUE;
	if (matchString(scanner, "while")) return TOKEN_WHILE;

	return TOKEN_IDENTIFIER;
}

static Token identifier(Scanner* scanner) {
	while (isAlpha(peek(scanner)) || isDigit(peek(scanner))) advance(scanner);
	return makeToken(scanner, identifierType(scanner));
}

static Token number(Scanner* scanner) {
	while(isDigit(peek(scanner))) advance(scanner);

	if (peek(scanner) == '.' && isDigit(peekNext(scanner))) {
		advance(scanner); // '.'
		while (isDigit(peek(scanner))) advance(scanner);
	}
	return makeToken(scanner, TOKEN_NUMBER);
}

static Token string(Scanner* scanner) {
	while (peek(scanner) != '"') {
		if (isAtEnd(scanner)) return errorToken(scanner->line, "Unterminated string");
		if (peek(scanner) == '\n') ++scanner->line;
		advance(scanner);
	}
	advance(scanner); // '"'
	return makeToken(scanner, TOKEN_STRING);
}

Token scanToken(Scanner* scanner) {
	skipWhitespace(scanner);
	scanner->start = scanner->current;

	if (isAtEnd(scanner)) return makeToken(scanner, TOKEN_EOF);

	char c = advance(scanner);
	if (isDigit(c)) return number(scanner);
	if (isAlpha(c)) return identifier(scanner);

	switch (c) {
		case '\n': ++scanner->line; return makeToken(scanner, TOKEN_LINE_END);
		case '(': return makeToken(scanner, TOKEN_LEFT_PAREN);
		case ')': return makeToken(scanner, TOKEN_RIGHT_PAREN);
		case '{': return makeToken(scanner, TOKEN_LEFT_BRACE);
		case '}': return makeToken(scanner, TOKEN_RIGHT_BRACE);
		case ':': return makeToken(scanner, TOKEN_COLON);
		case '.': return makeToken(scanner, TOKEN_DOT);
		case '+': return makeToken(scanner, TOKEN_PLUS);
		case '-': return makeToken(scanner, TOKEN_MINUS);
		case '*': return makeToken(scanner, TOKEN_STAR);
		case '/': return makeToken(scanner, TOKEN_SLASH);
		case '!':
			return match(scanner, '=') ? makeToken(scanner, TOKEN_BANG_EQUAL) : makeToken(scanner, TOKEN_BANG);
		case '>':
			return match(scanner, '=') ? makeToken(scanner, TOKEN_GREATER_EQUAL) : makeToken(scanner, TOKEN_GREATER);
		case '<':
			return match(scanner, '=') ? makeToken(scanner, TOKEN_LESS_EQUAL) : makeToken(scanner, TOKEN_LESS);
		case '=':
			return match(scanner, '=') ? makeToken(scanner, TOKEN_EQUAL_EQUAL) : errorToken(scanner->line, "Uncaught TheFuckIsThis exception");
		case '"': return string(scanner);
		default: break;
	}

	return errorToken(scanner->line, "Unexpected character.");
}