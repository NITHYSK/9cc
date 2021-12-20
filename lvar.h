#ifndef __LVAR_H__
#define __LVAR_H__

typedef struct LVar LVar;

struct LVar {
	LVar *next;	// 次の変数かNULL
	char *name;	// 変数名
	int len;	// 変数名の長さ
	int offset;	// RBPからのオフセット
};

// ローカル変数
LVar *locals;

LVar *find_lvar(Token *tok) {
	for (LVar *var = locals; var; var = var->next)
	{
		if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
		{
			return var;
		}
	}
	return NULL;
}

#endif
