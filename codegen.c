#ifndef __CODEGEN_C__
#define __CODEGEN_C__
#include "9cc.h"
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
#endif
