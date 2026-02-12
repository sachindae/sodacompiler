#ifndef AST_H
#define AST_H

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
	} as;
} Statement;

typedef struct {

} Program;


#endif // AST_H
