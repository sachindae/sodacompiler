#include "lexer.h"

#ifndef AST_H
#define AST_H

// Forward declarations
typedef struct Expression Expression;
typedef struct Statement Statement;

typedef struct {
	const char* op;
	unsigned int line_num;
} Operator;

typedef struct {
	const char* id;
	unsigned int line_num;
} Identifier;

typedef struct {
	const char* id;
	const char* type;
	unsigned int line_num;
} FuncParam;

typedef struct {
	Expression* expr1;
	Operator op;
	Expression* expr2;
	unsigned int line_num;
} BinaryExpr;

struct Expression {
	enum { INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL, EXPR_IDENTIFIER, BINARY_EXPR } type;
	union {
		int int_literal;
		float float_literal;
		const char* string_literal;
		Identifier identifier;
		BinaryExpr binary_expr;
	} as;
	unsigned int line_num;
};

typedef struct {
	Identifier identifier;
	Expression* value;
	unsigned int line_num;
} VarDeclaration;

typedef struct {
	Identifier identifier;
	Statement** body;
	unsigned int body_len;
	unsigned int body_capacity;
	unsigned int line_num;
} FuncDeclaration;

struct Statement {
	enum {
		VAR_DECL,
		FUNC_DECL,
	} type;

	union {
		VarDeclaration var_decl;
		FuncDeclaration func_decl;
	} as;
};

typedef struct {
	Statement** statements;
	size_t count;
	size_t capacity;
} ProgramAST;

#endif // AST_H
