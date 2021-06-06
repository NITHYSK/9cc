#include "9cc.h"
#include "global.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		error("引数の個数が正しくありません");
		return 1;
	}

	setUserInput(argv[1]);

	//グローバルスコープを初期化
	initGlobalScope();

	//トークナイズしてパースする
	setToken(tokenize(getUserinput()));
	program();
	prologue();

	for (int i = 0; getCode()[i]; i++) {
		gen(getCode()[i]);
		printf("	pop rax\n");
	}

	epilogue();

	for (int i = 0; strcmp(getGlobalScope(i), "") != 0; i++) {
		dbg("globalScope[%d]	= %s", i, getGlobalScope(i));
	}

	return 0;
}

