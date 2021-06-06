#include "global.h"

#include <stdlib.h>
#include <string.h>

#include "dbg.h"

//カプセル化されたグローバル変数
char *user_input;  //ユーザーが入力した数式
int varStackTop = 0;
char *globalScope[sizeof_globalscope];
int itr_top = 0;  // globalScopeのイテレータ

//メンバ関数

char *getUserinput() { return user_input; }

void setUserInput(char *u_i) { user_input = u_i; }

int getVarStackTop() { return varStackTop; }

void addVarStackTop(int size) { varStackTop += size; }

void subVarStackTop(int size) { varStackTop -= size; }

void initGlobalScope() {
	for (int i = 0; i < sizeof_globalscope; i++) {
		globalScope[i] = calloc(max_lengthof_idname, sizeof(char));
	}
}

void addGlobalScope(char *idname) {
	strcpy(globalScope[itr_top], idname);
	itr_top++;
}

char *getGlobalScope(int i) { return globalScope[i]; }

int searchGlobalScope(char *idname) {  //単純に上から探索していく
	for (int i = 0; i < sizeof_globalscope; i++) {
		if (strcmp(globalScope[i], idname) == 0) return i;
	}
	return -1;
}

int getVarOffsetFromIdent(char *idname) {
	//グローバルスコープから識別子名を検索し、一致するものがなければ新たに登録して変数のオフセットを設定する。
	int result_of_search = -1;
	if ((result_of_search = searchGlobalScope(idname)) == -1) {
		dbg("idname	= %s,	result_of_search	= %d", idname,
		    result_of_search);
		addGlobalScope(idname);
		addVarStackTop(8);
		return getVarStackTop();
	} else {
		//一致するものが見付かった場合には、同じ名前の変数のオフセットを取得する。
		dbg("idname	= %s,	result_of_search	= %d", idname,
		    result_of_search);
		return (result_of_search + 1) * 8;
	}
	return -1;
}

