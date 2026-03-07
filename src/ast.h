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
	enum  {
		ARG_ID,
		ARG_FLOAT,
		ARG_INT,
		ARG_STRING,
	} type;
	union {
		const char* identifier;
		int int_val;
		float float_val;
		const char* string_val;
	};
	unsigned int line_num;
} FuncArg;

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
	Expression* return_expr;
	unsigned int line_num;
} ReturnStatement;

typedef struct {
	Identifier identifier;
	FuncArg** args;
	size_t arg_count;
	size_t arg_capacity;
	unsigned int line_num;
} FuncCall;

typedef struct {
	Identifier identifier;
	const char* type;
	Expression* value;
	unsigned int line_num;
} VarDeclaration;

typedef struct {
	Identifier identifier;
	FuncParam** params;
	size_t param_len;
	size_t param_capacity;
	Statement** body;
	size_t body_len;
	size_t body_capacity;
	unsigned int line_num;
} FuncDeclaration;

struct Statement {
	enum {
		VAR_DECL,
		FUNC_DECL,
		FUNC_CALL,
		RETURN_STMT,
	} type;

	union {
		VarDeclaration var_decl;
		FuncDeclaration func_decl;
		FuncCall func_call;
		ReturnStatement return_stmt;
	} as;
};

typedef struct {
	Statement** statements;
	size_t count;
	size_t capacity;
} ProgramAST;

#endif // AST_H
