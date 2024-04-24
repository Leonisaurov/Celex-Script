#include "chunck.h"

Chunck chunk;

#ifdef debug
#include <ctype.h>
#include "symtable.h"
char* translateOpCode(op_code code) {
	switch (code) {
		case OP_PRINT: return "OP_PRINT";
		case OP_GOTOB: return "OP_GOTOB";
		case OP_GOTO: return "OP_GOTO";
		case OP_IF: return "OP_IF";
		case OP_ASIGN: return "OP_ASIGN";
		case OP_DEF: return "OP_DEF";
		case OP_INPUT: return "OP_INPUT";
		case OP_UPB: return "OP_UPB";
		case OP_UPV: return "OP_UPV";
		case OP_UPNIL: return "OP_UPNIL";
		case OP_UPS: return "OP_UPS";
		case OP_UPN: return "OP_UPN";
		case OP_RETURN: return "OP_RETURN";
	}
	return "Unhandle Op Code";
}

uint32_t getUInt(uint32_t pos) {
	return *(uint32_t*)(chunk.op_codes + pos);
}

byte getByte(uint32_t pos) {
	return *(byte*)(chunk.op_codes + pos);
}

void printChunck() {
	uint32_t i = 0;
	while (i < chunk.len) {
		op_code code = chunk.op_codes[i];
		i++;
		printf("Instruction: %s\n", translateOpCode(code));
		switch (code) {
			case OP_UPB: {
					     byte booleano = getByte(i);
					     if (booleano == 1) printf("\tContent: true\n");
					     else printf("\tContent: false\n");
					     i += sizeof(byte);
					     break;
				     }
			case OP_UPN: {
					     uint32_t ref = getUInt(i);
					     printf("\tContent: %u\n", ref);
					     printf("\tRef: %lu\n", getInt64(ref));
					     i += sizeof(uint32_t);
					     break;
				     }
			case OP_UPV:
			case OP_UPS: {
					     uint32_t ref = getUInt(i);
					     printf("\tContent: %u\n", ref);
					     slice string = getSlice(ref);
					     printf("\tRef: %.*s\n", string.length, string.txt);
					     i += sizeof(uint32_t);
					     break;
				     }
			default:
				     printf("Unhandle extra info\n");
				;
		}
	}
}

void printChunckSea(uint32_t start) {
	uint32_t at = start + 40;
	if (at > chunk.size) at = chunk.size - 1;
	printf("===Chunck Sea [%d]===\n", start);
	for (uint32_t i = start; i < at; i++) {
		if (i >= chunk.len) printf("empty, ");
		else {
			if (isprint(chunk.op_codes[i])) printf("'%c', ", chunk.op_codes[i]);
			else printf("%u, ", chunk.op_codes[i]);
		}
		if (i - start + 1 % 10 == 0) printf("\n");
	}
	printf("\n\n");
}
#endif

void initChunck() {
	chunk.op_codes = (byte*) malloc(sizeof(byte) * UINT8_MAX);
	chunk.size = UINT8_MAX;
	chunk.len = 0;
}

void addData(byte *data, uint32_t len) {
	byte try = 0;
	while (chunk.size - chunk.len < len) {
		byte *res = (byte*) realloc(chunk.op_codes, chunk.size * 2);
		if (res != NULL) {
			chunk.op_codes = res;
			chunk.size *= 2;
			continue;
		}
		try++;
		// No message Errorr
		if (try == 10) return;
	}

	for (uint32_t i = 0; i < len; i++) {
		chunk.op_codes[chunk.len] = data[i];
		chunk.len++;
	}
}

void addOpCode(op_code type) {
	byte b = type;
	addData(&b, 1);
}

void addUInt(uint32_t number) {
	addData((byte*)&number, sizeof(uint32_t));
}

void addBytecode(byte code) {
	addData(&code, 1);
}

void freeChunck() {
	free(chunk.op_codes);
}

Chunck getChunck() {
	return chunk;
}
