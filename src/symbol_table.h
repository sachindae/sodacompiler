#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string.h>

typedef struct {
	const char* var_name;
	// Var's current temp register can change
	const char* cur_temp_reg;
} SymbolTableEntry;

typedef struct {
	SymbolTableEntry** entries;
	size_t count;
	size_t capacity;
	size_t temp_reg_count;
} SymbolTable;

// Public function
const char* get_temp_reg(SymbolTable* table, const char* var_name);
void update_table(SymbolTable* table, const char* var_name, const char* cur_temp_reg);

#endif // SYMBOL_TABLE_H
