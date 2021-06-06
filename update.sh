#!/bin/bash

git init
git add README Makefile src include *.sh .gitignore lib obj
git commit -m "\"$1\""
#git log -p
git remote add origin git@github.com:NITHYSK/9cc.git
git push

