#ifndef MAIN_H
#define MAIN_H

#define YYERROR_VERBOSE
#define YYDEBUG 1
#define LEXDEBUG 1

#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "algorithm"
#include "ASTnodes.h"

extern int yydebug;
using namespace std;

extern vector<string> syntaxErrorInformation;
extern "C"
{
    int yyparse();
};

//重新定义属性类型
class Token
{
public:
    string type;              //类型
    string value;             //值
    int lineNo;               //所在行号
    vector<Token *> children; //语法树子节点

    Token() {}
    Token(string val) : type(val) {}
    Token(string tp, string val, int ln) : type(tp), value(val), lineNo(ln) {}
};

#define YYSTYPE Token *

#endif