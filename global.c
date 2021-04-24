#ifndef __GETGLOBAL_C__
#define __GETGLOBAL_C__
#include "9cc.h"

Token *getToken() { return token; }

void setToken(Token *set_token) { token = set_token; }

char *getUserinput() { return user_input; }
/*
void setUserInput(char *set_user_input) {
	user_input = (char *)calloc(strlen(set_user_input), sizeof(char));
	sprintf(user_input, "%s", set_user_input);
}*/

void setUserInput(char *u_i) { user_input = u_i; }

#endif
