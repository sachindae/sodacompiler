#include <stdio.h>

#include "lexer.h"

typedef struct {
	const char* input;	// Source text
	size_t input_len;	// Total length of input
	size_t cur_pos;		// Current position within input
	unsigned int line_num;	// Line num of token start
} Lexer;

TokenList lex(const char* file_path) {
	printf("File path: %s", file_path);
}
