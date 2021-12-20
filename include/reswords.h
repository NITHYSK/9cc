#ifndef __RESWORDS_H__
#define __RESWORDS_H__

#include "stdbool.h"

static const char *const reswords[] = 
{
	"void", "char", "short", "int", "long", "float", "double",
	"auto", "static", "const", "signed", "unsigned", "extern", "volatile", "register",
	"return", "goto", "if", "else", "switch", "case", "default", "break", "for", "while", "do", "continue",
	"typedef", "struct", "enum", "union", "sizeof"
}

bool isResWord(char *idname){
	for(int i=0; i<sizeof(reswords)/sizeof(static const char *const)){
	if(strcmp(reswords[i], idname) return true;
	}
	return false;
}

#endif
