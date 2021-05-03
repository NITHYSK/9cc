#include "9cc.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		error("引数の個数が正しくありません");
		return 1;
	}

	//グローバル変数に入力された文字列を格納
	/*
	char *u_i = calloc(strlen(argv[1]) + 1, sizeof(char));
	strcpy(u_i, argv[1]);
	setUserInput(u_i);
	dbg("argv[1] = \"%s\"", argv[1]);
	dbg("u_i = \"%s\"", u_i);
	*/
	setUserInput(argv[1]);

	//トークナイズしてパースする
	setToken(tokenize(getUserinput()));
	program();
	prologue();

	for (int i = 0; getCode()[i]; i++) {
		gen(getCode()[i]);
		printf("	pop rax\n");
	}

	epilogue();
	return 0;
}

