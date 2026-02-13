#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]){
	TokenList* tokens = lex("hello.soda");
	parse(tokens);
}
