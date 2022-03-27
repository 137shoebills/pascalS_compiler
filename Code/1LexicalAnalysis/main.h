#ifndef MAIN_H
#define MAIN_H
#define YYERROR_VERBOSE
#define YYDEBUG 1
#define LEXDEBUG

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

extern int yydebug;
using namespace std;

//重新定义属性类型
class Token{
public:
    string type;    //类型
    string value;   //值
    int lineNo;     //所在行号

    Token(){}
    Token(string tp, string val, int ln): type(tp), value(val), lineNo(ln){}
};

#define YYSTYPE Type*

#endif