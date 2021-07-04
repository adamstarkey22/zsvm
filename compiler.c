#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "scanner.h"

void compile(const char* source) {
	initScanner(source);
	int line = -1;
	for (;;) {
		Token token = scanToken();
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