#ifndef ASTNODES_H
#define ASTNODES_H

#include<llvm/IR/Value.h>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;
class _Program;
class _SubProgram;
class _Constant;
class _Variant;
class _Type;
class _TypeDef;
class _FunctionDefinition;
class _FormalParameter;
class _Compound;
class _VariantReference;
class _Statement;
class _Expression;
class _FunctionCall;
class _AssignStatement;
class _IfStatement;
class _ForStatement;
class _WhileStatement;
class _RepeatStatement;
class _ProcedureCall;
class _Idvpart;
class _CaseStatement;
class _Branch;

class _Program //程序(相当于program_head)
{
public:
    pair<string, int> programId;         // PASCAL程序名称标识符及行号
    vector<pair<string, int> > paraList; // PASCAL程序参数列表及行号
    _SubProgram *subProgram;

public:
    _Program();
    ~_Program();
};

class _SubProgram //分程序(相当于program_body)
{
public:
    vector<_Constant *> constList;                          //常数定义列表
    vector<_Variant *> variantList;                         //变量定义列表
    vector<_TypeDef *> typedefList;                         //重构定义列表
    vector<_FunctionDefinition *> subprogramDefinitionList; //子程序和子函数定义列表
    _Compound *compound;                                    //主程序体
public:
    _SubProgram();
    ~_SubProgram();
};

class _Constant //常量定义
{               //常量定义的时候，右值可以是已经定义好的常量标识符
                //在代码生成的时候，可以根据常量的范围对类型进行进一步的细化
public:
    pair<string, int> constId;
    string type; //常数类型,分为"id","integer","real","char"

    pair<string, int> valueId;
    char charValue;
    int intValue;
    float realValue;
    bool boolvalue;
    string strOfVal;  //所有常量取值的字符串表示
    bool isMinusShow; //是否为负数
public:
    _Constant() {}
    ~_Constant() {}
    llvm::Value* codeGen();
};

class _TypeDef //变量定义
{
public:
    pair<string, int> typedefId; //变量标识符ID及行号
    _Type *type;                 //变量类型
public:
    _TypeDef();
    _TypeDef(pair<string, int> _typeDefId, _Type *_type);
    ~_TypeDef();
    void codeGen();
};

class _Type //类型
{
public:
    pair<string, int> type;                 //基本类型及行号 "integer"、"char"、"real"、"boolean"
    int flag;                               // 0表示非数组，1表示数组
    vector<pair<int, int> > arrayRangeList; // flag=1时，表示数组各维上下界
    vector<_Variant *> recordList;

public:
    _Type();
    _Type(pair<string, int> _type, int _flag, vector<pair<int, int> > _arrayRangeList);
    ~_Type() {}

    //创建数组的LLVM类型
    llvm::Type* _Type::InitArrayType(string arrTypeName, string type);  //arrTypeName:自定义类型名，type:数组元素类型
    //创建record的LLVM类型
    llvm::Type* _Type::InitRecordType(string recTypeName);  //recTypeName:自定义类型名
};

class _Variant //变量定义
{
public:
    pair<string, int> variantId; //变量标识符ID及行号
    _Type *type;                 //变量类型
public:
    _Variant();
    _Variant(pair<string, int> _variantId, _Type *_type);
    ~_Variant();
    llvm::Value* codeGen();
};

class _FunctionDefinition
{ //函数/过程定义
public:
    pair<string, int> functionID;              //函数/过程标识符及行号
    vector<_FormalParameter *> formalParaList; //形式参数列表
    pair<string, int> type;                    //如果type.first是空串，则为过程，否则为函数,取值为"integer","real","boolean","char"四种
    vector<_Constant *> constList;             //常数定义列表
    vector<_TypeDef *> typedefList;            //重构定义列表
    vector<_Variant *> variantList;            //变量定义列表

    vector<_FunctionDefinition *> subprogramDefinitionList; //子程序和子函数定义列表
    _Compound *compound;                                    //主程序体
public:
    _FunctionDefinition();
    ~_FunctionDefinition();
    llvm::Value* codeGen(_SymbolRecord*);
};

class _FormalParameter //形式参数
{
public:
    pair<string, int> paraId; //形式参数标识符和行号
    string type;              //形式参数类型
    int flag;                 // flag=0表示传值调用，flag=1表示引用调用
public:
    _FormalParameter();
    _FormalParameter(pair<string, int> _paraId, string _type, int _flag);
    ~_FormalParameter() {}
    llvm::Value* codeGen();
};

class _Statement
{
public:
    string type;            //"compound","repeat","while","for","if","assign","procedure"
    string statementType;   //区别于type，取值为"void"或"error"
    int lineNo;             //行号
    bool isReturnStatement; //是否是返回值语句
public:
    _Statement() {}
    ~_Statement() {}
    llvm::Value* codeGen();
};

class _Compound : public _Statement
{
public:
    vector<_Statement *> statementList; //语句列表
                                        //行号由begin的位置决定
public:
    _Compound();
    ~_Compound();
};

class _AssignStatement : public _Statement
{
public:
    _VariantReference *variantReference; //左值变量
    _Expression *expression;             //右值表达式
                                         //行号由赋值符号的位置决定
public:
    _AssignStatement();
    ~_AssignStatement();
    llvm::Value* codeGen(string leftType, string rightType);
};

class _ProcedureCall : public _Statement
{
public:
    pair<string, int> procedureId;        //过程标识符
    vector<_Expression *> actualParaList; //实际参数列表，由表达式组成
                                          //行号由procedure名的位置决定
public:
    _ProcedureCall();
    ~_ProcedureCall();
    void codeGen();
};

class _FunctionCall
{
public:
    pair<string, int> functionId;         //函数标识符
    vector<_Expression *> actualParaList; //实际参数列表，由表达式组成
    string returnType;                    //"integer"、"real"、"char"、"boolean"、"error"，其中error表示函数标识符不存在
public:
    _FunctionCall();
    ~_FunctionCall();
    llvm::Value* codeGen();
};

class _Expression
{
public:
    string type; //表达式类型,"var"表示变量,"integer"表示整数,"real"表示浮点数,"char"表示常量字符
    //"function"表示函数调用,"compound"表示复合表达式,
    // compound有普通的二目运算符，还有"minus"、"not"、"bracket"等单目运算符

    _VariantReference *variantReference; //变量或常量或数组

    int intNum; //整数

    float realNum; //浮点数

    string strOfNum; //整数和浮点数的string表示（考虑从PASCAL-S源程序将字符串转为浮点数，再将浮点数转为字符串会带来精度问题，所以需要存下初始字符串值）

    char charVal; //常量字符

    _FunctionCall *functionCall; //函数调用

    int isMinusShow; //是否为负

    string operation;     //具体操作符
    string operationType; //操作符类型,"relop","mulop","addop","single"
    _Expression *operand1, *operand2;

    int lineNo; //行号, 用表达式中最先出现的操作数的行号表示
public:
    _Expression();
    ~_Expression();
    llvm::Value* codeGen();
    //语义分析相关
public:
    int totalIntValue;
    bool totalIntValueValid;
    string expressionType; //区别于type，这个值表示表达式的具体类型，即"integer"、"real"、"char"、"boolean"、"error"，其中error表示表达式中包含类型不一致的操作数
};

class _VariantReference
{
public:
    pair<string, int> variantId; //变量或常量标识符和行号
    //如果这个变量是结构体或数组：
    vector<_Idvpart *> IdvpartList; //结构体.属性或数组元素
    //IdvpartList:考虑多层访问  a.b.c; a[b].c

public:
    _VariantReference();
    ~_VariantReference();
    llvm::Value* codeGen();

public:
    int locFlag;        //-1表示左值，1表示右值，0表示什么都不是 左值特判
    string kind;        //"array","var","constant","function call","function return reference"
    string variantType; //"integer"、"real"、"char"、"boolean"、"error"，其中"error"表示数组某一维下标表达式的类型不为"integer"或标识符不存在
};

class _Idvpart
{
public:
    vector<_Expression *> expressionList; // flag = 0;
    pair<string, int> IdvpartId;          // flag = 1
    int flag;                             // flag=0表示数组
public:
    _Idvpart();
    ~_Idvpart();
    llvm::Value* codeGen(_SymbolRecord* variant);
};

class _IfStatement : public _Statement
{
public:
    _Expression *condition; //条件表达式
    _Statement *then;       //满足条件时执行的语句
    _Statement *els;        //不满足条件时执行的语句，如果为NULL，则没有else部分
                            //行号由if的位置决定
public:
    _IfStatement();
    ~_IfStatement();
    llvm::Value* codeGen();
};
class _CaseStatement : public _Statement
{
public:
    _Expression *caseid;
    vector<_Branch *> branch;
    vector<_Statement *> _do;
    _CaseStatement();
    ~_CaseStatement();
};
class _Branch
{
public:
    vector<_Expression *> condition;
    _Statement* _do;
    _Branch();
    ~_Branch();
};

class _ForStatement : public _Statement
{
public:
    pair<string, int> id; //循环变量
    _Expression *start;   //起始值
    _Expression *end;     //终止值
    _Statement *_do;      //循环体语句
                          //行号由for的位置决定
public:
    _Expression *condition;//判断条件
    _AssignStatement *increment;//增量
    _AssignStatement *initial;//初始化
    _ForStatement();
    ~_ForStatement();
    llvm::Value* codeGen();
};


class _RepeatStatement : public _Statement
{
public:
    _Expression *condition;   //条件表达式
    vector<_Statement *> _do; //循环体语句
                              //行号由repeat的位置决定
public:
    _RepeatStatement();
    ~_RepeatStatement();
    llvm::Value* codeGen();
};

class _WhileStatement : public _Statement
{
public:
    _Expression *condition; //条件表达式
    _Statement *_do;        //循环体语句
                            //行号由while的位置决定
public:
    _WhileStatement();
    ~_WhileStatement();
    llvm::Value* codeGen();
};
#endif
