#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "scanner.h"

void compile(const char* source) {
	Scanner scanner;
	initScanner(&scanner, source);
	int line = -1;
	for (;;) {
		Token token = scanToken(&scanner);
		if (token.line != line) {
			line = token.line;
			printf("%4d ", token.line);
		} else {
			printf("   | ");
		}

		printf("%-20s ", tokenTypeAsString(token.type));
		if (token.type != TOKEN_LINE_END) printf("%.*s", token.length, token.start);
		printf("\n");

		if (token.type == TOKEN_EOF) break;
	}
}