#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ast.h"
#include "lexer.h"
#include "parser.h"

ProgramAST* parse(const TokenList* tokens) {
	printf("Starting parsing...\n");

	Parser parser = {
		.tokens = tokens,
		.cur_pos = 0,
	};

	ProgramAST* ast = malloc(sizeof(ProgramAST));
	ast->statements = malloc(sizeof(Statement*) * 10);
	ast->count = 0;
	ast->capacity = 10; 

	while (parser.cur_pos < parser.tokens->count) {
		Token cur_token = peek_parser(&parser, 0);
		printf("[Main Loop] Token Line (%d): %s\n", cur_token.line_num, cur_token.value);
		
		Statement* statement = parse_statement(&parser);
		add_statement(statement, ast);
	}

	return ast;
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
	} else if (statement->type == 2) {
		printf("Added func call with %zu args\n", statement->as.func_call.arg_count);
	} else if (statement->type == 3) {
		printf("Added return statement\n");
	}
}

Statement* parse_statement(Parser* parser) {
	printf("Parsing statement...\n");

	Token cur_token = peek_parser(parser, 0);
	consume_parser(parser);
	printf("[Parse Stmt] Token Line (%d): %s\n", cur_token.line_num, cur_token.value);

	// Determine which route to go to
	if (strcmp(cur_token.value, "let") == 0) {
		return parse_var_declaration(parser);
	} else if (strcmp(cur_token.value, "fn") == 0) {
		return parse_func_declaration(parser);
	} else if (strcmp(cur_token.value, "return") == 0) {
		return parse_return_statement(parser);
	} else if (cur_token.type == IDENTIFIER) {
		Token next_token = peek_parser(parser, 0);
		consume_parser(parser);
		if (next_token.type == LEFTPAREN) {
			return parse_func_call(parser, &cur_token); 
		} else {
			printf("Parse statement failed -- token is not left paran: %s\n", next_token.value);
			return NULL;
		}
	}

}

/**
 * Var declaration is the following:
 * let x: int = 10;
 */
Statement* parse_var_declaration(Parser* parser) {
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
	var_declaration->type = type.value;
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
Statement* parse_func_declaration(Parser* parser) {
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

/**
 * Example is foo("hello", a);
 */
Statement* parse_func_call(Parser* parser, Token* id_token) {
	printf("Parsing func call...\n");

	// Create function call
	FuncCall* func_call = malloc(sizeof(FuncCall));
	func_call->identifier = (Identifier) {.id = id_token->value, .line_num = id_token->line_num};
	func_call->line_num = id_token->line_num;
	func_call->args = malloc(sizeof(FuncArg));
	func_call->arg_count = 0;
	func_call->arg_capacity = 1;

	// Loop over function args
	while (true) {
		Token arg = peek_parser(parser, 0);
		if (arg.type != IDENTIFIER && arg.type != FLOAT && arg.type != INTEGER && arg.type != STRING) {
			printf("[Parse func call] Invalid token for arg (line %u): %s\n", arg.line_num, arg.value);
			return NULL;
		}
		consume_parser(parser);

		// Create func arg
		FuncArg* func_arg = malloc(sizeof(FuncArg));
		switch (arg.type) {
			case IDENTIFIER: 
				func_arg->type = ARG_ID;
				func_arg->identifier = arg.value;
				break;
			case INTEGER:
				func_arg->type = ARG_INT;
				func_arg->int_val = atoi(arg.value);
				break;
			case FLOAT:
				func_arg->type = ARG_FLOAT;
				func_arg->float_val = atof(arg.value);
				break;
			case STRING: 
				func_arg->type = ARG_STRING;
				func_arg->string_val = arg.value;
				break;
			default:
				printf("Failed parsing func arg: %s\n", arg.value);
		}

		if (func_call->arg_count == func_call->arg_capacity){
			// Expand array of func args if needed
			func_call->arg_capacity *= 2;
			func_call->args = realloc(func_call->args, func_call->arg_capacity*sizeof(FuncArg));
		}
		func_call->args[func_call->arg_count] = func_arg;
		func_call->arg_count++;

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
	
	// Validate semi colon comes up
	Token semi_colon = peek_parser(parser, 0);
	if (semi_colon.type != SEMICOLON) {
		printf("[Parse func call] Invalid token for semi_colon (line %u): %s\n", semi_colon.line_num, semi_colon.value);
		return NULL;
	}
	consume_parser(parser);

	// Create Statement object
	Statement* return_val = malloc(sizeof(Statement));
	return_val->type = FUNC_CALL;
	return_val->as.func_call = *func_call;
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


/**
 * Example is return x + 5;
 */
Statement* parse_return_statement(Parser* parser) {
	printf("Parsing return statement...\n");
	
	// Parse expression
	Expression* return_expr = parse_expression(parser);
	if (return_expr == NULL) {
		printf("Error parsing return stmt expression\n");
		return NULL;
	}

	// Validate semi colon at end
	Token semi_colon = peek_parser(parser, 0);
	if (semi_colon.type != SEMICOLON) {
		printf("[Parse return statement] Invalid token for semi colon (line %u): %s\n", semi_colon.line_num, semi_colon.value);
		return NULL;
	}
	consume_parser(parser);

	ReturnStatement* return_stmt = malloc(sizeof(ReturnStatement));
	return_stmt->line_num = semi_colon.line_num;
	return_stmt->return_expr = return_expr;

	// Create Statement
	Statement* return_val = malloc(sizeof(Statement));
	return_val->type = RETURN_STMT;
	return_val->as.return_stmt = *return_stmt;
	return return_val;
}	

void print_statement(Statement* statement) {
	if (statement->type == VAR_DECL) {
		printf("(Line %u) VarDeclaration(Id=%s, )\n", statement->as.var_decl.line_num, statement->as.var_decl.identifier.id);
	} else if (statement->type == FUNC_DECL) {
		printf("(Line %u) FuncDeclaration(Id=%s, )\n", statement->as.func_decl.line_num, statement->as.func_decl.identifier.id);
	} else if (statement->type == FUNC_CALL) {
		printf("(Line %u) FuncCall(Id=%s, )\n", statement->as.func_call.line_num, statement->as.func_call.identifier.id);
	} else if (statement->type == RETURN_STMT) {
		printf("(Line %u) ReturnStatement(TODO)\n", statement->as.return_stmt.line_num);
	}
}

