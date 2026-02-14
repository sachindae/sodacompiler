#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "lexer.h"
#include "parser.h"

void parse(const TokenList* tokens) {
	printf("Starting parsing...\n");

	Parser parser = {
		.tokens = tokens,
		.cur_pos = 0,
	};

	ProgramAST ast = {
		.statements = malloc(sizeof(Statement*) * 10),
		.count = 0,
		.capacity = 10, 
	};

	while (parser.cur_pos < parser.tokens->count) {
		Token cur_token = peek_parser(&parser, 0);
		printf("[Main Loop] Token Line (%d): %s\n", cur_token.line_num, cur_token.value);
		
		Statement* statement = parse_statement(&parser);
		add_statement(statement, &ast);
	}

}

Token peek_parser(Parser* parser, unsigned int offset) {
	if (parser->cur_pos + offset >= parser->tokens->count) {
		printf("Parser is finished, likely an error");
	}
	return parser->tokens->tokens[parser->cur_pos + offset];
}

void consume_parser(Parser* parser) {
	parser->cur_pos++;
}

void add_statement(Statement* statement, ProgramAST* ast) {
	if (ast->count == ast->capacity) {
		// Expand array of statements if needed
		ast->capacity *= 2;
		ast->statements = realloc(ast->statements, ast->capacity * sizeof(Statement*));
	}

	ast->statements = &statement;
	ast->count++;
	
	printf("Added statement: %u\n", statement->type);
}

Statement* parse_statement(Parser* parser) {
	printf("Parsing statement...\n");

	Token cur_token = peek_parser(parser, 0);
	consume_parser(parser);
	printf("[Parse Stmt] Token Line (%d): %s\n", cur_token.line_num, cur_token.value);

	// Validate token is a keyword (all statements must being with one)
	if (cur_token.type != KEYWORD) {
		printf("Parse statement failed -- token is not a keyword: %s\n", cur_token.value);
		return NULL;
	}

	// Determine which route to go to
	if (strcmp(cur_token.value, "let") == 0) {
		return parse_var_declaration(parser, &cur_token);
	} else if (strcmp(cur_token.value, "fn") == 0) {
		printf("Appears to be func declaration\n");
	}

}

/**
 * Var declaration is the following:
 * let x: int = 10;
 */
Statement* parse_var_declaration(Parser* parser, Token* let_token) {
	printf("Parsing var declaration...\n");
	
	// Validate identifier comes up
	Token identifier = peek_parser(parser, 0);
	if (identifier.type != IDENTIFIER) {
		printf("[Parse var declaration] Invalid token for identifier (line %u): %s\n", identifier.line_num, identifier.value);
		return NULL;
	}
	consume_parser(parser);
	
	// Validate colon comes up
	Token colon = peek_parser(parser, 0);
	if (identifier.type != COLON) {
		printf("[Parse var declaration] Invalid token for colon (line %u): %s\n", colon.line_num, colon.value);
		return NULL;
	}
	consume_parser(parser);

	// Validate type comes up
	Token type = peek_parser(parser, 0);
	if (type.type != KEYWORD || (strcmp(type.value, "int") == 0 || strcmp(type.value, "float") == 0 || strcmp(type.value, "string") == 0)) {
		printf("[Parse var declaration] Invalid token for type (line %u): %s\n", type.line_num, type.value);
		return NULL;
	}
	consume_parser(parser);
	
	// Validate assignment op comes up
	Token assignment_op = peek_parser(parser, 0);
	if (assignment_op.type != COLON) {
		printf("[Parse var declaration] Invalid token for assignment_op (line %u): %s\n", assignment_op.line_num, assignment_op.value);
		return NULL;
	}
	consume_parser(parser);

	// Validate expression comes up
	Expression expr = parse_expression(parser);

}

Expression* parse_expression(Parser* parser) {
	
}

