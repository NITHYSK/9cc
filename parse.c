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

Node *expr() {
	dbg();
	return equality();
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

	// そうでなければ数値のはず
	dbg("num");
	return new_node_num(expect_number());
}

#endif
