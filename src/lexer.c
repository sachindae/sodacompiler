#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "lexer.h"

TokenList* lex(const char* file_path) {
	printf("Lexing file: %s\n", file_path);
	
	const char* input = read_file(file_path);
	Lexer lexer = {
		.input = input,
		.input_len = strlen(input),
		.cur_pos = 0,
		.line_num = 0,
	};

	TokenList* tokens = (TokenList*) malloc(sizeof(TokenList));
	*tokens = (TokenList){
		.tokens = (Token*) malloc(sizeof(Token) * 10),
		.count = 0,
		.size = 10,
	};

	// Split to tokens
	while (lexer.cur_pos < lexer.input_len) {
		const char start_pos = lexer.cur_pos;
		const char cur_char = peek(&lexer, 0);
		if (isalpha(cur_char)) {
			// Read till hit non-alphabet or digit
			consume(&lexer);
			char end_pos = start_pos + 1;
			while(isalpha(peek(&lexer, 0)) || isdigit(peek(&lexer, 0))) {
				consume(&lexer);
				end_pos++;
			}

			char* token_val = (char *) malloc(end_pos - start_pos + 1);
			strncpy(token_val, &lexer.input[start_pos], end_pos - start_pos);
			token_val[end_pos - start_pos] = '\0';
			Token* token = NULL;
			if (token == NULL) {
				// Check if KEYWORD
				for (int i = 0; i < KEYWORDS_COUNT; i++) {
					if (strcmp(token_val, KEYWORDS[i] )== 0){
						// Token is a KEYWORD
						token = (Token *) malloc(sizeof(Token));
						*token = (Token){KEYWORD, token_val, lexer.line_num};
						break;
					}
				}
			}
			if (token == NULL) {
				// Token must be IDENTIFIER
				token = (Token *) malloc(sizeof(Token));
				*token = (Token){IDENTIFIER, token_val, lexer.line_num};
			}

			// Add token to list
			add_token(token, tokens);
		} else if (isspace(peek(&lexer, 0))) {
			// TODO
			if (peek(&lexer, 0) == '\n') {
				lexer.line_num++;
			}
			consume(&lexer);
		} else if (peek(&lexer, 0) == ':') {
			consume(&lexer);
			if (peek(&lexer, 0) != ' ') {
				perror("TODO: Better colon error");
				return NULL;
			}
			const char* token_val = ":";
			Token* token = (Token *) malloc(sizeof(Token));
			*token = (Token){COLON, token_val, lexer.line_num};
			
			// Add token to list
			add_token(token, tokens);	
		} else if (peek(&lexer, 0) == '(') {
			consume(&lexer);
			const char* token_val = "(";
			Token* token = (Token *) malloc(sizeof(Token));
			*token = (Token){LEFTPAREN, token_val, lexer.line_num};
			
			// Add token to list
			add_token(token, tokens);
		} else if (peek(&lexer, 0) == ')') {
			consume(&lexer);
			const char* token_val = ")";
			Token* token = (Token *) malloc(sizeof(Token));
			*token = (Token){RIGHTPAREN, token_val, lexer.line_num};
			
			// Add token to list
			add_token(token, tokens);
		} else if (peek(&lexer, 0) == '=') {
			consume(&lexer);

			bool is_comparison_op = false;
			if (peek(&lexer, 0) == '=') {
				consume(&lexer);
				is_comparison_op = true;
			}

			if (!isspace(peek(&lexer, 0))) {
				perror("Invalid token after equal operator");
				return NULL;
			}
			
			char* token_val;
			Token* token = (Token *) malloc(sizeof(Token));
			if (is_comparison_op){
				token_val = "==";
				*token = (Token){COMPARISON_OP, token_val, lexer.line_num};
			} else {
				token_val = "=";
				*token = (Token){ASSIGNMENT_OP, token_val, lexer.line_num};
			}


			// Add token to list
			add_token(token, tokens);	
		} else if (peek(&lexer, 0) == '+' || peek(&lexer, 0) == '-' || peek(&lexer, 0) == '/' || peek(&lexer, 0) == '*') {
			char* token_val = (char *) malloc(2);
			token_val[0] = peek(&lexer, 0);
			token_val[1] = '\0';
			consume(&lexer);
			Token* token = (Token *) malloc(sizeof(Token));
			*token = (Token){OPERATOR, token_val, lexer.line_num};

			if (!isspace(peek(&lexer, 0))) {
				perror("Invalid token after operator");
				return NULL;
			}

			// Add token to list
			add_token(token, tokens);
		} else if (isdigit(peek(&lexer, 0))) {
			
			// Read till hit non-digit
			consume(&lexer);
			char end_pos = start_pos + 1;
			while(isdigit(peek(&lexer, 0))) {
				consume(&lexer);
				end_pos++;
			}
			
			// Validate no letter after the digit
			if (isalpha(peek(&lexer, 0))) {
				perror("TODO: Invalid alpha after digit");
				return NULL;
			}

			char* token_val = (char *) malloc(end_pos - start_pos + 1);
			strncpy(token_val, &lexer.input[start_pos], end_pos - start_pos);
			token_val[end_pos - start_pos] = '\0';
			Token* token = (Token *) malloc(sizeof(Token));
			*token = (Token){INTEGER, token_val, lexer.line_num};

			// Add token to list
			add_token(token, tokens);
		} else if (peek(&lexer, 0) == ';') {
			consume(&lexer);
			if (peek(&lexer, 0) != '\n') {
				perror("TODO: Better semicolon end line error");
				return NULL;
			}
			const char* token_val = ";";
			Token* token = (Token *) malloc(sizeof(Token));
			*token = (Token){SEMICOLON, token_val, lexer.line_num};
			
			// Add token to list
			add_token(token, tokens);
		} else {
			perror("TODO: Invalid token");
			return NULL;
		}
	}

	// Print out all tokens
	for (int i = 0; i < tokens->count; i++) {
		printf("Token %d (%s) Line (%d): %s\n", i, TOKEN_TYPE_TO_STR(tokens->tokens[i].type), tokens->tokens[i].line_num, tokens->tokens[i].value);
	}
}

const char peek(Lexer* lexer, unsigned int offset) {
	return lexer->input[lexer->cur_pos + offset];
}

void consume(Lexer* lexer) {
	lexer->cur_pos++;
}

void add_token(Token* token, TokenList* list) {
	if (list->count == list->size) {
		// Expand array of tokens if needed
		list->size *= 2;
		list->tokens = realloc(list->tokens, list->size * sizeof(Token));
	}
	list->tokens[list->count] = *token;
	list->count++;
	
	printf("Added token: %s\n", token->value);
}

const char* read_file(const char* file_path) {
	// Open file
	FILE* file = fopen(file_path, "rb");
	if (file == NULL) {
		perror("Error opening file");
		return NULL;
	}

	// Seek to the end of the file to determine the size
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	if (file_size == -1) {
		perror("Error getting file size");
		fclose(file);
		return NULL;
	}
	fseek(file, 0, SEEK_SET);

	// Allocate memory for loading the full file into memory
	char* buffer = (char*) malloc(file_size + 1);
	if (buffer == NULL) {
		perror("Error allocating memory to load file");
		fclose(file);
		return NULL;
	}

	// Read file into buffer
	size_t bytes_read = fread(buffer, 1, file_size, file);
	if (bytes_read != file_size) {
		perror("Error reading file");
		fclose(file);
		free(buffer);
		return NULL;
	}
	buffer[file_size] = '\0';

	fclose(file);

	printf("Read file: %s\n", buffer);

	return (const char *) buffer;
}

// Token things
const char* KEYWORDS[] = {"let", "while", "if", "else", "int", "float", "char", "fn"};
const int KEYWORDS_COUNT = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);

const char* TOKEN_TYPE_TO_STR(TokenType type) {
    switch(type) {
        case KEYWORD:      return "KEYWORD";
        case IDENTIFIER:   return "IDENTIFIER";
        case OPERATOR:     return "OPERATOR";
	case ASSIGNMENT_OP: return "ASSIGNMENT_OP";
	case COMPARISON_OP: return "COMPARISON_OP";
        case INTEGER:      return "INTEGER";
        case FLOAT:        return "FLOAT";
        case STRING:       return "STRING";
        case COMMA:        return "COMMA";
        case SEMICOLON:    return "SEMICOLON";
        case COLON:        return "COLON";
        case LEFTBRACKET:  return "LEFTBRACKET";
        case RIGHTBRACKET: return "RIGHTBRACKET";
        case LEFTPAREN:    return "LEFTPAREN";
        case RIGHTPAREN:   return "RIGHTPAREN";
        default:           return "UNKNOWN";
    }
}
