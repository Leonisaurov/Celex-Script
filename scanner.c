#include "scanner.h"

typedef struct {
	const char* source;
	uint64_t len;
	char* index;
	char* previus;
	uint16_t line;
} Code;

Code code;

#ifdef debug
char* translateTokenType(TokenType type) {
	switch (type) {
		case TOKEN_ERR: return "TOKEN_ERR";
		case TOKEN_EOF: return "TOKEN_EOF";
		case TOKEN_DEF: return "TOKEN_DEF";
		case TOKEN_ASIGN: return "TOKEN_ASIGN";
		case TOKEN_FOR: return "TOKEN_FOR";
		case TOKEN_WHILE: return "TOKEN_WHILE";
		case TOKEN_ELSE: return "TOKEN_ELSE";
		case TOKEN_IF: return "TOKEN_IF";
		case TOKEN_RETURN: return "TOKEN_RETURN";
		case TOKEN_INPUT: return "TOKEN_INPUT";
		case TOKEN_PRINT: return "TOKEN_PRINT";
		case TOKEN_LE: return "TOKEN_LE";
		case TOKEN_GE: return "TOKEN_GE";
		case TOKEN_LT: return "TOKEN_LT";
		case TOKEN_GT: return "TOKEN_GT";
		case TOKEN_EQ: return "TOKEN_EQ";
		case TOKEN_DIV: return "TOKEN_DIV";
		case TOKEN_MULT: return "TOKEN_MULT";
		case TOKEN_DEC: return "TOKEN_DEC";
		case TOKEN_ADD: return "TOKEN_ADD";
		case TOKEN_TYPE: return "TOKEN_TYPE";
		case TOKEN_RANGE: return "TOKEN_RANGE";
		case TOKEN_REF: return "TOKEN_REF";
		case TOKEN_IDENT: return "TOKEN_IDENT";
		case TOKEN_BOOL: return "TOKEN_BOOL";
		case TOKEN_STR: return "TOKEN_STR";
		case TOKEN_NUM: return "TOKEN_NUM";
	}
	return "No handle Token Type";
}

void printToken(Token token) {
	printf("Token: %s\n\tSegment: \"%.*s\"\n\t%lu:%lu\n\tLine: %hu\n", translateTokenType(token.type), (int)token.len, token.start, (unsigned long) (token.start - code.source), token.len,token.line);
}
#endif

void loadSource(const char* source) {
	code.source = source;
	code.len = strlen(source);
	code.index = (char*) source;
	code.previus = code.index;
	code.line = 0;
}

_Bool isEnd() {
	return (uint64_t) (code.index - code.source) >= code.len;
}

char peek() {
	if (isEnd()) return '\0';
	return *code.index;
}

char advance() {
	char p = peek();
	code.index++;
	return p;
}

_Bool match(char match) {
	advance();
	if (peek() == match)
		return true;
	code.index--;
	return false;
}

void skipWhitespaces() {
	while(true) {
		switch (peek()) {
			case '\n':
				code.line++;
			case ' ':
			case '\t':
				advance();
				break;
			default:
				return;
		}
	}
}

Token makeToken(TokenType type) {
	return (Token) {
		.type = type,
			.line = code.line,
			.start = code.previus,
			.len = (uint64_t)(code.index++ - code.previus),
	};
}

Token makeErr(const char* err) {
	return (Token) {
		.type = TOKEN_ERR,
			.start = err,
			.len = strlen(err),
			.line = code.line,
	};
}

_Bool isNumeric(char n) {
	return n >= '0' && n <= '9';
}

_Bool isUpper(char a) {
	return (a >= 'A' && a <= 'Z');
}

_Bool isAlpha(char s) {
	return isUpper(s) || (s >= 'a' && s <= 'z') || s == '_';
}

_Bool isAlphanumeric(char c) {
	return isNumeric(c) || isAlpha(c);
}

Token getString() {
	code.previus = ++code.index;
	while (!isEnd() && peek() != '"') advance();
	if (isEnd()) return makeErr("El string nunca termina");
	return makeToken(TOKEN_STR);
}

Token getNumber() {
	while (!isEnd() && isNumeric(peek())) advance();
	if (peek() == '.') {
		advance();
		while (!isEnd() && isNumeric(peek())) advance();
	}
	return makeToken(TOKEN_NUM);
}

_Bool compareKeys(const char* key) {
	if (code.index - code.previus != strlen(key)) return false;
	return memcmp(code.index, key, code.index - code.previus);
}

Token matchToken() {
	switch (code.previus[0]) {
		case 'i':
			switch (code.previus[1]) {
				case 'f':
					if (compareKeys("if")) return makeToken(TOKEN_IF);
					break;
				case 'n':
					if (compareKeys("input")) return makeToken(TOKEN_INPUT);
					break;
			}
			break;
		case 'e':
			if (compareKeys("else")) return makeToken(TOKEN_ELSE);
			break;
		case 'w':
			if (compareKeys("while")) return makeToken(TOKEN_WHILE);
			break;
		case 'p':
			if (compareKeys("print")) return makeToken(TOKEN_PRINT);
			break;
		case 'r':
			if (compareKeys("return")) return makeToken(TOKEN_RETURN);
			break;
	}
	return makeToken(TOKEN_IDENT);
}

Token getIdent() {
	char state = 1;
	while (!isEnd() && isAlphanumeric(peek())) {
		if (isAlpha(peek()) && !isUpper(peek())) state = 0;
		else if (isNumeric(peek()) && state == 1) state = 2;
		else if (state == 2 && isAlpha(peek())) state = 0;
		advance();
	}

	if (state == 2)  {
		if (peek() == ':') {
			advance();
			if (!isAlphanumeric(peek())) return makeErr("El rango no tiene limite");
			state = 1;
			while (!isEnd() && isAlphanumeric(peek())) {
				if (isAlpha(peek()) && !isUpper(peek())) state = 0;
				else if (isNumeric(peek()) && state == 1) state = 2;
				else if (state == 2 && isAlpha(peek())) state = 0;
				advance();
			}
			return makeToken(TOKEN_RANGE);
		}
		return makeToken(TOKEN_REF);
	}

	return matchToken();
}

Token getToken() {
	skipWhitespaces();
	if (isEnd())
		return makeToken(TOKEN_EOF);
	code.previus = code.index;
	switch (peek()) {
		case '+': return makeToken(TOKEN_ADD);
		case '-': return makeToken(TOKEN_DEC);
		case '*': return makeToken(TOKEN_MULT);
		case '/': return makeToken(TOKEN_DIV);
		case '>': 
			  if (match('=')) return makeToken(TOKEN_GE);
			  else return makeToken(TOKEN_GT);
			  break;
		case '<': 
			  if (match('=')) return makeToken(TOKEN_LE);
			  else return makeToken(TOKEN_LT);
			  break;
		case '=': 
			  if (match('=')) return makeToken(TOKEN_EQ);
			  else return makeToken(TOKEN_ASIGN);
			  break;
		case '"': return getString();
		default:
			  if (isNumeric(peek())) return getNumber();
			  if (isAlphanumeric(peek())) return getIdent();
			  break;
	}
	code.index++;
	return makeErr("Simbolo no reconocido");
}
