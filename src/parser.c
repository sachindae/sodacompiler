#include <stdio.h>

#include "ast.h"
#include "parser.h"

void parse(const TokenList* tokens) {
	printf("Starting parsing...\n");

	// Print out all tokens
	for (int i = 0; i < tokens->count; i++) {
		printf("Token %d Line (%d): %s\n", i, tokens->tokens[i].line_num, tokens->tokens[i].value);
	}

	Parser parser = {
		.tokens = tokens,
		.cur_pos = 0,
	};



}

Token peek_parser(Parser* parser, unsigned int offset) {
	return parser->tokens->tokens[parser->cur_pos + offset];
}

void consume_parser(Parser* parser) {
	parser->cur_pos++;
}

Statement parse_statement(Parser* parser) {

}

Declaration parse_declaration(Parser* parser) {

}

DeclarationVar parse_var_declaration(Parser* parser) {

}


