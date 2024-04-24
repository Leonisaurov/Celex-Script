#ifndef CELEX_SCRIPT_VM
#define CELEX_SCRIPT_VM

#include <stdio.h>
#include <stdlib.h>
#include "chunck.h"

typedef enum {
	VAR_NUM, NUM_DOUBLE, VAR_STR, VAR_NULO, 
	VAR_BOOL, VAR_REF, VAR_RANGE
} VariableType;

typedef struct {
	VariableType type;
	union {
		char* str;
		int64_t number;
	} value;
} Variable;

typedef enum {
	VM_SUCCESS,
	VM_ERR,
} VM_RESULT;

VM_RESULT execute(const char* source);

#endif
