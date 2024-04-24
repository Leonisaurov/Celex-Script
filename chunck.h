#ifndef CELEX_SCRIPT_CHUNCK
#define CELEX_SCRIPT_CHUNCK
#include <stdlib.h>

#define debug

typedef char byte;

typedef struct {
	byte *op_codes;
	uint32_t len;
	uint32_t size;
} Chunck;

typedef enum {
	OP_UPN, OP_UPS, OP_UPB, OP_UPV, OP_UPNIL,
	OP_PRINT, OP_INPUT,
	OP_DEF, OP_ASIGN,
	OP_IF, OP_GOTO, OP_GOTOB,
	OP_RETURN
} op_code;

void addOpCode(op_code type);
void addUInt(uint32_t number);
void addBytecode(byte code);

void addData(byte *data, uint32_t len);

Chunck getChunck();

void initChunck();
void freeChunck();

#ifdef debug
void printChunck();
void printChunckSea(uint32_t start);
#endif

#endif
