#!/bin/bash

assert(){
	input="$1"

	./9cc "$input" > tmp.s
	cc -o tmp tmp.s
	./tmp
	echo "$input => $?"
}

assert "1==1;"
assert "1+2+3 != 6;"
assert "-(-(-(-(5+2)*3)))-4 < 1+ 1 + 1+1+1+1+1+1;"
assert "123 - 50;"
assert "50   +1   +   2;"
assert "-23   +(((23+4+5*(5+2*6))-(-23))-1) - 2 ;"
assert "+123;"
assert "100 > 1;"
assert "100 < 1;"
assert "100 >=1;"
assert "100<= 1;"
assert "foo=12; bar=34; foo+bar;"
assert "hoge=12; _piyo_piyo=23; c=(hoge+_piyo_piyo)/5;"

