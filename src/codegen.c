#ifndef __CODEGEN_C__
#define __CODEGEN_C__
#include "9cc.h"
#include "global.h"

#define dispkind() (dbg("node->kind = \"%s\"", ND_TABLE[node->kind]))

void gen_lval(Node *node) {
	dbg();
	if (node->kind != ND_LVAR) error("代入の左辺値が変数ではありません");
	printf("	mov rax, rbp\n");
	printf("	sub rax, %d\n", node->offset);
	printf("	push rax\n");
}

void gen(Node *node) {
	switch (node->kind) {
		case ND_NUM:
			dispkind();
			printf("	push %d\n", node->val);
			return;
		case ND_LVAR:
			dispkind();
			gen_lval(node);
			printf("	pop rax\n");
			printf("	mov rax, [rax]\n");
			printf("	push rax\n");
			return;
		case ND_ASSIGN:
			dispkind();
			gen_lval(node->lhs);
			gen(node->rhs);

			printf("	pop rdi\n");
			printf("	pop rax\n");
			printf("	mov [rax], rdi\n");
			printf("	push rdi\n");
			return;
	}

	gen(node->lhs);
	gen(node->rhs);

	printf("	pop rdi\n");
	printf("	pop rax\n");

	switch (node->kind) {
		case ND_EQL:
			dispkind();
			printf("	cmp rax, rdi\n");
			printf("	sete al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_NEQ:
			dispkind();
			printf("	cmp rax, rdi\n");
			printf("	setne al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_GOE:
			dispkind();
			printf("	cmp rax, rdi\n");
			printf("	setle al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_LOE:
			dispkind();
			printf("	cmp rdi, rax\n");
			printf("	setle al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_GTR:
			dispkind();
			printf("	cmp rax, rdi\n");
			printf("	setl al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_LES:
			dispkind();
			printf("	cmp rdi, rax\n");
			printf("	setl al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_ADD:
			dispkind();
			printf("	add rax, rdi\n");
			break;
		case ND_SUB:
			dispkind();
			printf("	sub rax, rdi\n");
			break;
		case ND_MUL:
			dispkind();
			printf("	imul rax, rdi\n");
			break;
		case ND_DIV:
			dispkind();
			printf("	cqo\n");
			printf("	idiv rdi\n");
	}

	printf("	push rax\n");
}
#endif
