#include "vm.h"

typedef struct {
	Variable* vars;
	unsigned int length;
	unsigned int size;
} Stack;

Stack stack;

const char *symbols;
const char *instruccions;
unsigned int pointerIns;
unsigned int LenIns;
unsigned int LenSyms;

unsigned int getRef() {
	return *(unsigned int*)(instruccions + pointerIns);
}

void initVM(const char* source) {
	LenSyms = *(unsigned int*)source;
	symbols = source + sizeof(unsigned int);

	LenIns = *(unsigned int*) (symbols + LenSyms);
	instruccions = symbols + LenSyms + sizeof(unsigned int);
	pointerIns = 0;
}

void initStack() {
	stack.vars = (Variable*) malloc(sizeof(Variable) * UINT8_MAX);
	if (stack.vars == NULL) {
		printf("Error al iniciarlizar el Stack\n");
		return;
	}

	stack.length = 0;
	stack.size = UINT8_MAX;
}

void bigStack() {
	Variable* vars = (Variable*) realloc(stack.vars, sizeof(Variable) * stack.size * 1.5);
	if (vars == NULL) {
		printf("Error al hacer el Stack más grande\n");
		return;
	}
	stack.vars = vars;
	stack.size *= 2;
}

void printStack() {
	printf("=====Stack=====\n");
	for (uint32_t i = 0; i < stack.length; i++) {
		Variable var = stack.vars[i];
		switch (var.type) {
			case VAR_STR:
				printf("\tSTR: %s\n", var.value.str);
				break;
			case VAR_NUM:
				printf("\tNUM: %ld\n", var.value.number);
				break;
			default:
				printf("Print no handled for that value type\n");
		}
	}
}

void freeStack() {
	for (uint32_t i = 0; i < stack.length; i++) {
		if (stack.vars[i].type == VAR_STR) free(stack.vars[i].value.str);
	}
	free(stack.vars);
	stack.length = 0;
	stack.size = 0;
}

_Bool reachEnd() {
	return pointerIns >= LenIns;
}

uint32_t getRefChunk() {
	return *(uint32_t*) (instruccions + pointerIns);
}

Variable getStrSym() {
	pointerIns++;
	uint32_t ref = getRefChunk();
	pointerIns += sizeof(uint32_t);

	uint32_t length = *(uint32_t*)(symbols + ref);
	Variable val = {
		.type = VAR_STR,
		.value.str = (char*) malloc(sizeof(char) * (length + 1)),
	};
	memcpy(val.value.str, (char*) (symbols + ref + sizeof(uint32_t)), length);
	val.value.str[length] = '\0';
	pointerIns--;

	return val;
}

Variable getNumSym() {
	pointerIns++;
	uint32_t ref = getRefChunk();
	pointerIns += sizeof(uint32_t);
	Variable val = {
		.type = VAR_NUM,
		.value.number = *(int64_t*) (symbols + ref),
	};
	pointerIns--;
	return val;
}

void Up(Variable var) {
	while (stack.length >= stack.size) {
		bigStack();
	}

	stack.vars[stack.length] = var;
	stack.length++;
}

VM_RESULT execute(const char* source) {
	initVM(source);
	initStack();

	while (!reachEnd()) {
		switch (instruccions[pointerIns]) {
			case OP_UPS:
				Up(getStrSym());
				break;
			case OP_UPN:
				Up(getNumSym());
				break;
			default:
				printf("Instruccion no handled\n");
				break;
		}
		pointerIns++;
	}

	printStack();
	freeStack();
	return VM_SUCCESS;
}
