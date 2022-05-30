#ifndef SEMANTICANALYSE_H
#define SEMANTICANALYSE_H

#include <string>
#include <vector>
#include <iostream>

#include <sstream>
#include <set>
#include "symbolTable.h"
#include "ASTnodes.h"
#include "main.h"


int isLibName(string id);		   //检查是否与库函数、主程序同名
bool isReDef(string id, int &loc); //检查是否重定义
void SemanticAnalyse(_Program *ASTRoot);
void createSymbolTableAndInit(); //创建主符号表并初始化

void SemanticAnalyseSubprogram(_SubProgram *subprogram);						   //对分程序进行语义分析
void SemanticAnalyseProgram(_Program *program);									   //对程序进行语义分析
void SemanticAnalyseConst(_Constant *constant);									   //对常量定义进行语义分析
void SemanticAnalyseTypedef(_TypeDef *typedefi);								   //对自定义进行语义分析
void SemanticAnalyseVariant(_Variant *variant);									   //对变量定义进行语义分析
void SemanticAnalyseSubprogramDefinition(_FunctionDefinition *functionDefinition); //对子程序定义进行语义分析
void SemanticAnalyseFormalParameter(_FormalParameter *formalParameter);			   //对形式参数进行语义分析
void SemanticAnalyseStatement(_Statement *statement);							   //对语句进行语义分析

vector<_SymbolRecord *> SemanticAnalyseRecord(vector<_Variant *> recordList, pair<string, int> VID, int is_type); //对record类型进行语义分析

string SemanticAnalyseVariantReference(_VariantReference *variantReference); //对变量引用进行语义分析
string SemanticAnalyseFunctionCall(_FunctionCall *functionCall, int line);			 //对函数调用进行语义分析
string SemanticAnalyseExpression(_Expression *expression);					 //对表达式进行语义分析

void relocation(); //重定位

string itos(int num); //将int转化为string

//添加重定义错误信息
void addDuplicateDefinitionErrorInformation(string preId, int preLineNumber, string preFlag, string preType, int curLineNumber); //获得重复定义的语义错误信息
// 添加未定义错误信息
void addUndefinedErrorInformation(string id, int curLineNumber);
// 添加标识符类型错误信息
void addUsageTypeErrorInformation(string curId, int curLineNumber, string curType, string usage, string correctType);
//添加个数不匹配错误信息
void addNumberErrorInformation(string curId, int curLineNumber, int curNumber, int correctNumber, string usage);
// 添加标识符种类错误信息
void addPreFlagErrorInformation(string curId, int curLineNumber, string curFlag, int preLineNumber, string preFlag);
//添加表达式类型错误信息
void addExpressionTypeErrorInformation(_Expression *exp, string curType, string correctType, string description);
// 添加赋值语句左值和右值类型不匹配错误信息
void addAssignTypeMismatchErrorInformation(_VariantReference *leftVariantReference, _Expression *rightExpression);
//添加数组下标越界错误信息
void addArrayRangeOutOfBoundErrorInformation(_Expression *expression, string arrayId, int X, pair<int, int> range);
// //添加数组下界比上界大的错误信息
// void addArrayRangeUpSideDownErrorInformation(string curId, int curLineNumber, int X, int lowBound, int highBound);
//添加运算符两边的操作数类型不一致的错误信息
void addOperandExpressionsTypeMismatchErrorInformation(_Expression *exp1, _Expression *exp2);
//添加某个操作数类型错误的信息
void addSingleOperandExpressionTypeMismatchErrorInformation(_Expression *exp, string correctType);
// 添加read的实参错误信息
void addactualParameterOfReadErrorInformation(int curLineNumber, string procedureId, int X, _Expression *exp);
//添加除0错误信息
void addDivideZeroErrorInformation(string operation, _Expression *exp);
// 添加read读取boolean类型变量错误的信息
// void addReadBooleanErrorInformation(_Expression *exp, int X);
//添加变量引用错误信息
void addVariantReferenceErrorInformation(int line, string info);
//将错误信息直接添加到错误信息的列表中
void addGeneralErrorInformation(string errorInformation);

#endif