#ifndef __EPILOGUE_C__
#define __EPILOGUE_C__
#include "9cc.h"

// エピローグを出力する関数
void epilogue() {
	dbg();
	printf("	mov rsp, rbp\n");
	printf("	pop rbp\n");
	printf("	ret\n");
}

#endif

