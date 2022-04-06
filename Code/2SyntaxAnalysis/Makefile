all:
	gcc pre.c -o pre.out
	./pre.out pascal_s_test.pas
	bison -d yacc.y
	flex lex.l
	mv lex.yy.c lex.yy.cpp
	mv yacc.tab.c yacc.tab.cpp
	g++ yacc.tab.cpp lex.yy.cpp main.cpp -o test.exe