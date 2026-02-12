#include <lexer.h>
#include <ast.h>

#ifndef PARSER_H
#define PARSER_H

typedef struct {
	const TokenList* tokens;	// Tokens to be parsed
	unsigned int cur_pos;		// Current token ID 
} Parser;


// Public functions
TODO parse(const TokenList* tokens);


#endif // PARSER_H
