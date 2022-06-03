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
	$(RM) -rf yacc.cpp yacc.hpp lex.cpp compiler $(OBJS)

yacc.cpp: yacc.y
	bison -d -o $@ $<

yacc.hpp: yacc.cpp

lex.cpp: lex.l yacc.hpp
	flex -o $@ $<

ParseT2AST.cpp: ParseT2AST.h

ASTnodes.cpp: ASTnodes.h

TypeSystem.cpp: TypeSystem.h

CodeGen.cpp: CodeGen.h

symbolTable.cpp: symbolTable.h

semanticAnalyse.cpp: semanticAnalyse.h CodeGen.h

ObjGen.cpp: ObjGen.h

main.cpp: ObjGen.h semanticAnalyse.h ParseT2AST.h

%.o: %.cpp
	g++ -c $(CPPFLAGS) -o $@ $<

compiler: $(OBJS)
	g++ $(CPPFLAGS) -o $@ $(OBJS) $(LIBS)
