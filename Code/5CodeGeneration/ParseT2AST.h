#ifndef __PARSET2AST_H__
#define __PARSET2AST_H__

#include "main.h"
#include "ASTnodes.h"


int str2int(string str);
float str2float(string str);
_Program* getProgram(Token *now);
void getProgramHead(Token *now, pair<string, int>& _programId, vector< pair<string, int> >& _paraList);
_SubProgram* getProgramBody(Token *now);
void getIdList(Token *now, vector< pair<string, int> >& res,bool reverseFlag);
void setConst(Token *now, _Constant* &_constant);
void getConst(Token *now, vector<_Constant*>& _constantList);
void getConstList(Token *now, vector<_Constant*>& _constantList);
void getVariantList(Token *now,vector<_Variant*>& _variantList);
void getVariant(Token *now,vector<_Variant*>& _variantList);
void getTypeDefList(Token *now,vector<_TypeDef*>& _typedefList);
void getTypeDef(Token *now,vector<_TypeDef*>& _typedefList);
_Type* getType(Token *now);
void getrecordBody(Token *now,vector<_Variant*> &recordList);
void getArrayRangeList(Token *now,vector< pair<int,int> >& _arrayRangeList);
void getArrayRange(Token *now,vector< pair<int,int> >& _arrayRangeList);
void getSubprogramDefinitionList(Token *now,vector<_FunctionDefinition*>& _subprogramDefinitionList);
_FunctionDefinition* getSubprogramDefinition(Token *now);
void getSubprogramHead(Token *now,pair<string,int>& functionID,vector<_FormalParameter*>& _formalParaList,pair<string,int> &_type);
void getFormalParaList(Token *now,vector<_FormalParameter*>& _formalParaList);
void getFormalParameters(Token *now,vector<_FormalParameter*>& _formalParaList);
void getParameter(Token *now,vector<_FormalParameter*>& _formalParaList);
void getValueParameter(Token *now,vector<_FormalParameter*>& _formalParaList,int flag);
void getSubprogramBody(Token *now,vector<_Constant*>& _constList,vector<_TypeDef*> typedefList,vector<_Variant*>& _variantList,vector<_FunctionDefinition*> & subprogramDefinitionList,_Compound* &_compound);
_Compound* getCompoundStatement(Token *now);
void getStatementList(Token *now,vector<_Statement*>& _statementList);
_Statement* getStatement(Token *now);
void getforStatementOperation(Token *now,_ForStatement* &_forStatement);
_Statement* getElseStatement(Token *now);
void getidVpartList(Token *now,vector<_Idvpart*> &idvpartlist);
void getVpart(Token *now,vector<_Idvpart*> &idvpartlist);
_Statement* getProcedureCall(Token *now);
void getExpressionList(Token *now,vector<_Expression*>& _expressionList);
_Expression* getExpression(Token *now);
_Expression* getSimpleExpression(Token *now);
_Expression* getTerm(Token *now);
_Expression* getFactor(Token *now);
void getCaseBody(Token *now,_CaseStatement* &_caseStatement);
void getBranchlist(Token *now,_CaseStatement* &_caseStatement);
void getBranch(Token *now,_CaseStatement* &_caseStatement);
void getConstlist(Token *now,vector<_Constant *> &constlist);
void const2exp(_Constant * constant,_Expression *& exp);
void dfsAST(_Program* ASTRoot);
void dfssubP(_SubProgram* subProgram);
void dfsconstlist(vector<_Constant*> constList);
void dfstypedefList(vector<_TypeDef*> typedefList);
void printType(_Type *type);
void dfsvariantList(vector<_Variant*> variantList);
void dfssubprogramDefinitionList(vector<_FunctionDefinition*> subprogramDefinitionList);
void dfscompound(_Compound *compound);
void printidvpart(vector<_Idvpart*> idvpartlist);
void printExp(_Expression* expression);
void printfuncall(_FunctionCall* functionCall);
void printstatement(_Statement *statement);
string getUINUM(int &intTarget,float &floatTarget,string value);
string getUFNUM(int &intTarget,float &floatTarget,string value);
#endif
