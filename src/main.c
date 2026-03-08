#include <stdio.h>
#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "tac.h"

void print_tokens(TokenList* tokens) {
	for (int i = 0; i < tokens->count; i++) {
		printf("Token %d (%s) Line (%d): %s\n", i, TOKEN_TYPE_TO_STR(tokens->tokens[i].type), tokens->tokens[i].line_num, tokens->tokens[i].value);
	}
}

void print_ast(ProgramAST* ast) {
	for (int i = 0; i < ast->count; i++) {
		print_statement(ast->statements[i]);
	}
}

int main(int argc, char *argv[]){
	TokenList* tokens = lex("hello.soda");
	ProgramAST* ast = parse(tokens);

	print_tokens(tokens);
	print_ast(ast);
	
	ProgramTAC* tac = ast_to_tac(ast);
}
