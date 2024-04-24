#include "compiler.h"

void addContent(byte* out, byte* content, uint32_t length) {
	for (uint32_t i = 0; i < length; i++) {
		out[i] = content[i];
	}
}

COMPILER_RESULT compile(const char* source, byte** out, uint64_t *len) {
	//loadSource(source);
	initChunck();
	initSymTable();

	// Action
	char msg[] = "Hola, como estas?";
	addBytecode(OP_UPS);
	addUInt(addString(msg, strlen(msg)));

	addBytecode(OP_UPN);
	addUInt(addInt64("4758", 4));

	printChunck();
	printChunckSea(0);
	printSymsea(0);
	printSymtable();

	Chunck inst = getChunck();
	SymTable symbols = getSymbols();

	uint64_t length = inst.len + symbols.length + sizeof(uint32_t) * 2;

	*out = (char*) malloc(sizeof(byte) * length);
	if (*out == NULL) {
		printf("Error al juntar el compilado final\n");
		freeSymTable();
		freeChunck();
		return COMPILER_RESULT_ERR;
	}
	char *outM = *out;
	*len = length;

	addContent(outM, (byte*)&symbols.length, sizeof(uint32_t));
	outM += sizeof(uint32_t);

	addContent(outM, symbols.data, symbols.length);
	outM += symbols.length;

	addContent(outM, (byte*)&inst.len, sizeof(uint32_t));
	outM += sizeof(uint32_t);

	addContent(outM, inst.op_codes, inst.len);
	outM += inst.len; 

	freeSymTable();
	freeChunck();
	return COMPILER_RESULT_OK;
}
