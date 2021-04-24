//#define __DEBUG__

#include "9cc.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		error("引数の個数が正しくありません");
		return 1;
	}

	//グローバル変数に入力された文字列を格納
	user_input = (char *)malloc(strlen(argv[1]));
	sprintf(user_input, "%s", argv[1]);
	setUserInput(user_input);

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

