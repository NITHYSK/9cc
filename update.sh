#!/bin/bash

git init
git add 9cc.h Makefile codegen.c dbg.h error.c global.c main.c parse.c test.sh test1.sh tokenize.c update.sh .gitignore
git commit -m "\"$1\""
#git log -p
git remote add origin git@github.com:NITHYSK/9cc.git
git push

