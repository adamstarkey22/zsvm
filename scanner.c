#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "scanner.h"

static bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') ||
	       (c >= 'A' && c <= 'Z') ||
	        c == '_';
}

static bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

static bool isAtEnd(_ZSVMscanner* scanner) {
	return *scanner->current == '\0';
}

static char advance(_ZSVMscanner* scanner) {
	scanner->current++;
	return scanner->current[-1];
}

static char peek(_ZSVMscanner* scanner) {
	return *scanner->current;
}

static char peekNext(_ZSVMscanner* scanner) {
	if (isAtEnd(scanner)) return '\0';
	return scanner->current[1];
}

static bool match(_ZSVMscanner* scanner, char expected) {
	if (isAtEnd(scanner)) return false;
	if (*scanner->current != expected) return false;
	scanner->current++;
	return true;
}

static _ZSVMtoken makeToken(_ZSVMscanner* scanner, _ZSVMtokentype type) {
	_ZSVMtoken token;
	token.type = type;
	token.start = scanner->start;
	token.length = (int)(scanner->current - scanner->start);
	token.line = scanner->line;

	if (type == _TOKEN_LINE_END) scanner->line++;
	return token;
}

static _ZSVMtoken errorToken(const char* message, int line) {
	_ZSVMtoken token;
	token.type = _TOKEN_ERROR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = line;
	return token;
}

static void skipWhitespace(_ZSVMscanner* scanner) {
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
					advance(scanner); // '<'
					advance(scanner); // '#'
					while (true) {
						if (isAtEnd(scanner)) return;
						if (peek(scanner) == '\n') scanner->line++;
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

static bool matchString(_ZSVMscanner* scanner, const char* string) {
	if (scanner->current - scanner->start == strlen(string) && memcmp(scanner->start, string, strlen(string)) == 0) {
		return true;
	} // strings are the same length and are equal
	return false;
}

static _ZSVMtokentype identifierType(_ZSVMscanner* scanner) {
	if (matchString(scanner, "add"))    return _TOKEN_ADD;
	if (matchString(scanner, "and"))    return _TOKEN_AND;
	if (matchString(scanner, "break"))  return _TOKEN_BREAK;
	if (matchString(scanner, "def"))    return _TOKEN_DEF;
	if (matchString(scanner, "div"))    return _TOKEN_DIV;
	if (matchString(scanner, "else"))   return _TOKEN_ELSE;
	if (matchString(scanner, "false"))  return _TOKEN_FALSE;
	if (matchString(scanner, "fun"))    return _TOKEN_FUN;
	if (matchString(scanner, "if"))     return _TOKEN_IF;
	if (matchString(scanner, "mul"))    return _TOKEN_MUL;
	if (matchString(scanner, "null"))    return _TOKEN_NULL;
	if (matchString(scanner, "or"))     return _TOKEN_OR;
	if (matchString(scanner, "return")) return _TOKEN_RETURN;
	if (matchString(scanner, "set"))    return _TOKEN_SET;
	if (matchString(scanner, "sub"))    return _TOKEN_SUB;
	if (matchString(scanner, "true"))   return _TOKEN_TRUE;
	if (matchString(scanner, "while"))  return _TOKEN_WHILE;

	return _TOKEN_IDENTIFIER;
}

static _ZSVMtoken identifier(_ZSVMscanner* scanner) {
	while (isAlpha(peek(scanner)) || isDigit(peek(scanner))) advance(scanner);
	return makeToken(scanner, identifierType(scanner));
}

static _ZSVMtoken number(_ZSVMscanner* scanner) {
	while(isDigit(peek(scanner))) advance(scanner);

	if (peek(scanner) == '.' && isDigit(peekNext(scanner))) {
		advance(scanner); // '.'
		while (isDigit(peek(scanner))) advance(scanner);
	}
	return makeToken(scanner, _TOKEN_NUMBER);
}

static _ZSVMtoken string(_ZSVMscanner* scanner) {
	while (peek(scanner) != '"') {
		if (isAtEnd(scanner)) return errorToken("Unterminated string", scanner->line);
		if (peek(scanner) == '\n') scanner->line++;
		advance(scanner);
	}
	advance(scanner); // '"'
	return makeToken(scanner, _TOKEN_STRING);
}

void _zsvmInitScanner(_ZSVMscanner* scanner, const char* source) {
	scanner->start = source;
	scanner->current = source;
	scanner->line = 1;
}

_ZSVMtoken _zsvmScanToken(_ZSVMscanner* scanner) {
	skipWhitespace(scanner);
	scanner->start = scanner->current;

	if (isAtEnd(scanner)) return makeToken(scanner, _TOKEN_EOF);

	char c = advance(scanner);
	if (isDigit(c)) return number(scanner);
	if (isAlpha(c)) return identifier(scanner);

	switch (c) {
		case '\n': return makeToken(scanner, _TOKEN_LINE_END);
		case '(':  return makeToken(scanner, _TOKEN_LEFT_PAREN);
		case ')':  return makeToken(scanner, _TOKEN_RIGHT_PAREN);
		case '{':  return makeToken(scanner, _TOKEN_LEFT_BRACE);
		case '}':  return makeToken(scanner, _TOKEN_RIGHT_BRACE);
		case ':':  return makeToken(scanner, _TOKEN_COLON);
		case '.':  return makeToken(scanner, _TOKEN_DOT);
		case '+':  return makeToken(scanner, _TOKEN_PLUS);
		case '-':  return makeToken(scanner, _TOKEN_MINUS);
		case '*':  return makeToken(scanner, _TOKEN_STAR);
		case '/':  return makeToken(scanner, _TOKEN_SLASH);

		case '!': return match(scanner, '=') ? makeToken(scanner, _TOKEN_BANG_EQUAL)    : makeToken(scanner, _TOKEN_BANG);
		case '>': return match(scanner, '=') ? makeToken(scanner, _TOKEN_GREATER_EQUAL) : makeToken(scanner, _TOKEN_GREATER);
		case '<': return match(scanner, '=') ? makeToken(scanner, _TOKEN_LESS_EQUAL)    : makeToken(scanner, _TOKEN_LESS);
		case '=': if (match(scanner, '=')) return makeToken(scanner, _TOKEN_EQUAL_EQUAL); break;

		case '"': return string(scanner);
		default: break;
	}

	return errorToken("Unexpected character.", scanner->line);
}