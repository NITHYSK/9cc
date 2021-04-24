//#define __DEBUG__
#ifdef __DEBUG__
#define dbg(...)                                                               \
	(printf("%d	%s", __LINE__, __func__), printf("	"__VA_ARGS__), \
	 printf("\n"))
#else
#define dbg(...)
#endif

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//トークンの種類
typedef enum {
	TK_RESERVED,  //記号
	TK_NUM,	      //整数トークン
	TK_EOF,	      //入力の終わりを表すトークン
} TokenKind;

static const char *const TK_TABLE[] = {
    "TK_RESERVED",
    "TK_NUM",
    "TK_EOF",
};

typedef struct Token Token;

//トークン型
struct Token {
	TokenKind kind;	 //トークンの型
	Token *next;	 //次の入力トークン
	int val;	 // kindがTK_NUMの場合、その数値
	char *str;	 //トークン文字列
	int len;	 //トークンの長さ
};

//現在着目しているトークン
Token *token;

// 抽象構文木のノードの種類
typedef enum {
	ND_ADD,	 // +
	ND_SUB,	 // -
	ND_MUL,	 // *
	ND_DIV,	 // /
	ND_NUM,	 // 整数
	ND_EQL,	 // ==
	ND_NEQ,	 // !=
	ND_GOE,	 // <=
	ND_LOE,	 // >=
	ND_GTR,	 // <
	ND_LES,	 // >
} NodeKind;

static const char *const ND_TABLE[] = {
    "ND_ADD",  // +
    "ND_SUB",  // -
    "ND_MUL",  // *
    "ND_DIV",  // /
    "ND_NUM",  // 整数
    "ND_EQL",  // ==
    "ND_NEQ",  // !=
    "ND_GOE",  // <=
    "ND_LOE",  // >=
    "ND_GTR",  // <
    "ND_LES",  // >
};

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
	NodeKind kind;	// ノードの型
	Node *lhs;	// 左辺
	Node *rhs;	// 右辺
	int val;	// kindがND_NUMの場合のみ使う
};

// prototype declarations
bool consume(char *);
void expect(char *);
int expect_number();
void gen(Node *);
Node *add();
Node *expr();
Node *primary();
Node *new_node(NodeKind, Node *, Node *);
Node *new_node_num(int);
Node *unary();
Node *equality();
Node *relational();

//ユーザーが入力した数式
char *user_input;

void gen(Node *node) {
	dbg("kind = %s", ND_TABLE[node->kind]);
	if (node->kind == ND_NUM) {
		printf("	push %d\n", node->val);
		return;
	}

	dbg("call recursive");
	gen(node->lhs);
	gen(node->rhs);

	printf("	pop rdi\n");
	printf("	pop rax\n");

	switch (node->kind) {
		case ND_EQL:
			printf("	cmp rax, rdi\n");
			printf("	sete al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_NEQ:
			printf("	cmp rax, rdi\n");
			printf("	setne al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_GOE:
			printf("	cmp rax, rdi\n");
			printf("	setle al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_GTR:
			printf("	cmp rax, rdi\n");
			printf("	setl al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_LOE:
			printf("	cmp rdi, rax\n");
			printf("	setle al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_LES:
			printf("	cmp rdi, rax\n");
			printf("	setl al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_ADD:
			printf("	add rax, rdi\n");
			break;
		case ND_SUB:
			printf("	sub rax, rdi\n");
			break;
		case ND_MUL:
			printf("	imul rax, rdi\n");
			break;
		case ND_DIV:
			printf("	cqo\n");
			printf("	idiv rdi\n");
			break;
	}

	printf("	push rax\n");
}

Node *equality() {
	Node *node = relational();

	for (;;) {
		if (consume("==")) {
			dbg("ND_EQL");
			node = new_node(ND_EQL, node, relational());
		} else if (consume("!=")) {
			dbg("ND_NEQ");
			node = new_node(ND_NEQ, node, relational());
		} else {
			dbg("ELSE");
			return node;
		}
	}
}

Node *relational() {
	dbg();
	Node *node = add();

	for (;;) {
		if (consume("<="))
			node = new_node(ND_GOE, node, add());
		else if (consume(">="))
			node = new_node(ND_LOE, node, add());
		else if (consume("<"))
			node = new_node(ND_GTR, node, add());
		else if (consume(">"))
			node = new_node(ND_LES, node, add());
		else
			return node;
	}
}

// 単項演算子のノードを作成する関数
Node *unary() {
	dbg();
	if (consume("+")) return primary();
	if (consume("-")) return new_node(ND_SUB, new_node_num(0), primary());
	return primary();
}

// 二項演算子のノードを作成する関数
Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
	dbg();
	Node *node = calloc(1, sizeof(Node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

// 数値のノードを作成する関数
Node *new_node_num(int val) {
	dbg();
	Node *node = calloc(1, sizeof(Node));
	node->kind = ND_NUM;
	node->val = val;
}

// primary = num | "(" expr ")"
Node *primary() {
	// 次のトークンが"("なら、"(" expr ")" のはず
	if (consume("(")) {
		dbg("expr");
		Node *node = expr();
		expect(")");
		return node;
	}

	// そうでなければ数値のはず
	dbg("num");
	return new_node_num(expect_number());
}

// mul = unary ("*" unary | "/" unary)*
Node *mul() {
	dbg();
	Node *node = unary();

	for (;;) {
		if (consume("*"))
			node = new_node(ND_MUL, node, unary());
		else if (consume("/"))
			node = new_node(ND_DIV, node, unary());
		else
			return node;
	}
}

// 左結合の演算子を展開
// expr = mul ("+" mul | "-" mul)*
Node *add() {
	dbg();
	Node *node = mul();

	for (;;) {
		if (consume("+"))
			node = new_node(ND_ADD, node, mul());
		else if (consume("-"))
			node = new_node(ND_SUB, node, mul());
		else
			return node;
	}
}

//エラーを報告するための関数
// printfと同じ引数を取る
void error(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit;
}

//エラー箇所を報告する
void error_at(char *loc, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, " ");
	fprintf(stderr, "^ ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

//次のトークンが期待している記号のときには、トークンを１つ読み進めて
//真を返す。それ以外の場合には偽を返す。
bool consume(char *op) {
	if (token->kind != TK_RESERVED) {
		dbg("%s	false: kind=%s", op, TK_TABLE[token->kind]);
		return false;
	} else if (strlen(op) != token->len) {
		dbg("%s	false: strlen(op)=%d , token->len=%d, kind=%s", op,
		    (int)strlen(op), token->len, TK_TABLE[token->kind]);
		return false;
	} else if (memcmp(token->str, op, token->len != 0)) {
		dbg("%s	false: token->str=\"%s\" , op=\"%s\", token->len=%d, "
		    "kind=%s",
		    op, token->str, op, token->len, TK_TABLE[token->kind]);
		return false;
	}

	token = token->next;
	dbg("%s	true", op);
	return true;
}

//次のトークンが期待している記号のときには、トークンを１つ読み進めて
//それ以外の場合にはエラーを報告する。
void expect(char *op) {
	dbg();
	if (token->kind != TK_RESERVED || strlen(op) != token->len ||
	    memcmp(token->str, op, token->len))
		error_at(token->str, "'%c'ではありません", op);
	token = token->next;
}

//次のトークンが数値の場合、トークンを１つ読み進めて
//それ以外の場合にはエラーを報告する。
int expect_number() {
	if (token->kind != TK_NUM) error_at(token->str, "数ではありません");
	int val = token->val;
	dbg("%d", val);
	token = token->next;
	return val;
}

bool at_eof() { return token->kind == TK_EOF; }

//新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str) {
	dbg("kind = %s, str = \"%s\"", TK_TABLE[kind], str);
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	// tok->len = len;
	cur->next = tok;
	return tok;
}

Node *expr() {
	dbg();
	return equality();
}

//入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
	dbg("start");
	Token head;
	head.next = NULL;
	Token *cur = &head;
	int loop = 1;
	while (*p) {
		dbg("loop:%d", loop++);
		//空白文字をスキップ
		if (isspace(*p)) {
			p++;
			continue;
		}

		if (strncmp(p, "==", 2) == 0 || strncmp(p, "!=", 2) == 0 ||
		    strncmp(p, "<=", 2) == 0 || strncmp(p, ">=", 2) == 0) {
			dbg();
			cur = new_token(TK_RESERVED, cur, p);
			cur->len = 2;
			p += 2;
			continue;
		}

		if (*p == '>' || *p == '<') {
			dbg();
			cur = new_token(TK_RESERVED, cur, p++);
			cur->len = 1;
			continue;
		}

		if (*p == '(' || *p == ')') {
			dbg();
			cur = new_token(TK_RESERVED, cur, p++);
			cur->len = 1;
			continue;
		}

		if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
			dbg();
			cur = new_token(TK_RESERVED, cur, p++);
			cur->len = 1;
			continue;
		}

		if (isdigit(*p)) {
			dbg();
			cur = new_token(TK_NUM, cur, p);
			char *pbuff = p;
			cur->val = strtol(p, &p, 10);
			cur->len = (int)(pbuff - p);
			continue;
		}

		error_at(p, "トークナイズできません");

		exit(1);
	}

	new_token(TK_EOF, cur, p);
	return head.next;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		error("引数の個数が正しくありません");
		return 1;
	}

	//グローバル変数に入力された文字列を格納
	user_input = (char *)malloc(strlen(argv[1]));
	sprintf(user_input, "%s", argv[1]);

	//トークナイズしてパースする
	token = tokenize(user_input);
	dbg("Tokenized");
#ifdef __DEBUG__
	printf("Token->str = \"%s\"\n", token->str);
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

	free(user_input);
	return 0;
}

