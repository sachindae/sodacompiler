#include <stdio.h>
#include <stdlib.h>
#include "tac.h"
#include "symbol_table.h"

ProgramTAC* ast_to_tac(ProgramAST* ast) {
	printf("Starting AST to TAC conversion...\n");

	ProgramTAC* tac = malloc(sizeof(ProgramTAC) * 10);
	tac->count = 0;
	tac->capacity = 10;

	SymbolTable* sym_table = malloc(sizeof(SymbolTable));
	sym_table->entries = malloc(sizeof(SymbolTableEntry));
	sym_table->count = 0;
	sym_table->capacity = 1;
	sym_table->temp_reg_count = 0;

	for (unsigned int i = 0; i < ast->count; i++) {
		printf("Converting statement %u to TAC\n", i);
		Statement* cur_statement = ast->statements[i];
		Tac* tac = statement_to_tac(cur_statement, sym_table);
	}
}


Tac* var_decl_tac(VarDeclaration var_decl, SymbolTable* sym_table) {
	printf("Converting var decl to TAC\n");

	Tac* tac = malloc(sizeof(Tac));

	// Get temp registers for expression
	if (var_decl.value->type == BINARY_EXPR) {
		
	} else if (var_decl.value->type == EXPR_IDENTIFIER) {

	} else {
		tac->type = TAC_CONST;
		TacConst tac_const;
		
		if (var_decl.value->type == INT_LITERAL) {
			tac_const = (TacConst) {
				.target_reg = "a",
				.type = TAC_CONST_INT,
				.as.int_val = var_decl.value->as.int_literal
			};
		} else if (var_decl.value->type == FLOAT_LITERAL) {
			tac_const = (TacConst) {
				.target_reg = "a",
				.type = TAC_CONST_FLOAT,
				.as.float_val = var_decl.value->as.float_literal
			};
		} else if (var_decl.value->type == STRING_LITERAL) {
			printf("String var decl not supported yet\n");
			return NULL;
		}
	}

	// Get temp register for identifier
}
Tac* func_decl_tac(FuncDeclaration func_decl, SymbolTable* sym_table) {
	printf("Converting func decl to TAC\n");
}
Tac* func_call_tac(FuncCall func_call, SymbolTable* sym_table) {
	printf("Converting func call to TAC\n");
}
Tac* ret_stmt_tac(ReturnStatement return_stmt, SymbolTable* sym_table) {
	printf("Converting return statement to TAC\n");
}

Tac* statement_to_tac(Statement* statement, SymbolTable* sym_table) {
	switch(statement->type) {
		case VAR_DECL: return var_decl_tac(statement->as.var_decl, sym_table);
		case FUNC_DECL: return func_decl_tac(statement->as.func_decl, sym_table);
		case FUNC_CALL: return func_call_tac(statement->as.func_call, sym_table);
		case RETURN_STMT: return ret_stmt_tac(statement->as.return_stmt, sym_table);
		default: 
			printf("Statement to tac failure\n");
			return NULL;
	}
}


void print_tac(Tac* tac) {
	if (tac->type == TAC_OP) {
		printf("%s = %s %s %s\n", tac->as.op.target_reg, tac->as.op.src_reg1, tac->as.op.op, tac->as.op.src_reg2);
	} else if (tac->type == TAC_CONST) {
		(tac->as._const.type == TAC_CONST_INT) ? 
		printf("%s = %d\n", tac->as._const.target_reg, tac->as._const.as.int_val) :
		printf("%s = %f\n", tac->as._const.target_reg, tac->as._const.as.float_val);
	} else if (tac->type == TAC_FUNCLABEL) {
		printf("%s:\n", tac->as.func_label.label);
	} else if (tac->type == TAC_PARAM) {
		printf("param ");
		if (tac->as.param.type == TAC_PARAM_INT) {
			printf("%d", tac->as.param.as.int_val);
		} else if (tac->as.param.type == TAC_PARAM_FLOAT) {
			printf("%f", tac->as.param.as.float_val);
		} else if (tac->as.param.type == TAC_PARAM_STR) {
			printf("%s", tac->as.param.as.string_val);
		} else if (tac->as.param.type == TAC_PARAM_ID) {
			printf("%s", tac->as.param.as.identifier);
		}
		printf("\n");
	} else if (tac->type == TAC_BEGINFUNC) {
		printf("BeginFunc %d\n", tac->as.begin_func.num_bytes);
	} else if (tac->type == TAC_ENDFUNC) {
		printf("EndFunc\n");
	} else if (tac->type == TAC_RETURN) {
		printf("return %s\n", tac->as._return.return_reg);
	} else if (tac->type == TAC_CALL) {
		printf("call %s, %u\n", tac->as.call.fn_name, tac->as.call.num_args);
	}
}

