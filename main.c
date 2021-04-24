

#include "9cc.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		error("引数の個数が正しくありません");
		return 1;
	}

	//グローバル変数に入力された文字列を格納
	char *u_i = calloc(strlen(argv[1]) + 1, sizeof(char));
	strcpy(u_i, argv[1]);
	setUserInput(u_i);
	dbg("argv[1] = \"%s\"", argv[1]);
	dbg("u_i = \"%s\"", u_i);

	//トークナイズしてパースする
	setToken(tokenize(getUserinput()));
	dbg("Tokenized");
#ifdef __DEBUG__
	printf("Token->str = \"%s\"\n", getToken()->str);
#endif
	Node *node = expr();

	//アセンブリの前半部分を出力
	printf(".intel_syntax noprefix\n");
	printf(".globl main\n");
	printf("main:\n");

	// 抽象構文木を下りながらコードを生成
	gen(node);

	// スタックトップをraxにロードし、返り値とする
	printf("	pop rax\n");
	printf("	ret\n");

	return 0;
}

