#ifndef CELEX_SCRIPT_SCANNER
#define CELEX_SCRIPT_SCANNER
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define true 1
#define false 0

//#define debug

typedef enum {
  // Numero, String, Booleano, Indentificador
  TOKEN_NUM, TOKEN_STR, TOKEN_BOOL, TOKEN_IDENT,
  // Referencia, Rango
  TOKEN_REF, TOKEN_RANGE,
  // Tipo de dato, Asignacion, Definicion
  TOKEN_TYPE, TOKEN_ASIGN, TOKEN_DEF,
  // +, -, *, /
  TOKEN_ADD, TOKEN_DEC, TOKEN_MULT, TOKEN_DIV,
  // ==, >, <, >=, <=
  TOKEN_EQ, TOKEN_GT, TOKEN_LT, TOKEN_GE, TOKEN_LE,
  // imprimir, leer, retornar
  TOKEN_PRINT, TOKEN_INPUT, TOKEN_RETURN, TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_FOR,
  // Final
  TOKEN_EOF,
  // Error
  TOKEN_ERR,
} TokenType;

typedef struct {
  TokenType type;
  const char* start;
  uint64_t len;
  uint16_t line;
} Token;

#ifdef debug
void printToken(Token token);
#endif
void loadSource(const char* source);
Token getToken();

#endif
