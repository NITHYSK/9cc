/*
 * global.h
 * ユーザー入力や9ccの変数などといったグローバルスコープのデータを扱う。
 *
 *
 */

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define __DEBUG__
#define sizeof_globalscope 256
#define max_lengthof_idname 64

char *getUserinput();
void setUserInput(char *);
int getVarStackTop();
void addVarStackTop(int);
void subVarStackTop(int);
void initGlobalScope();
char *getGlobalScope(int);
int getVarOffsetFromIdent(char *);

#endif
