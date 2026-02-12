#include <stddef.h>

#ifndef LEXER_H 
#define LEXER_H

typedef enum {
	KEYWORD,		// “let”, “while”, “if”, “else”, “int”, “float”, “char”, “fn”
	IDENTIFIER, 		// {a-zA-Z}+{...}
	OPERATOR,		// “+”, “-”, “/”, “*”
	ASSIGNMENT_OP,		// "="
	COMPARISON_OP,		// "==", ">=", ">", "<=", "<'
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
	size_t size;		// Current size
} TokenList;

typedef struct {
	const char* input;	// Source text
	unsigned int input_len;	// Total length of input
	unsigned int cur_pos;	// Current position within input
	unsigned int line_num;	// Line num of token start
} Lexer;

// Public functions
TokenList* lex(const char* file_path); 

// Private functions
const char* read_file(const char* file_path);
const char peek(Lexer* lexer, unsigned int offset);
void consume(Lexer* lexer);
void add_token(Token* token, TokenList* list);

// Token terms
extern const char* KEYWORDS[];
extern const int KEYWORDS_COUNT;
extern const char* TOKEN_TYPE_TO_STR(TokenType type);

#endif // LEXER_H
