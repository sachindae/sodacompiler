#include <stdio.h>
#include "tac.h"

void print_tac(Tac* tac) {
	if (tac->type == TAC_OP) {
		printf("TAC_OP");
	} else if (tac->type == TAC_CONST) {
		printf("TAC_CONST");
	} else if (tac->type == TAC_LITERAL) {
		printf("TAC_LITERAL");
	} else if (tac->type == TAC_PARAM) {
		printf("TAC_PARAM");
	} else if (tac->type == TAC_BEGINFUNC) {
		printf("TAC_BEGINFUNC");
	} else if (tac->type == TAC_ENDFUNC) {
		printf("TAC_ENDFUNC");
	} else if (tac->type == TAC_RETURN) {
		printf("TAC_RETURN");
	} else if (tac->type == TAC_CALL) {
		printf("TAC_CALL");
	}
}

