all:
	bison -d yacc.y
	gcc pre.c -o pre.out
	./pre.out pascal_s_test.pas
	flex lex.l
	mv lex.yy.c lex.yy.cpp
	g++ lex.yy.cpp -lfl
	gcc yacc.tab.c lex.yy.cpp