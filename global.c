#ifndef __GETGLOBAL_C__
#define __GETGLOBAL_C__
#include "9cc.h"

//カプセル化されたグローバル変数
static char *user_input;  //ユーザーが入力した数式
static Token *token;	  //現在着目しているトークン
static Node *code[100];	  //

//メンバ関数
Token *getToken() { return token; }

void setToken(Token *set_token) { token = set_token; }

char *getUserinput() { return user_input; }
/*
void setUserInput(char *set_user_input) {
	user_input = (char *)calloc(strlen(set_user_input), sizeof(char));
	sprintf(user_input, "%s", set_user_input);
}*/

void setUserInput(char *u_i) { user_input = u_i; }

Node **getCode() { return code; }

void setCode(Node *node, int i) { code[i] = node; }

#endif
