#include <stdio.h>
#include "tac.h"

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

