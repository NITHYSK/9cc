#ifndef __GETGLOBAL_C__
#define __GETGLOBAL_C__
#include "9cc.h"

Token *getToken() { return token; }

void setToken(Token *set_token) { token = set_token; }

char *getUserinput() { return user_input; }

void setUserInput(char *set_user_input) { user_input = set_user_input; }

#endif
