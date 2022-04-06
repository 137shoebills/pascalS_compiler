%{
#include "main.h"
#include "yacc.tab.h"

//处理位置信息
int yycolumn = 1;
extern YYLTYPE yylloc;
    
vector<string >lexicalErrorInformation;
string charTemp; //保存字符常量
void addLexicalErrorInformation(char* word, string info);
string itos(int num);

// extern YYSTYPE yylval;

//函数声明
extern "C"
{
	int yywrap();
	int yylex();
}
%}

%option yylineno
%x CH
%x SCMT
%x MCMT

line \n
digit [0-9]
letter [a-zA-Z]
identifier {letter}({letter}|{digit})*
integer ({digit}+)
floater {digit}+\.{digit}+
expint ({integer})E[+|-]?{integer}
expfloa ({floater})E[+|-]?{integer}
blank_chars [ \f\r\t\v]+
assginop (:=)
relop (>=|>|<=|<|<>|=)
addop (\+|or)
mulop (\*|\/|div|mod|and)
divop (\(|\)|\[|\]|:|,|;|\.)




%%
{blank_chars} {}
{line} {}

"array" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="ARRAY";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return ARRAY;
}

"begin" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="BEGIN";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return _BEGIN;
}

"boolean" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="BOOLEAN";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return BOOLEAN;
}

"case" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="CASE";
    yylval->lineNo = yylineno ;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return CASE;
}

"char" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="CHAR";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return CHAR;
}

"const" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="CONST";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return CONST;
}

"do" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="DO";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return DO;
}

"downto" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="DOWNTO";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return DOWNTO;
}

"else" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="ELSE";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return ELSE;
}


"end" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="END";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return END;
}


"for" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="FOR";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return FOR;
}

"function" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="FUNCTION";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return FUNCTION;
}

"if" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="IF";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return IF;
}

"integer" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="INTEGER";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return INTEGER;
}

"not" {
    yylval=new Token;
	yylval->value=yytext;
	yylval->type="NOT";
	yylval->lineNo = yylineno;
#ifdef LEXDEBUG
	cout << "NOT: " << yylval->value << endl;
#endif
	return NOT;
}

"of" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="OF";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return OF;
}

"procedure" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="PROCEDURE";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return PROCEDURE;
}

"program" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="PROGRAM";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return PROGRAM;
}

"real" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="REAL";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return REAL;
}

"record" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="RECORD";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return RECORD;
}

"repeat" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="REPEAT";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return REPEAT;
}

"then" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="THEN";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return THEN;
}

"to" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="TO";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return TO;
}

"type" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="TYPE";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return TYPE;
}

"until" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="UNTIL";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return UNTIL;
}

"var" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="VAR";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return VAR;
}

"while" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="WHILE";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "keyword: " << yylval->value << endl;
#endif
	return WHILE;
}

 {addop} {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="ADDOP";
    yylval->lineNo = yylineno;
 #ifdef LEXDEBUG
    cout << "addop:" << yylval->value << endl;
 #endif
    return ADDOP;
 }


 {mulop} {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="MULOP";
    yylval->lineNo = yylineno;
 #ifdef LEXDEBUG
    cout << "mulop:" << yylval->value << endl;
 #endif
    return MULOP;
 } 

 {relop} {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="RELOP";
    yylval->lineNo = yylineno;
 #ifdef LEXDEBUG
    cout << "relop:" << yylval->value << endl;
 #endif
    return RELOP;
 }

":=" {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="ASSIGNOP";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "ASSIGNOP: " << yylval->value << endl;
#endif
    return ASSIGNOP;
}

{divop} {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type=yytext;
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "divop:" << yylval->value << endl;
#endif
    return yytext[0];
}

{integer} {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="UINUM";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "integer:" << yylval->value << endl;
#endif
    return UINUM;
}

{floater} {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="UFNUM";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "floater:" << yylval->value << endl;
#endif
    return UFNUM;
}

{expint} {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="UINUM";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "UINUM:" << yylval->value << endl;
#endif
    return UINUM;
}

{expfloa} {
    yylval=new Token;
    yylval->value=yytext;
    yylval->type="UFNUM";
    yylval->lineNo = yylineno;
#ifdef LEXDEBUG
    cout << "UFNUM:" << yylval->value << endl;
#endif
    return UFNUM;
}

{identifier} {
    if(yyleng > 8){
        addLexicalErrorInformation(yytext, "IDENTIFIER TOO LONG");
        yytext[8]=0; //截断处理
        yyleng=8;
    }
    yylval=new Token;
	yylval->value=yytext;
	yylval->type = "IDENTIFIER";
	yylval->lineNo = yylineno;
#ifdef LEXDEBUG
	cout << "identifier: " << yylval->value << endl;
#endif
	return IDENTIFIER;
}

"'" {//进入字符常量识别
    BEGIN CH;
	charTemp="";
}

"{" { 
    BEGIN SCMT;
}

"(*" { 
    BEGIN MCMT;
}

. {//非法字符错误
    addLexicalErrorInformation(yytext, "Invalid character!");
	//cout << "error: invalid char" << endl;
}

<SCMT>"}" {
    BEGIN INITIAL;
}

<MCMT>"*)" {
    BEGIN INITIAL;
}

<SCMT><<EOF>> {
    addLexicalErrorInformation(yytext, "Unexpected EOF before a singleline comment ends");
    return 0;
}

<SCMT>"\n" {
    addLexicalErrorInformation(yytext, "Unexpected '\n' before a singleline comment ends");
    yyless(0);
}

<SCMT>. {/*do nothing*/}

<MCMT><<EOF>> {
    addLexicalErrorInformation(yytext, "Unexpected EOF before a multiline comment ends");
    return 0;
}

<MCMT>{line} {
    yyless(0);
}

<MCMT>. {}

<CH><<EOF>> {
    addLexicalErrorInformation(yytext, "Unexpected end of file when reading a char constant");
    return 0;
}
<CH>("'"|"\n") {
    int len = charTemp.length();
    if(yytext[0] == '\'' && len == 0){  //双引号之间无空格,错误处理为\0
        addLexicalErrorInformation(yytext,"Missing character!");
        yylval = new Token;
        yylval -> value = "\0";
        yylval -> type = "LETTER";
        yylval -> lineNo = yylineno;
#ifdef LEXDEBUGs
		cout << "char: " << yylval->value << endl;
#endif
        BEGIN INITIAL;
        return LETTER;
    }
    else if(yytext[0] == '\'' && len == 1){ //正确识别字符
        yylval = new Token;
        yylval -> value = charTemp;
        yylval -> type = "LETTER";
        yylval -> lineNo = yylineno;
#ifdef LEXDEBUGs
		cout << "char: " << yylval->value << endl;
#endif
        BEGIN INITIAL;
        return LETTER;
    }
    else if(yytext[0] == '\''){ //引号之间有多个字符，错误处理为第一个字符
        addLexicalErrorInformation(yytext,"Too many characters!");
        yylval = new Token;
        yylval -> value = charTemp[0];
        yylval -> type = "LETTER";
        yylval -> lineNo = yylineno;
#ifdef LEXDEBUGs
		cout << "char: " << yylval->value << endl;
#endif
        BEGIN INITIAL;
        return LETTER;
    }
    else if(yytext[0] == '\n'){ //缺失单引号
        addLexicalErrorInformation(yytext,"Right quote missing!");
        yyless(0);
        yylval = new Token;
        if(len == 0){
            yylval -> value = "\0";
        }
        else{
            yylval -> value = charTemp[0];   
        }
        yylval -> type = "LETTER";
        yylval -> lineNo = yylineno;
#ifdef LEXDEBUG
		cout << "char: " << yylval->value << endl;
#endif
        BEGIN INITIAL;
        return LETTER; 
    }
}
<CH>. {charTemp+=yytext[0];}



%%
void addLexicalErrorInformation(char* word, string info){
    string errorInformation = "[" + info + "] " + itos(yylineno) + ":" + string(word) + "\n";
    //cout << errorInformation << endl;
    lexicalErrorInformation.push_back(errorInformation);
}

string itos(int num){
	stringstream sin;
	sin<<num;
	return sin.str();
}

//这个函数是必须的，不用动
int yywrap()
{
	return 1;
}