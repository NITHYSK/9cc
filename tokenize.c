#ifndef __TOKENIZE_C__
#define __TOKENIZE_C__

#include "9cc.h"

//入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
	dbg("start");
	Token head;
	head.next = NULL;
	Token *cur = &head;
	int loop = 1;
	while (*p) {
		breakPoint();
		dbg("loop:%d", loop++);
		//空白文字をスキップ
		if (isspace(*p)) {
			p++;
			continue;
		}
		if (strncmp(p, "==", 2) == 0 || strncmp(p, "!=", 2) == 0 ||
		    strncmp(p, "<=", 2) == 0 || strncmp(p, ">=", 2) == 0) {
			dbg();
			cur = new_token(TK_RESERVED, cur, p);
			cur->len = 2;
			p += 2;
			continue;
		}
		if (*p == '>' || *p == '<') {
			dbg();
			cur = new_token(TK_RESERVED, cur, p++);
			cur->len = 1;
			continue;
		}
		if (*p == '(' || *p == ')') {
			dbg();
			cur = new_token(TK_RESERVED, cur, p++);
			cur->len = 1;
			continue;
		}
		if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
			dbg();
			cur = new_token(TK_RESERVED, cur, p++);
			cur->len = 1;
			continue;
		}
		if ('a' <= *p && *p <= 'z') {
			dbg();
			cur = new_token(TK_IDENT, cur, p++);
			cur->len = 1;
			continue;
		}
		if (*p == '=') {
			dbg();
			cur = new_token(TK_RESERVED, cur, p++);
			cur->len = 1;
			continue;
		}
		if (*p == ';') {
			dbg();
			cur = new_token(TK_RESERVED, cur, p++);
			cur->len = 1;
			continue;
		}
		if (isdigit(*p)) {
			dbg();
			cur = new_token(TK_NUM, cur, p);
			char *pbuff = p;
			cur->val = strtol(p, &p, 10);
			cur->len = (int)(pbuff - p);
			continue;
		}

		error_at(p, "トークナイズできません");
		exit(1);
	}

	new_token(TK_EOF, cur, p);
	return head.next;
}

//次のトークンが期待している記号のときには、トークンを１つ読み進めて
//それ以外の場合にはエラーを報告する。
void expect(char *op) {
	dbg();
	if (getToken()->kind != TK_RESERVED || strlen(op) != getToken()->len ||
	    memcmp(getToken()->str, op, getToken()->len))
		error_at(getToken()->str, "'%c'ではありません", op);
	setToken(getToken()->next);
}

//次のトークンが数値の場合、トークンを１つ読み進めて
//それ以外の場合にはエラーを報告する。
int expect_number() {
	if (getToken()->kind != TK_NUM)
		error_at(getToken()->str, "数ではありません");
	int val = getToken()->val;
	dbg("%d", val);
	setToken(getToken()->next);
	return val;
}

//新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str) {
	dbg("kind = %s, str = \"%s\"", TK_TABLE[kind], str);
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	cur->next = tok;
	return tok;
}

bool at_eof() { return getToken()->kind == TK_EOF; }

#endif
