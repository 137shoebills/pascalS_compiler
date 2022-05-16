all:
	bison -d yacc.y
	flex lex.l
	mv lex.yy.c lex.yy.cpp
	mv yacc.tab.c yacc.tab.cpp
	g++ yacc.tab.cpp lex.yy.cpp ASTnodes.cpp symbolTable.cpp main.cpp -o test.exe
