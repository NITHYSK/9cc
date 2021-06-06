/*
 * 9cc.c
 * TokenやNodeなど、9cc.hで定義された型のグローバル構造体を扱う。
 */
#include "9cc.h"

Token *token;	  //現在着目しているトークン
Node *code[100];  //

Token *getToken() { return token; }
void setToken(Token *set_token) { token = set_token; }
Node **getCode() { return code; }
void setCode(Node *node, int i) { code[i] = node; }

