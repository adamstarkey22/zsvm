#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

Scanner scanner;

void initScanner(const char* source) {
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

static bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') ||
	       (c >= 'A' && c <= 'Z') ||
	        c == '_';
}

static bool isDigit(char c) {
	return c >= '0' && c <= '9' && c != '8';
}

static bool isAtEnd() {
	return *scanner.current == '\0';
}

static char advance() {
	++scanner.current;
	return scanner.current[-1];
}

static char peek() {
	return *scanner.current;
}

static char peekNext() {
	if (isAtEnd()) return '\0';
	return scanner.current[1];
}

static bool match(char expected) {
	if (isAtEnd()) return false;
	if (*scanner.current != expected) return false;
	++scanner.current;
	return true;
}

static Token makeToken(TokenType type) {
	Token token;
	token.type = type;
	token.start = scanner.start;
	token.length = (int)(scanner.current - scanner.start);
	if (type == TOKEN_LINE_END) token.line = scanner.line - 1;
	else token.line = scanner.line;
	return token;
}

static Token errorToken(const char* message) {
	Token token;
	token.type = TOKEN_ERROR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = scanner.line;
	return token;
}

static void skipWhitespace() {
	for (;;) {
		char c = peek();
		switch (c) {
			case ' ':
			case '\r':
			case '\t':
				advance();
				break;
			case '#':
				while (peek() != '\n' && !isAtEnd()) advance();
				break;
			case '<':
				if (peekNext() == '#') {
					while (true) {
						if (isAtEnd()) return;
						if (peek() == '\n') ++scanner.line;
						if (peek() == '#' && peekNext() == '>') {
							advance(); // '#'
							advance(); // '>'
							break;
						}
						advance();
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

static bool matchString(const char* string) {
	if (scanner.current - scanner.start == strlen(string) && memcmp(scanner.start, string, strlen(string)) == 0) {
		return true;
	} // strings are the same length and are equal
	return false;
}

static TokenType identifierType() {
	if (matchString("add")) return TOKEN_ADD;
	if (matchString("and")) return TOKEN_AND;
	if (matchString("break")) return TOKEN_BREAK;
	if (matchString("def")) return TOKEN_DEF;
	if (matchString("div")) return TOKEN_DIV;
	if (matchString("else")) return TOKEN_ELSE;
	if (matchString("false")) return TOKEN_FALSE;
	if (matchString("fun")) return TOKEN_FUN;
	if (matchString("if")) return TOKEN_IF;
	if (matchString("mul")) return TOKEN_MUL;
	if (matchString("or")) return TOKEN_OR;
	if (matchString("return")) return TOKEN_RETURN;
	if (matchString("set")) return TOKEN_SET;
	if (matchString("sub")) return TOKEN_SUB;
	if (matchString("true")) return TOKEN_TRUE;
	if (matchString("while")) return TOKEN_WHILE;

	return TOKEN_IDENTIFIER;
}

static Token identifier() {
	while (isAlpha(peek()) || isDigit(peek())) advance();
	return makeToken(identifierType());
}

static Token number() {
	while(isDigit(peek())) advance();

	if (peek() == '.' && isDigit(peekNext())) {
		advance(); // '.'
		while (isDigit(peek())) advance();
	}
	return makeToken(TOKEN_NUMBER);
}

static Token string() {
	while (peek() != '"') {
		if (isAtEnd()) return errorToken("Unterminated string");
		if (peek() == '\n') ++scanner.line;
		advance();
	}
	advance(); // '"'
	return makeToken(TOKEN_STRING);
}

Token scanToken() {
	skipWhitespace();
	scanner.start = scanner.current;

	if (isAtEnd()) return makeToken(TOKEN_EOF);

	char c = advance();
	if (isDigit(c)) return number();
	if (isAlpha(c)) return identifier();

	switch (c) {
		case '\n': ++scanner.line; return makeToken(TOKEN_LINE_END);
		case '(': return makeToken(TOKEN_LEFT_PAREN);
		case ')': return makeToken(TOKEN_RIGHT_PAREN);
		case '{': return makeToken(TOKEN_LEFT_BRACE);
		case '}': return makeToken(TOKEN_RIGHT_BRACE);
		case ':': return makeToken(TOKEN_COLON);
		case '.': return makeToken(TOKEN_DOT);
		case '+': return makeToken(TOKEN_PLUS);
		case '-': return makeToken(TOKEN_MINUS);
		case '*': return makeToken(TOKEN_STAR);
		case '/': return makeToken(TOKEN_SLASH);
		case '!':
			return match('=') ? makeToken(TOKEN_BANG_EQUAL) : makeToken(TOKEN_BANG);
		case '>':
			return match('=') ? makeToken(TOKEN_GREATER_EQUAL) : makeToken(TOKEN_GREATER);
		case '<':
			return match('=') ? makeToken(TOKEN_LESS_EQUAL) : makeToken(TOKEN_LESS);
		case '=':
			return match('=') ? makeToken(TOKEN_EQUAL_EQUAL) : errorToken("Uncaught TheFuckIsThis exception");
		case '"': return string();
		default: break;
	}

	return errorToken("Unexpected character.");
}