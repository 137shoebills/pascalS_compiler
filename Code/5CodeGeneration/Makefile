all: compiler

OBJS =  yacc.o \
		lex.o  \
		ParseT2AST.o \
		ASTnodes.o \
		TypeSystem.o \
		CodeGen.o \
		symbolTable.o \
		semanticAnalyse.o \
		ObjGen.o \
		main.o	 \

LLVMCONFIG = llvm-config-10
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++14
LIBS = `$(LLVMCONFIG) --libs`

clean:
	$(RM) -rf yacc.cpp yacc.hpp lex.cpp $(OBJS)

yacc.cpp: yacc.y
	bison -d -o $@ $<

yacc.hpp: yacc.cpp

lex.cpp: lex.l yacc.hpp
	flex -o $@ $<

ParseT2AST.cpp: main.h ASTnodes.h

ASTnodes.cpp: ASTnodes.h

TypeSystem.cpp: TypeSystem.h

CodeGen.cpp: CodeGen.h

symbolTable.cpp: symbolTable.h

semanticAnalyse.cpp: CodeGen.h ASTnodes.h symbolTable.h

ObjGen.cpp: ObjGen.h

#main.cpp: ObjGen.h semanticAnalyse.cpp ParseT2AST.cpp
main.cpp: ObjGen.h

%.o: %.cpp
	g++ -c $(CPPFLAGS) -o $@ $<

compiler: $(OBJS)
	g++ $(CPPFLAGS) -o $@ $(OBJS) $(LIBS)

# test: compiler test.input
# 	cat test.input | ./compiler

# testlink: output.o testmain.cpp
# 	clang output.o testmain.cpp -o test
# 	./test
