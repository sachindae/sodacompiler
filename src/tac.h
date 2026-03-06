#include "ast.h"

#ifndef TAC_H
#define TAC_H

typedef struct {

} TacCall;

typedef struct {

} TacReturn;

typedef struct {

} TacEndFunc;

typedef struct {

} TacBeginFunc;

typedef struct {

} TacParam;

typedef struct {

} TacLiteral;

typedef struct {

} TacConst;

typedef struct {

} TacOp;

typedef struct {
	enum {
		TAC_OP,
		TAC_CONST,
		TAC_LITERAL,
		TAC_PARAM,
		TAC_BEGINFUNC,
		TAC_ENDFUNC,
		TAC_RETURN,
		TAC_CALL,
	} type;
	
	union {
		TacOp op;
		TacConst _const;
		TacLiteral literal;
		TacParam param;
		TacBeginFunc begin_func;
		TacEndFunc end_func;
		TacReturn _return;
		TacCall call;
	} as;
} Tac;

typedef struct {
	Tac** tac_list;
} ProgramTAC;

// Public functions
void ast_to_tac(ProgramAST* ast);

// Private functions
void print_tac(Tac* tac);

#endif // TAC_H
