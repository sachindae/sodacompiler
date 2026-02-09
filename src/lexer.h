#include <stddef.h>

#ifndef LEXER_H 
#define LEXER_H

typedef enum {
	KEYWORD,		// “let”, “while”, “if”, “else”, “int”, “float”, “char”, “fn”
	IDENTIFIER, 		// {a-zA-Z}+{...}
	OPERATOR,		// “=”, “+”, “-”, “/”, “*”
	INTEGER, 		// “1”, “100”, etc
	FLOAT,			// “3.23”, “100.1”, etc.
	STRING,			// “word”, etc.
	COMMA,			// “,”
	SEMICOLON,		// “;”
	COLON,			// “:”
	LEFTBRACKET,		// “{“
	RIGHTBRACKET,		// “}”
	LEFTPAREN,		// “(“
	RIGHTPAREN,		// “)”
} TokenType;

typedef struct {
	TokenType type;		// Type of token
	const char* value;	// Value of token
	unsigned int line_num;	// Line number of token
} Token;

typedef struct {
	Token* tokens;		// List of tokens
	size_t count;		// Num tokens
} TokenList;

TokenList lex(const char* file_path); 

#endif // LEXER_H
