#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

	ast->statements[ast->count] = statement;
	ast->count++;
	
	printf("Added statement: %u %s\n", statement->type, statement->as.var_decl.identifier.id);
	if (statement->type == 0) {
		printf("Added var declaration\n");
	} else if (statement->type == 1) {
		printf("Added func declaration with %zu params and %zu statements\n", statement->as.func_decl.param_len, statement->as.func_decl.body_len);
	}
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
		return parse_func_declaration(parser, &cur_token);
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
	if (colon.type != COLON) {
		printf("[Parse var declaration] Invalid token for colon (line %u): %s %u\n", colon.line_num, colon.value, colon.type);
		return NULL;
	}
	consume_parser(parser);

	// Validate type comes up
	Token type = peek_parser(parser, 0);
	if (type.type != KEYWORD || (strcmp(type.value, "int") != 0 && strcmp(type.value, "float") != 0 && strcmp(type.value, "string") != 0)) {
		printf("[Parse var declaration] Invalid token for type (line %u): %s\n", type.line_num, type.value);
		return NULL;
	}
	consume_parser(parser);
	
	// Validate assignment op comes up
	Token assignment_op = peek_parser(parser, 0);
	if (assignment_op.type != ASSIGNMENT_OP) {
		printf("[Parse var declaration] Invalid token for assignment_op (line %u): %s\n", assignment_op.line_num, assignment_op.value);
		return NULL;
	}
	consume_parser(parser);

	// Validate expression comes up
	Expression* expr = parse_expression(parser);
	
	// Validate semi colon comes up
	Token semi_colon = peek_parser(parser, 0);
	if (semi_colon.type != SEMICOLON) {
		printf("[Parse var declaration] Invalid token for semi colon (line %u): %s\n", semi_colon.line_num, semi_colon.value);
		return NULL;
	}
	consume_parser(parser);
	
	VarDeclaration* var_declaration = malloc(sizeof(VarDeclaration));
	var_declaration->identifier = (Identifier) {.id = identifier.value, .line_num = identifier.line_num};
	var_declaration->value = expr;
	var_declaration->line_num = var_declaration->identifier.line_num; 

	// Create Statement object
	Statement* return_val = malloc(sizeof(Statement));
	return_val->type = VAR_DECL;
	return_val->as.var_decl = *var_declaration;
	return return_val;
}

/**
 * Example is fn add(a: int, b: int): int { }
 */
Statement* parse_func_declaration(Parser* parser, Token* func_token) {
	printf("Parsing func declaration...\n");
	
	// Validate identifier comes up
	Token identifier = peek_parser(parser, 0);
	if (identifier.type != IDENTIFIER) {
		printf("[Parse func declaration] Invalid token for identifier (line %u): %s\n", identifier.line_num, identifier.value);
		return NULL;
	}
	consume_parser(parser);
	
	// Validate left_paren comes up
	Token left_paren = peek_parser(parser, 0);
	if (left_paren.type != LEFTPAREN) {
		printf("[Parse func declaration] Invalid token for left paren (line %u): %s %u\n", left_paren.line_num, left_paren.value, left_paren.type);
		return NULL;
	}
	consume_parser(parser);

	// Create function declaration
	FuncDeclaration* func_declaration = malloc(sizeof(FuncDeclaration));
	func_declaration->params = malloc(sizeof(FuncParam));
	func_declaration->body = malloc(sizeof(Statement));
	func_declaration->param_len = 0;
	func_declaration->param_capacity = 1;
	func_declaration->body_len = 0;
	func_declaration->param_capacity = 1;
	func_declaration->identifier = (Identifier) {.id = identifier.value, .line_num = identifier.line_num};
	func_declaration->line_num = func_declaration->identifier.line_num; 

	// Func param loop
	while (true) {
		printf("Starting id loop\n");

		// Validate identifier comes up
		Token loop_identifier = peek_parser(parser, 0);
		if (loop_identifier.type != IDENTIFIER) {
			printf("[Parse func declaration] Invalid token for loop_identifier (line %u): %s\n", loop_identifier.line_num, loop_identifier.value);
			return NULL;
		}
		consume_parser(parser);

		// Validate colon comes up
		Token loop_colon = peek_parser(parser, 0);
		if (loop_colon.type != COLON) {
			printf("[Parse func declaration] Invalid token for loop_colon (line %u): %s\n", loop_colon.line_num, loop_colon.value);
			return NULL;
		}
		consume_parser(parser);
		
		// Validate type comes up
		Token loop_type = peek_parser(parser, 0);
		if (loop_type.type != KEYWORD || (strcmp(loop_type.value, "int") != 0 && strcmp(loop_type.value, "float") != 0 && strcmp(loop_type.value, "string") != 0)) {
			printf("[Parse func declaration] Invalid token for loop_type (line %u): %s\n", loop_type.line_num, loop_type.value);
			return NULL;
		}
		consume_parser(parser);

		FuncParam* param = malloc(sizeof(FuncParam));
		param->id = loop_identifier.value;
		param->type = loop_type.value;
		param->line_num = loop_identifier.line_num;

		// TODO: Add identifier to a list of fn params
		if (func_declaration->param_len == func_declaration->param_capacity){
			// Expand array of func params if needed
			func_declaration->param_capacity *= 2;
			func_declaration->params = realloc(func_declaration->params, func_declaration->param_capacity*sizeof(FuncParam));
		}
		func_declaration->params[func_declaration->param_len] = param;
		func_declaration->param_len++;
		
		// Check for right paren (end of param list) 
		Token loop_terminal = peek_parser(parser, 0);
		if (loop_terminal.type == COMMA) {
			consume_parser(parser);
		} else if (loop_terminal.type == RIGHTPAREN) {
			consume_parser(parser);
			break;
		} else {
			printf("[Parse func declaration] Invalid token for loop_terminal (line %u): %s\n", loop_terminal.line_num, loop_terminal.value);
			return NULL;
		}
	}

	// Validate colon comes up
	Token colon = peek_parser(parser, 0);
	if (colon.type != COLON) {
		printf("[Parse func declaration] Invalid token for colon (line %u): %s\n", colon.line_num, colon.value);
		return NULL;
	}
	consume_parser(parser);

	// Validate return type comes up
	Token return_type = peek_parser(parser, 0);
	if (return_type.type != KEYWORD || (strcmp(return_type.value, "int") != 0 && strcmp(return_type.value, "float") != 0 && strcmp(return_type.value, "string") != 0)) {
		printf("[Parse func declaration] Invalid token for return_type (line %u): %s\n", return_type.line_num, return_type.value);
		return NULL;
	}
	consume_parser(parser);

	// Validate left bracket { comes up
	Token left_bracket = peek_parser(parser, 0);
	if (left_bracket.type != LEFTBRACKET) {
		printf("[Parse func declaration] Invalid token for left_bracket (line %u): %s\n", left_bracket.line_num, left_bracket.value);
		return NULL;
	}
	consume_parser(parser);

	func_declaration->body = malloc(sizeof(Statement));
	func_declaration->body_len = 0;
	func_declaration->body_capacity = 1;

	// TODO: Parse list of statements that may come up
	while (true) {
		
		Statement* body_statement = parse_statement(parser);
		if (func_declaration->body_len == func_declaration->body_capacity){
			// Expand array of body statements if needed
			func_declaration->body_capacity *= 2;
			func_declaration->body = realloc(func_declaration->body, func_declaration->body_capacity*sizeof(Statement));
		}
		func_declaration->body[func_declaration->body_len] = body_statement;
		func_declaration->body_len++;

		// Check for right bracket (indicates end of statement)
		Token loop_terminal = peek_parser(parser, 0);
		if (loop_terminal.type == RIGHTBRACKET) {
			consume_parser(parser);
			break;
		}
	}

	// Create Statement object
	Statement* return_val = malloc(sizeof(Statement));
	return_val->type = FUNC_DECL;
	return_val->as.func_decl = *func_declaration;
	return return_val;
}

// Expression can be literal, identifier or binaryexpr(expr, op, expr)
Expression* parse_expression(Parser* parser) {
	printf("Parssing expression fn start...\n");
	Expression* left = malloc(sizeof(Expression));
	Token left_token = peek_parser(parser, 0);
	if (left_token.type == IDENTIFIER) {
		printf("Parssing expression found identifier...\n");
		left->type = EXPR_IDENTIFIER;
		left->as.identifier.id = left_token.value;
		left->as.identifier.line_num = left_token.line_num;
	} else if (left_token.type == INTEGER) {
		printf("Parssing expression found integer...\n");
		left->type = INT_LITERAL;
		left->as.int_literal = atoi(left_token.value); 
	} else if (left_token.type == FLOAT) {
		printf("Parssing expression found float...\n");
		left->type = FLOAT_LITERAL;
		left->as.float_literal = atof(left_token.value);
	} else if (left_token.type == STRING) {
		printf("Parssing expression found string..\n");
		left->type = STRING_LITERAL;
		left->as.string_literal = left_token.value;
	} else {
		printf("Invalid parse expression for token: %s\n", left_token.value);
		return NULL;
	}
	consume_parser(parser);

	// Parse op
	Token maybe_op = peek_parser(parser, 0);
	if (maybe_op.type != OPERATOR && maybe_op.type != COMPARISON_OP) {
		printf("No op in expression, parse finished\n");
		return left;
	}
	consume_parser(parser);

	Expression* right = malloc(sizeof(Expression));
	right = parse_expression(parser);

	// Create Binary op
	Expression* binary_op = malloc(sizeof(Expression));
	binary_op->type = BINARY_EXPR;
	binary_op->as.binary_expr = (BinaryExpr) {
		.expr1 = left,
		.op = (Operator) {
			.op = maybe_op.value,
			.line_num = maybe_op.line_num
		},
		.expr2 = right
	};
	printf("Created a binary op with op: %s\n", binary_op->as.binary_expr.op.op);

	return binary_op;
}

// Expression can be literal, identifier or binaryexpr(expr, op, expr)
Expression* parse_expression_op(Parser* parser) {
	printf("Parssing expression fn start...\n");
	Expression* left = malloc(sizeof(Expression));
	Token left_token = peek_parser(parser, 0);
	if (left_token.type == IDENTIFIER) {
		printf("Parssing expression found identifier...\n");
		left->type = EXPR_IDENTIFIER;
		left->as.identifier.id = left_token.value;
		left->as.identifier.line_num = left_token.line_num;
	} else if (left_token.type == INTEGER) {
		printf("Parssing expression found integer...\n");
		left->type = INT_LITERAL;
		left->as.int_literal = atoi(left_token.value); 
	} else if (left_token.type == FLOAT) {
		printf("Parssing expression found float...\n");
		left->type = FLOAT_LITERAL;
		left->as.float_literal = atof(left_token.value);
	} else if (left_token.type == STRING) {
		printf("Parssing expression found string..\n");
		left->type = STRING_LITERAL;
		left->as.string_literal = left_token.value;
	} else {
		printf("Invalid parse expression for token: %s\n", left_token.value);
		return NULL;
	}
	consume_parser(parser);

	// TODO: Parse op
	//

	return left;
}


