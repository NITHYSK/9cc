#ifndef __PARSE_C__
#define __PARSE_C__

#include "9cc.h"

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

void program() {
	int i = 0;
	while (!at_eof()) setCode(stmt(), i++);
	setCode(NULL, i);
}

Node *stmt() {
	dbg();
	Node *node = expr();
	expect(";");
	return node;
}

Node *expr() {
	dbg();
	return assign();
}

Node *assign() {
	Node *node = equality();
	if (consume("=")) {
		dbg("ND_ASSIGN");
		node = new_node(ND_ASSIGN, node, assign());
	}
	return node;
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

// 単項演算子のノードを作成する関数
Node *unary() {
	dbg();
	if (consume("+")) return primary();
	if (consume("-")) return new_node(ND_SUB, new_node_num(0), primary());
	return primary();
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

	// 次のトークンが識別子なら、その文字に応じてスタック上のオフセットが設定されたノードを作成
	Token *tok = consume_ident();
	if (tok) {
		Node *node = calloc(1, sizeof(Node));
		node->kind = ND_LVAR;
		node->offset = (tok->str[0] - 'a' + 1) * 8;
		return node;
	}

	// そうでなければ数値のはず
	dbg("num");
	return new_node_num(expect_number());
}

Node *ident() {}

//次のトークンが期待している記号のときには、トークンを１つ読み進めて
//真を返す。それ以外の場合には偽を返す。
bool consume(char *op) {
	if (getToken()->kind != TK_RESERVED) {
		dbg("%s	false: kind=%s", op, TK_TABLE[getToken()->kind]);
		return false;
	} else if (strlen(op) != getToken()->len) {
		dbg("%s	false: strlen(op)=%d , getToken()->len=%d, kind=%s", op,
		    (int)strlen(op), getToken()->len,
		    TK_TABLE[getToken()->kind]);
		return false;
	} else if (memcmp(getToken()->str, op, getToken()->len != 0)) {
		dbg("%s	false: getToken()->str=\"%s\" , op=\"%s\", "
		    "getToken()->len=%d, "
		    "kind=%s",
		    op, getToken()->str, op, getToken()->len,
		    TK_TABLE[getToken()->kind]);
		return false;
	}

	setToken(getToken()->next);
	dbg("%s	true", op);
	return true;
}

//次のトークンが識別子のときには、トークンをひとつ読み進める
//それ以外の場合にはNULLを返す
Token *consume_ident() {
	if (getToken()->kind != TK_IDENT) {
		dbg("false: getToken->kind=\"%s\"", TK_TABLE[getToken()->kind]);
		return NULL;
	}
	Token *bufftoken = getToken();
	dbg("%c	true", getToken()->str[0]);
	setToken(getToken()->next);
	return bufftoken;
}

#endif
