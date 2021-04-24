#ifndef __9CC_H__
#define __9CC_H__

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"

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
static Token *token;

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
Token *getToken();
void setToken(Token *);
char *getUserinput();
void setUserInput(char *);

void error(char *, ...);
void error_at(char *, char *, ...);

Token *tokenize(char *);
Token *new_token(TokenKind, Token *, char *);

bool consume(char *);
void expect(char *);
int expect_number();
void gen(Node *);
Node *new_node(NodeKind, Node *, Node *);
Node *new_node_num(int);
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

//ユーザーが入力した数式
static char *user_input;

#endif
