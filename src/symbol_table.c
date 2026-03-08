#include "symbol_table.h"


const char* get_temp_reg(SymbolTable* table, const char* var_name) {
	for (unsigned int i = 0; i < table->count; i++) {
		if (strccmp(var_name, table->entries[i].var_name) == 0) {
			return table->entries[i].cur_temp_reg;
		}
	}
	// Entry not in table
	return NULL;
}

void update_table(SymbolTable* table, const char* var_name, const char* cur_temp_reg) {
	if (get_temp_reg(table, var_name)) {
		for (unsigned int i = 0; i < table->count; i++) {
			if (strccmp(var_name, table->entries[i].var_name) == 0) {
				table->entries[i].cur_temp_reg = cur_temp_reg;
				break;
			}
		}

	} else {
		if (table->count == table->capacity) {
			table->capacity *= 2;
			table->entries = realloc(table->emtries, table->capacity * sizeof(SymbolTableEntry));
		}

		SymbolTableEntry* entry = malloc(sizeof(SymbolTableEntry));
		entry->var_name = var_name;
		entry->cur_temp_reg = cur_temp_reg;
		
		table->entries[table->count] = entry; 
		table->count++;
	}
}

