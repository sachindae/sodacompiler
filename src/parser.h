#include "lexer.h"
#include "ast.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct {
	const TokenList* tokens;	// Tokens to be parsed
	unsigned int cur_pos;		// Current token ID 
} Parser;


// Public functions
void parse(const TokenList* tokens);
Token peek_parser(Parser* parser, unsigned int offset);
void consume_parser(Parser* parser);

// Private functions
void add_statement(Statement*, ProgramAST* ast);
Statement* parse_statement(Parser* parser);
Statement* parse_var_declaration(Parser* parser, Token* let_token);
Expression* parse_expression(Parser* parser);

#endif // PARSER_H
