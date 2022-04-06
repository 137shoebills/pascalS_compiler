%{
#include "main.h"
#include "yacc.tab.h"

extern "C"
{
	int yyparse();  //分析器的入口函数
	void yyerror(const char *s);    //检测到语法错误时报告，参数为描述错误的字符串
	extern int yylex(); //在lex.yy.c中定义 yyparse()需要调用该函数
}

extern YYSTYPE yylval;
extern int yylineno;
extern char* yytext;
extern char lineBuffer[500];
extern int yyleng;
extern int yycolumn;
extern string itos(int num);

Token* ParseTreeHead=NULL;
bool haveSemanticError=false;
vector<string> syntaxErrorInformation; //存放语法错误信息

void yyerror(const char *s, YYLTYPE *loc);
void yyerror(const char *s, int line, int col);
void yyerror(const char *s, int startLine, int startCol, int endLine, int endCol);

%}

%token PROGRAM	//%token 声明终结符
%token CONST 
%token VAR 
%token ARRAY 
%token OF 
%token PROCEDURE 
%token FUNCTION 
%token _BEGIN 
%token END 
%token IF 
%token THEN 
%token CASE 
%token FOR 
%token TO 
%token DOWNTO 
%token DO 
%token ELSE 
%token REPEAT 
%token UNTIL 
%token WHILE 

%token IDENTIFIER
%token UINUM
%token UFNUM
%token CHAR
%token LETTER 
%token INTEGER 
%token TYPE 
%token ASSIGNOP
%token RELOP 
%token ADDOP 
%token MULOP 
%token NOT 
%token RECORD 
%token REAL 
%token BOOLEAN

%start programstruct	//%start 文法开始符号

%left '+' '-' ADD
%right '*' '/' MUL
%nonassoc ELSE	//不可结合


%%	

programstruct: program_head program_body '.' {
                        ParseTreeHead=$$=new Token("programstruct");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                        if(yylex())     yyerror("redundant content at the end",@3.last_line,@3.last_column+1);
                        YYACCEPT;
                } | error program_body '.' {//error: program_head识别出错
                        ParseTreeHead=$$=new Token("programstruct");
                        yyerror("fatal error in program_head, maybe missing keyword \"PROGRAM\"",@1.first_line,@1.first_column,@1.last_line,@1.last_column);
                } | program_head error '.' {//error: program_body识别出错
                       ParseTreeHead=$$=new Token("programstruct");
                        yyerror("fatal error in program body.");
                } | program_head program_body error{//error: 末尾缺少'.'
                        ParseTreeHead=$$=new Token("programstruct");
                        yyerror("missing '.' here",@2.last_line,@2.last_column+1);
                };

program_head: PROGRAM IDENTIFIER '(' idlist ')' ';' {
                        $$=new Token("program_head");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                        $$->children.push_back($4);
                        $$->children.push_back($5);
                        $$->children.push_back($6);
                } | PROGRAM error '(' idlist ')' ';' {//error:POGRAM后缺少主程序名
                        $$=new Token("program_head");
                        yyerror("missing PROGRAM name here",@1.last_line,@1.last_column+1);
                } | PROGRAM IDENTIFIER error idlist ')' ';' {//error: 缺少左括号'('
                        $$=new Token("program_head");
                        yyerror("missing '(' here.",@4.last_line,@4.last_column-1);
                } | PROGRAM IDENTIFIER '(' error')' ';' {//error: idlist识别失败
                        $$=new Token("program_head");
                        yyerror("program identifier missing or illegal",@4.first_line,@4.first_column,@4.last_line,@4.last_column);
                } | PROGRAM IDENTIFIER '(' idlist error ';' {//error:缺少右括号')'
                        $$=new Token("program_head");
                        yyerror("missing '(' here.",@4.last_line,@4.last_column+1);
                } | PROGRAM IDENTIFIER '(' idlist ')' error {//error: 缺少分号';'
                        $$=new Token("program_head");
                        yyerror("missing ';' here.",@5.last_line,@5.last_column+1);
                };

program_body: const_declarations type_declarations var_declarations subprogram_declarations compound_statement {
                        $$=new Token("program_body");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                        $$->children.push_back($4);
                        $$->children.push_back($5);
                } | error type_declarations var_declarations subprogram_declarations compound_statement { //error:const_declarations识别失败
                        $$=new Token("program_body");
                        yyerror("fatal error in const_declarations, maybe missing keyword \"CONST\"",@1.first_line,@1.first_column-1);
                } | const_declarations type_declarations var_declarations subprogram_declarations error { //error: compound_statement识别失败
                        $$=new Token("program_body");
                        yyerror("fatal error in compound_statement, maybe missing keyword \"begin\"",@5.first_line,@5.first_column-1);
                };

idlist: idlist ',' IDENTIFIER {
                        $$=new Token("idlist");
			            $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                } | IDENTIFIER {
                        $$=new Token("idlist");
			            $$->children.push_back($1);
                };

const_declarations: CONST const_declaration ';' {
                        $$=new Token("const_declarations");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);                
                } | CONST error ';' {//error: const_declarations识别失败
                        $$=new Token("const_declarations");
                        yyerror("fatal error in const_declaration, identifier may be illegal",@2.first_line,@2.first_column,@2.last_line,@2.last_column);
                } | CONST const_declaration error {//error: 缺少分号';'
                        $$=new Token("const_declarations");
                        yyerror("missing ';' here.",@2.last_line,@2.last_column+1);
                } | {
                        $$=new Token("const_declarations");
                };

const_declaration: const_declaration ';' IDENTIFIER '=' const_variable {    //产生式1
                        $$=new Token("const_declaration");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                        $$->children.push_back($4);
                        $$->children.push_back($5);
                } | IDENTIFIER '=' const_variable {    //产生式2
                        $$=new Token("const_declaration");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                } | error ';' IDENTIFIER '=' const_variable {     //error: const_declaration识别失败（产生式1）
                        $$=new Token("const_declaration");
                        yyerror("fatal error in const_declaration, identifier missing or illegal.",@1.first_line,@1.first_column,@1.last_line,@1.last_column);
                } | const_declaration error IDENTIFIER '=' const_variable {//error: 缺少分号（产生式1）
                        $$=new Token("const_declaration");
                        yyerror("missing ';' here.",@1.last_line,@1.last_column+1);
                } | const_declaration ';' error '=' const_variable {      //error: 标识符识别失败（产生式1）
                        $$=new Token("const_declaration");
                        yyerror("identifier is illegal or missing .",@3.first_line,@3.first_column,@3.last_line,@3.last_column);
                } | const_declaration ';' IDENTIFIER error const_variable {   //error: 缺少等号'+'（产生式1）
                        $$=new Token("const_declaration");
                        yyerror("missing '=' here.",@3.last_line,@3.last_column+1);
                } | const_declaration ';' IDENTIFIER '=' error {       //error: 无法识别const_variable（产生式1）
                        $$=new Token("const_declaration");
                        yyerror("fatal error in const_variable",@5.first_line,@5.first_column,@5.last_line,@5.last_column);
                } | error '=' const_variable {     //error: 标识符无法识别或缺失（产生式2）
                        $$=new Token("const_declaration");
                        yyerror("identifier is illegal or missing.",@2.last_line,@2.last_column-1);
                } | IDENTIFIER error const_variable {  //error:缺少等号'='（产生式2）
                        $$=new Token("const_declaration");
                        yyerror("missing '=' here.",@1.last_line,@1.last_column+1);
                } | IDENTIFIER '=' error {     //error: const_variable识别失败（产生式2）
                        $$=new Token("const_declaration");
                        yyerror("fatal error in const_variable",@3.first_line,@3.first_column,@3.last_line,@3.last_column);
                };


const_variable: '+' IDENTIFIER {
                        $$=new Token("const_variable");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                } | '-' IDENTIFIER {
                        $$=new Token("const_variable");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                } | IDENTIFIER {
                        $$=new Token("const_variable");
                        $$->children.push_back($1);
                } | '+' UINUM {
                        $$=new Token("const_variable");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                } | '+' UFNUM {
                        $$=new Token("const_variable");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                } | '-' UINUM {
                        $$=new Token("const_variable");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                } | '-' UFNUM {
                        $$=new Token("const_variable");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                } | UINUM {
                        $$=new Token("const_variable");
                        $$->children.push_back($1);
                } | UFNUM {
                        $$=new Token("const_variable");
                        $$->children.push_back($1);
                } | '\'' LETTER '\'' {
                        $$=new Token("const_variable");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                };



type_declarations: TYPE type_declaration';' {
                        $$=new Token("type_declarations");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                } | {
                        $$=new Token("type_declarations");
                } | error type_declaration';' {       //error: 缺少关键字TYPE
                        $$=new Token("type_declarations");
                        yyerror("missing keyword \"TYPE\"",@2.first_line,@2.last_column-1);
                } | TYPE error ';' {      //error: type_declaration识别失败
                        $$=new Token("type_declarations");
                        yyerror("fatal error in type_declaration, check whether identifiers is legal.",@2.first_line,@2.first_column,@2.last_line,@2.last_column);
                } | TYPE type_declaration error {      //error: 缺少分号';'
                        $$=new Token("type_declarations");
                        yyerror("missing ';' here.",@2.last_line,@2.last_column+1);
                };

type_declaration:  type_declaration';' IDENTIFIER '=' type {     //产生式1
                        $$=new Token("type_declaration");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                        $$->children.push_back($4);
                        $$->children.push_back($5);
                } | IDENTIFIER '=' type {      //产生式2
                        $$=new Token("type_declaration");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                } | type_declaration error IDENTIFIER '=' type {  //error: 缺少分号';'（产生式1）
                        $$=new Token("type_declaration");
                        yyerror("missing ';' here.",@1.last_line,@1.last_column+1);
                } | type_declaration';' IDENTIFIER error type {   //error: 缺少等号'='（产生式1）
                        $$=new Token("type_declaration");
                        yyerror("missing '=' here.",@3.last_line,@3.last_column+1);
                } | error '=' type {    //error:标识符无法识别或缺失（产生式2）
                        $$=new Token("type_declaration");
                        yyerror("identifier is illegal or missing.",@1.first_line,@1.first_column,@1.last_line,@1.last_column);
                } | IDENTIFIER error type {    //error:缺少等号'='（产生式2）
                        $$=new Token("type_declaration");
                        yyerror("missing '=' here.",@1.last_line,@1.last_column+1);
                } | IDENTIFIER '=' error {     //error: type识别失败
                        $$=new Token("type_declaration");
                        yyerror("fatal error in type",@3.first_line,@3.first_column,@3.last_line,@3.last_column);
                };

type:  standard_type {   //产生式1
                        $$=new Token("type");
                        $$->children.push_back($1);
                } | RECORD record_body END {    //产生式2
                        $$=new Token("type");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                } | ARRAY '[' periods ']' OF type {  //产生式3
                        $$=new Token("type");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                        $$->children.push_back($4);
                        $$->children.push_back($5);
                        $$->children.push_back($6);
                } | error record_body END {    //error: 缺少关键字"RECORD" （产生式2）
                        $$=new Token("type");
                        yyerror("missing keyword \"RECORD\"",@2.last_line,@2.last_column-1);
                } | RECORD record_body error {     //error: 缺少关键字"END" （产生式2）
                        $$=new Token("type");
                        yyerror("missing keyword \"END\"",@2.last_line,@2.last_column+1);
                } | error '[' periods ']' OF type {   //error: 缺少关键字“ARRAY” （产生式3）
                        $$=new Token("type");
                        yyerror("missing keyword \"ARRAY\"",@2.last_line,@2.last_column-1);
                } | ARRAY error periods ']' OF type {   //error:缺少左括号'[' （产生式3）
                        $$=new Token("type");
                        yyerror("missing '[' here.",@2.first_line,@2.first_column-1);
                } | ARRAY '[' error ']' OF type {      //error:periods识别失败 （产生式3）
                        $$=new Token("type");
                        yyerror("fatal error in periods",@3.first_line,@3.first_column,@3.last_line,@3.last_column);
                } | ARRAY '[' periods error OF type {      //error:缺少右括号'[' （产生式3）
                        $$=new Token("type");
                        yyerror("missing ']' here.",@3.last_line,@3.last_column+1);
                } | ARRAY '[' periods ']' error type {    //error: 缺少关键字“OF” （产生式3）
                        $$=new Token("type");
                        yyerror("missing keyword \"OF\".",@4.last_line,@4.last_column+1);
                } | ARRAY '[' periods ']' OF error {       //error: type识别失败 （产生式3）
                        $$=new Token("type");
                        yyerror("fatal error in type",@6.first_line,@6.first_column,@6.last_line,@6.last_column);
                };


standard_type: INTEGER {
                        $$=new Token("standard_type");
                        $$->children.push_back($1);

                } | REAL {
                        $$=new Token("standard_type");
                        $$->children.push_back($1);
                } | BOOLEAN {
                        $$=new Token("standard_type");
                        $$->children.push_back($1);
                } | CHAR {
                        $$=new Token("standard_type");
                        $$->children.push_back($1);
                };

record_body : var_declaration {
                        $$=new Token("record_body");
                        $$->children.push_back($1);
                } | {
                        $$=new Token("record_body");
                };

periods: periods','period {
                        $$=new Token("periods");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                } | period {
                        $$=new Token("periods");
                        $$->children.push_back($1);
                } | periods error period {     //error: 缺少','
                        $$=new Token("periods");
                        yyerror("missing ',' here.",@1.last_line,@1.last_column+1);
                } | periods','error {          //error: period识别失败
                        $$=new Token("periods");
                        yyerror("fatal error in period",@3.first_line,@3.first_column,@3.last_line,@3.last_column);
                };

period: const_variable'.' '.' const_variable {
                        $$=new Token("period");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                        $$->children.push_back($4);
                } | const_variable error '.' const_variable {   //缺少'.'
                        $$=new Token("period");
                        yyerror("missing '.' here.",@1.last_line,@1.last_column+1);
                } | const_variable'.' error  const_variable {     //error: 缺少'.'
                        $$=new Token("period");
                        yyerror("missing '.' here.",@2.last_line,@2.last_column+1);
                } | const_variable'.' '.' error {      //error: const_variable识别失败
                        $$=new Token("period");
                        yyerror("fatal error in const_variable",@4.first_line,@4.first_column,@4.last_line,@4.last_column);
                };

var_declarations: VAR var_declaration';'{
                        $$=new Token("var_ declarations");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                } | {
                        $$=new Token("var_ declarations");
                } | error var_declaration';' {     //error: 缺少关键字"VAR"
                        $$=new Token("var_ declarations");
                        yyerror("missing keyword \"VAR\"",@2.last_line,@2.last_column-1);
                } | VAR error';' {       //error: var_declaration识别失败
                        $$=new Token("var_ declarations");
                        yyerror("fatal error in var_declaration",@2.first_line,@2.first_column,@2.last_line,@2.last_column);
                } | VAR var_declaration error {   //error: 缺少分号';'
                        $$=new Token("var_ declarations");
                        yyerror("missing ';' here.",@2.last_line,@2.last_column+1);
                };

var_declaration : var_declaration ';' idlist ':' type {  //产生式1
                        $$=new Token("var_declaration");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                        $$->children.push_back($4);
                        $$->children.push_back($5);
                } | idlist ':' type {    //产生式2
                        $$=new Token("var_declaration");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                } | var_declaration error idlist ':' type {    //error: 缺少分号';' （产生式1）
                        $$=new Token("var_declaration");
                        yyerror("missing ':' here. ",@1.last_line,@1.last_column+1);
                } | var_declaration ';' error ':' type {      //error: 无法识别idlist （产生式1）
                        $$=new Token("var_declaration");
                        yyerror("fatal error in idlist, identifier may be ilegal or missing",@3.first_line,@3.first_column,@3.last_line,@3.last_column);
                } | var_declaration ';' idlist error type {      //error:缺少冒号':' （产生式1）
                        $$=new Token("var_declaration");
                        yyerror("missing ':' here.",@3.last_line,@3.last_column+1);
                } | var_declaration ';' idlist ':' error {      //error: 无法识别type （产生式1）
                        $$=new Token("var_declaration");
                        yyerror("fatal error in type.",@5.first_line,@5.first_column,@5.last_line,@5.last_column);
                } | idlist error type {    //error: 缺少冒号 （产生式2）
                        $$=new Token("var_declaration");
                        yyerror("missing ':' here.",@1.last_line,@1.last_column+1);
                } | idlist ':' error {     //error: type识别失败
                        $$=new Token("var_declaration");
                        yyerror("fatal error in type",@3.first_line,@3.first_column,@3.last_line,@3.last_column);
                };

subprogram_declarations: subprogram_declarations subprogram_declaration';' {
                        $$=new Token("subprogram_declarations");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                } | { 
                        $$=new Token("subprogram_declarations");
                } | subprogram_declarations subprogram_declaration error { //error: 缺少分号';'
                        $$=new Token("subprogram_declarations");
                        yyerror("miising ';' here.",@2.last_line,@2.last_column+1);
                };

subprogram_declaration : subprogram_head program_body {
                        $$=new Token("subprogram_declaration");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                } | error program_body {     //error: subprogram_head识别失败
                        $$=new Token("subprogram_declaration");
                        yyerror("fatal error in subprogram_head.",@1.first_line,@1.first_column,@1.last_line,@1.last_column);
                } | subprogram_head error {     //error: program_body识别失败
                        $$=new Token("subprogram_declaration");
                        yyerror("fatal error in program_body.",@2.first_line,@2.first_column,@2.last_line,@2.last_column);
                };

subprogram_head : FUNCTION IDENTIFIER  formal_parameter ':' standard_type';' {   //产生式1
                        $$=new Token("subprogram_head");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                        $$->children.push_back($4);
                        $$->children.push_back($5);
                        $$->children.push_back($6);
                } | PROCEDURE IDENTIFIER  formal_parameter';' {   //产生式2
                        $$=new Token("subprogram_head");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                        $$->children.push_back($4);
                } | error IDENTIFIER  formal_parameter ':' standard_type';' {   //error: 缺少关键字“FUNCTOPN"（产生式1）
                        $$=new Token("subprogram_head");
                        yyerror("missing keyword 'FUNCTION' here. ",@2.last_line,@2.last_column-1);
                } | FUNCTION error  formal_parameter ':' standard_type';' {   //error: "IDENTIFIER"非法或缺失 （产生式1）
                        $$=new Token("subprogram_head");
                        yyerror("identifier is ilegal or miising.",@2.first_line,@2.first_column,@2.last_line,@2.last_column);
                } | FUNCTION IDENTIFIER  error ':' standard_type';' {   //error: formal_parameter识别失败 （产生式1）
                        $$=new Token("subprogram_head");
                        yyerror("fatal error in formal_parameter.",@3.first_line,@3.first_column,@3.last_line,@3.last_column);
                } | FUNCTION IDENTIFIER  formal_parameter error standard_type';' {   //error: 缺少冒号 ':'  （产生式1）
                        $$=new Token("subprogram_head");
                        yyerror("missing ':' here",@3.last_line,@3.last_column+1);
                } | FUNCTION IDENTIFIER  formal_parameter ':' error ';' {   //error: standard_type识别失败 （产生式1）
                        $$=new Token("subprogram_head");
                        yyerror("fatal error in standard_type.",@5.first_line,@5.first_column,@5.last_line,@5.last_column);
                } | FUNCTION IDENTIFIER  formal_parameter ':' standard_type error {   //error: 缺少分号';' （产生式1）
                        $$=new Token("subprogram_head");
                        yyerror("missing ';' here.",@5.last_line,@5.last_column+1);
                } | error IDENTIFIER  formal_parameter';' {     //error: 缺少关键字"PROCEDURE" （产生式2）
                        $$=new Token("subprogram_head");
                        yyerror("missing keyword 'PROCEDURE' here.",@2.first_line,@2.first_column-1);
                } | PROCEDURE error  formal_parameter';' {     //error: "IDENTIFIER"非法或缺失 （产生式2）
                        $$=new Token("subprogram_head");
                        yyerror("identifier is illegal or missing.",@2.first_line,@2.first_column,@2.last_line,@2.last_column);
                } | PROCEDURE IDENTIFIER  error ';' {     //error:  formal_parameter识别失败  （产生式2）
                        $$=new Token("subprogram_head");
                        yyerror("fatal error in formal_parameter.",@3.first_line,@3.first_column,@3.last_line,@3.last_column);
                } | PROCEDURE IDENTIFIER  formal_parameter error {     //error: 缺少分号';'    （产生式2）
                        $$=new Token("subprogram_head");
                        yyerror("missing ';' here.",@3.last_line,@3.last_column+1);
                };

formal_parameter: '(' parameter_lists ')' {
                        $$=new Token("formal_parameter");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                } | {
                        $$=new Token("formal_parameter");
                } | error parameter_lists ')' {       //error: 缺少左括号'('
                        $$=new Token("formal_parameter");
                        yyerror("missing '(' here.",@2.first_line,@2.first_column-1);
                } | '(' error ')' {      //error: 无法识别 parameter_lists
                        $$=new Token("formal_parameter");
                        yyerror("fatal error in parameter_lists.",@2.first_line,@2.first_column,@2.last_line,@2.last_column);
                } | '(' parameter_lists error {       //error:缺少右括号')'
                        $$=new Token("formal_parameter");
                        yyerror("missing ')' here.",@2.last_line,@2.last_column+1);
                };

parameter_lists: parameter_lists ';' parameter_list {
                        $$=new Token("parameter_lists");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                } | parameter_list {
                        $$=new Token("parameter_lists");
                        $$->children.push_back($1);
                } | parameter_lists error parameter_list {   //error: 缺少分号';'
                        $$=new Token("parameter_lists");
                        yyerror("missing ';' here.",@1.last_line,@1.last_column+1);
                } | parameter_lists ';' error {   //error: parameter_list识别失败
                        $$=new Token("parameter_lists");
                        yyerror("fatal error in parameter_list.",@3.first_line,@3.first_column,@3.last_line,@3.last_column);
                };

parameter_list: var_parameter{
                        $$=new Token("parameter_list");
                        $$->children.push_back($1);
                } | value_parameter {
                        $$=new Token("parameter_list");
                        $$->children.push_back($1);
                };


/* var x,y: interger */
var_parameter : VAR value_parameter {
                    $$ = new Token("var_parameter");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                } | VAR error { //不完整的引用参数列表
                    $$ = new Token("var_parameter");
                    yyerror("incomplete refereced parameter list", &@$);
                };

/* x,y : interger */
value_parameter : idlist ':' standard_type {
                    $$ = new Token("value_parameter");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);
                } | idlist error TYPE { //缺少冒号
                    $$ = new Token("value_parameter");
                    yyerror("missing a colon ':' here", @1.first_line, @1.last_column+1);
                } | idlist ':' error {  //缺少基本类型关键字
                    $$ = new Token("value_parameter");
                    yyerror("missing a base type keyword here", @2.last_line, @2.last_column+1);
                } | idlist error{ //缺少基本类型关键字
                    $$ = new Token("value_parameter");
                    yyerror("missing a base type keyword here", @1.last_line, @1.last_column+1);
				} ;

compound_statement : _BEGIN statement_list END {
                    $$ = new Token("compound_statement");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);
                } | _BEGIN statement_list error{ //ERROR 缺少END关键字 checked
                    $$ = new Token("compound_statement");
                    yyerror("missing keyword \"end\"", @2.last_line, @2.last_column+1);
				};

statement_list : statement_list ';' statement {
                    $$ = new Token("statement_list");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);
                } statement_list error statement {  //缺少分号
                    $$ = new Token("statement_list");
                    yyerror("missing a semicolon ';' here", @1.last_line, @1.last_column+1);
                } | statement {
                    $$ = new Token("statement_list");
                    $$->children.push_back($1);
                };

statement : variable ASSIGNOP expression {
                    $$ = new Token("statement");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);
                } | procedure_call {
                    $$ = new Token("statement");
                    $$->children.push_back($1);
                } | compound_statement {
                    $$ = new Token("statement");
                    $$->children.push_back($1);
                } | IF expression THEN statement else_part {
                    $$ = new Token("statement");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);                        
                    $$->children.push_back($4);                        
                    $$->children.push_back($5);                        
                } | IF expression error statement else_part {  //缺少then关键字
                    $$ = new Token("statement");
                    yyerror("missing keyword \"then\"", @2.last_line, @2.last_column+1);
                } | CASE expression OF case_body END {
                    $$ = new Token("statement");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);                        
                    $$->children.push_back($4);                        
                    $$->children.push_back($5);
                } | CASE expression error case_body END {   //缺少of关键字
                    $$ = new Token("statement");
                    yyerror("missing keyword \"of\"", @2.last_line, @2.last_column+1);
                } | CASE expression OF case_body error {    //缺少end关键字
                    $$ = new Token("statement");
                    yyerror("missing keyword \"end\"", @4.last_line, @4.last_column+1);
                } | WHILE expression DO statement {
                    $$ = new Token("statement");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);                        
                    $$->children.push_back($4);
                } |WHILE expression error statement{ //缺少关键字do
                    $$ = new Token("statement");
				    yyerror("missing keywrod \"do\"", @2.last_line, @2.last_column+1);
			    } | REPEAT statement_list UNTIL expression {
                    $$ = new Token("statement");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);                        
                    $$->children.push_back($4);
                } | REPEAT statement_list error expression {
                    $$ = new Token("statement");
				    yyerror("missing keywrod \"until\"", @2.last_line, @2.last_column+1);
                } | FOR IDENTIFIER ASSIGNOP expression updown expression DO statement {
                    $$ = new Token("statement");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);                        
                    $$->children.push_back($4);                        
                    $$->children.push_back($5);                        
                    $$->children.push_back($6);                        
                    $$->children.push_back($7);                        
                    $$->children.push_back($8);
                } | FOR IDENTIFIER error expression updown expression DO statement { //缺少赋值号
                    $$ = new Token("statement");
                    yyerror("missing assignop \":=\"", @2.last_line, @2.last_column+1);
                } | FOR IDENTIFIER ASSIGNOP expression error expression DO statement {  //缺少to 或 downto
                    $$ = new Token("statement");
                    yyerror("missing keyword \"to\" or \"downto\"", @2.last_line, @2.last_column+1);
                } | FOR IDENTIFIER ASSIGNOP expression updown expression error statement{ //缺少关键字do
                    $$ = new Token("statement");
				    yyerror("missing keywrod \"do\"", @6.last_line, @4.last_column+1);
			    } | {
                    $$ = new Token("statement");
                };

variable : IDENTIFIER id_varparts {
                    $$ = new Token("variable");
                    $$->children.push_back($1);                        
                    $$->children.push_back($2);                        
                };

id_varparts : id_varparts id_varpart {
                    $$ = new Token("variable");
                    $$->children.push_back($1);                        
                    $$->children.push_back($2); 
                } | {
                    $$ = new Token("variable");
                };

id_varpart : '[' expression_list ']' {
                    $$ = new Token("id_varpart");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);
                } '[' expression_list error {   //缺少右大括号
                    $$ = new Token("id_varpart");
                    yyerror("missing a right bracket ']' here", @2.last_line, @2.last_column+1);
                } | '.' IDENTIFIER {
                    $$ = new Token("id_varpart");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                };

else_part : ELSE statement {
                    $$ = new Token("else_part");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                } | {
                    $$ = new Token("else_part");
                };

case_body : branch_list {
                    $$ = new Token("case_body");
                    $$->children.push_back($1);
                } | {
                    $$ = new Token("case_body");
                };

branch_list : branch_list ';' branch {
                    $$ = new Token("branch_list");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);
                } branch_list error branch {    //缺少分号
                    $$ = new Token("branch_list");
                    yyerror("missing a semicolon ';' here", @1.first_line, @1.last_column+1);
                } | branch {
                    $$ = new Token("branch_list");
                    $$->children.push_back($1);
                };

branch : const_list ':' statement {
                    $$ = new Token("branch");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);
                } const_list error statement {  //缺少冒号
                    $$ = new Token("branch");
                    yyerror("missing a colon ':' here", @1.first_line, @1.last_column+1);
                };

const_list : const_list ',' const_variable {
                    $$ = new Token("const_list");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);
                } const_list error const_variable { //缺少逗号
                    $$ = new Token("const_list");
                    yyerror("missing a comma ',' here", @1.first_line, @1.last_column+1);
                } | const_variable {
                    $$ = new Token("const_list");
                    $$->children.push_back($1);
                };

updown : TO {
                $$ = new Token("updown");
                $$->children.push_back($1);
            } | DOWNTO {
                $$ = new Token("updown");
                $$->children.push_back($1);
            };

procedure_call : IDENTIFIER {
                        $$ = new Token("procedure_call");
                        $$->children.push_back($1);
                    } | IDENTIFIER '(' expression_list ')' {
                        $$ = new Token("procedure_call");
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                        $$->children.push_back($3);
                        $$->children.push_back($4);
                    } | IDENTIFIER '(' expression_list error {  //缺少右括号
                        $$ = new Token("procedure_call");
                        yyerror("missing a right bracket ')' here", @3.last_line, @3.last_column+1);
                    };

expression_list : expression_list ',' expression {
                    $$ = new Token("expression_list");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);
                } expression_list error expression {
                    $$ = new Token("expression_list");
                    yyerror("missing a comma ',' here", @1.last_line, @1.last_column+1);
                } | expression {
                    $$ = new Token("expression_list");
                    $$->children.push_back($1);
                };

expression : simple_expression RELOP simple_expression {
                $$ = new Token("expression");
                $$->children.push_back($1);
                $$->children.push_back($2);
                $$->children.push_back($3);
            } | simple_expression {
                $$ = new Token("expression");
                $$->children.push_back($1);
            };

simple_expression : term {
                    $$ = new Token("simple_expression");
                    $$->children.push_back($1);
                } | '+' term {
                    $$ = new Token("simple_expression");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                } | '-' term {
                    $$ = new Token("simple_expression");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                } | simple_expression ADDOP term {
                    $$ = new Token("simple_expression");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);
                } | simple_expression ADDOP error term %prec ADD {  //ADDOP后缺少操作数
                    $$ = new Token("simple_expression");
                    yyerror("missing operand",@2.last_line, @2.last_column+1);
				};

term : term MULOP factor {
                    $$ = new Token("term");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);
                } | term MULOP error factor %prec MUL{  //MULOP后缺少操作数 a*-b 
                    $$ = new Token("term");
                    yyerror("missing operand",@2.last_line, @2.last_column+1);
                } | factor {
                    $$ = new Token("term");
                    $$->children.push_back($1);
                };

factor : unsign_const {
                $$ = new Token("factor");
                $$->children.push_back($1);
            } | variable {
                $$ = new Token("factor");
                $$->children.push_back($1);
            } | IDENTIFIER '(' expression_list ')' {
                $$ = new Token("factor");
                $$->children.push_back($1);
                $$->children.push_back($2);
                $$->children.push_back($3);
                $$->children.push_back($4);
            } | IDENTIFIER '(' expression_list error {  //缺少右括号
                $$ = new Token("factor");
                yyerror("missing a right bracket ')' here", @3.last_line, @3.last_column+1);
            } | IDENTIFIER '(' error { //函数调用的表达式列表缺失
                $$ = new Token("factor");
                yyerror("missing actual parameter list of function call", @2.last_line, @2.last_column+1);
            } | '(' expression ')' {
                $$ = new Token("factor");
                $$->children.push_back($1);
                $$->children.push_back($2);
                $$->children.push_back($3);
            } | '(' expression error {  //缺少右括号
                $$ = new Token("factor");
                yyerror("missing a right bracket ')' here", @2.last_line, @2.last_column+1);
            } | NOT factor {
                $$ = new Token("factor");
                $$->children.push_back($1);
                $$->children.push_back($2);
            };

unsign_const : IDENTIFIER {
                    $$ = new Token("unsign_const");
                    $$->children.push_back($1);
                } | UFNUM {
                    $$ = new Token("unsign_const");
                    $$->children.push_back($1);
                } | UINUM {
                    $$ = new Token("unsign_const");
                    $$->children.push_back($1);
                } | '\'' LETTER '\'' {
                    $$ = new Token("unsign_const");
                    $$->children.push_back($1);
                    $$->children.push_back($2);
                    $$->children.push_back($3);
                };

%%


void yyerror(const char *s){
	haveSemanticError = true;//错误标志，含有语法错误
	string errorInformation;//定义错误信息
	errorInformation += string(s);//添加错误信息
	errorInformation += ", location: " + itos(yylineno-1) + "." + itos(yycolumn-yyleng);//添加错误位置
	syntaxErrorInformation.push_back(errorInformation);//存放错误信息
}

void yyerror(const char *s, YYLTYPE *loc){//处理单个字符的错误
	haveSemanticError = true;
	string errorInformation;
	errorInformation = "syntax error, " + string(s) + ", location: " + itos(loc->first_line) + "." + itos(loc->first_column) + "-" + itos(loc->last_line) + "." + itos(loc->last_column);
	syntaxErrorInformation.push_back(errorInformation);
}

void yyerror(const char *s, int line, int col){//处理一行以内的错误
	haveSemanticError = true;
	string errorInformation;
	errorInformation = "syntax error, " + string(s) + ", location: " + itos(line) + "." + itos(col);
	syntaxErrorInformation.push_back(errorInformation);
}

void yyerror(const char *s, int startLine, int startCol, int endLine, int endCol){//处理涉及多行的错误
	haveSemanticError = true;
	string errorInformation;
	errorInformation = "syntax error, " + string(s) + ", location: " + itos(startLine) + "." + itos(startCol) + "-" + itos(endLine) + "." + itos(endCol);
	syntaxErrorInformation.push_back(errorInformation);
}

int main()
{
    printf("-----begin parsing\n");  
    yyparse();//使yacc开始读取输入和解析，它会调用lex的yylex()读取记号  
    puts("-----end parsing"); 
}