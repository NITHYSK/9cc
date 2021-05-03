#ifndef __PROLOGUE_C__
#define __PROLOGUE_C__
#include "9cc.h"

// プロローグを出力する関数
void prologue() {
	dbg();
	//アセンブリの前半部分を出力
	printf(".intel_syntax noprefix\n");
	printf(".globl main\n");
	printf("main:\n");

	printf("	push rbp\n");
	printf("	mov rbp, rsp\n");
	printf("	sub rsp, 208\n");  //変数26個分の領域を確保
}

#endif

