#include "lexer.h"
#include "ast.h"
#include "parser.h"

int main(int argc, char *argv[]){
	TokenList* tokens = lex("hello.soda");
	ProgramAST* ast = parse(tokens);
}
