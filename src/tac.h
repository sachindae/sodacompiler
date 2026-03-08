#include "ast.h"
#include "symbol_table.h"

#ifndef TAC_H
#define TAC_H

typedef struct {
	const char* fn_name;
	unsigned int num_args;
} TacCall;

typedef struct {
	const char* return_reg;	
} TacReturn;

typedef struct {
	
} TacEndFunc;

typedef struct {
	int num_bytes;
} TacBeginFunc;

typedef struct {
	enum { TAC_PARAM_INT, TAC_PARAM_FLOAT, TAC_PARAM_STR, TAC_PARAM_ID } type;
	union {
		int int_val;
		float float_val;
		const char* string_val;
		const char* identifier;
	} as;
} TacParam;

typedef struct {
	const char* label;
} TacFuncLabel;

typedef struct {
	const char* target_reg;
	enum { TAC_CONST_INT, TAC_CONST_FLOAT } type;
	union {
		int int_val;
		float float_val;
	} as;
} TacConst;

typedef struct {
	const char* target_reg;
	const char* src_reg1;
	const char* op;
	const char* src_reg2;
} TacOp;

typedef struct {
	enum {
		TAC_OP,
		TAC_CONST,
		TAC_FUNCLABEL,
		TAC_PARAM,
		TAC_BEGINFUNC,
		TAC_ENDFUNC,
		TAC_RETURN,
		TAC_CALL,
	} type;
	
	union {
		TacOp op;
		TacConst _const;
		TacFuncLabel func_label;
		TacParam param;
		TacBeginFunc begin_func;
		TacEndFunc end_func;
		TacReturn _return;
		TacCall call;
	} as;
} Tac;

typedef struct {
	Tac** tac_list;
	size_t count;
	size_t capacity;
} ProgramTAC;

// Public functions
ProgramTAC* ast_to_tac(ProgramAST* ast);
Tac* statement_to_tac(Statement* statement, SymbolTable* sym_table);

// Private functions
void print_tac(Tac* tac);
Tac* var_decl_tac(VarDeclaration var_decl, SymbolTable* sym_table);
Tac* func_decl_tac(FuncDeclaration func_decl, SymbolTable* sym_table);
Tac* func_call_tac(FuncCall func_call, SymbolTable* sym_table);
Tac* ret_stmt_tac(ReturnStatement return_stmt, SymbolTable* sym_table);

#endif // TAC_H
