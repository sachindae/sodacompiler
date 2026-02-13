#include "lexer.h"

#ifndef AST_H
#define AST_H

typedef enum {
	ARG_INTEGER,
	ARG_FLOAT,
	ARG_STRING,
} ArgType;

typedef struct {
	ArgType type;
	Token identifer;
} Arg;

typedef enum {
	FUNC_PARAM_ARG,
	FUNC_PARAM_MULTI,
} FuncParamsType;

typedef struct {
	FuncParamsType type;
	union {
		struct {
			Arg arg;
		} arg;

		struct {
			Arg arg;
			FuncParamsType* params;
		} params;
	} as;
} FuncParams;

typedef struct {
	Token identifier;
	FuncParams params;
} DeclarationFunc;

typedef enum {
	PRIM_ID,
	PRIM_INT,
	PRIM_FLOAT,
	PRIM_STRING,
} PrimaryType;

typedef struct {
	PrimaryType type;
	union {
		struct {
			Token identifier;
		} id;

		struct {
			Token _int;
		} _int;

		struct {
			Token _float;
		} _float;

		struct {
			Token _string;
		} _string;
	} as;
} Primary;

typedef enum {
	EXPR_PRIMARY,
	EXPR_OP,
} ExpressionType;

typedef struct Expression Expression;

struct Expression {
	ExpressionType type;
	union {
		struct {
			Primary p;
		} primary;

		struct {
			Primary p;
			Token op;
			Expression* expr;
		} op;
	} as;
};

typedef struct {
	Token identifier;
	Token type;
	Expression expr;
} DeclarationVar;

typedef enum {
	DECL_FUNC,
	DECL_VAR,
} DeclarationType;

typedef struct {
	const DeclarationType type;
	union {
		struct {
			DeclarationFunc decl;
		} func_declaration;

		struct {
			DeclarationVar decl;
		} var_declaration;
	} as;
} Declaration;

typedef enum {
	STMT_DECLARATION,
	STMT_IF,
	STMT_WHILE,
	STMT_ASSIGNMENT,
	STMT_FN_CALL,
	STMT_RETURN,
} StatementType;

typedef struct {
	const StatementType type;
	union {
		struct {
			Declaration declaration;
		} declaration;

		/*
		struct {
			StatementIf statement;
		} _if;

		struct {
			StatementWhile statement;
		} _while;

		struct {
			Assignment assignment;
		} assignment;

		struct {
			FnCall fn_call;
		} fn_call;

		struct {
			Return _return;
		} _return;
		*/
	} as;
} Statement;

typedef struct {

} Program;


#endif // AST_H
