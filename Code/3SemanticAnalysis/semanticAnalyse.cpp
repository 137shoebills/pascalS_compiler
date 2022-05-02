/*
语义分析实现代码
*/
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <set>
#include "symbolTable.h"
#include "ASTnodes.h"
#include "main.h"

using namespace std;

extern _SymbolTable *mainSymbolTable;																								//主符号表
extern _SymbolRecord *findSymbolRecord(_SymbolTable *currentSymbolTable, string id);												//从符号表中找出id对应的记录
extern void inputFunctionCall(_FunctionCall *functionCallNode, string &functionCall, int mode = 0);									//获取函数调用
extern int inputExpression(_Expression *expressionNode, string &expression, int mode = 0, bool isReferedActualPara = false);		//获取表达式
extern void inputVariantRef(_VariantReference *variantRefNode, string &variantRef, int mode = 0, bool isReferedActualPara = false); //获取变量引用
extern int str2int(string str);
extern void returnExistedCheckFunctionDefinition(_FunctionDefinition *functionDefinitionNode);

vector<string> semanticErrorInformation;   //存储错误信息的列表
vector<string> semanticWarningInformation; //存储警告信息的列表

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

string SemanticAnalyseVariantReference(_VariantReference *variantReference); //对变量引用进行语义分析
string SemanticAnalyseFunctionCall(_FunctionCall *functionCall);			 //对函数调用进行语义分析
string SemanticAnalyseExpression(_Expression *expression);					 //对表达式进行语义分析

void relocation(); //重定位

string itos(int num); //将int转化为string

//我把他原有的注释函数注释了，如果需要的话大家拿出来重写一些即可

//添加重定义错误信息
void addDuplicateDefinitionErrorInformation(string preId, int preLineNumber, string preFlag, string preType, int curLineNumber); //获得重复定义的语义错误信息
// //添加未定义错误信息
// void addUndefinedErrorInformation(string id, int curLineNumber);
// //添加标识符类型错误信息
// void addUsageTypeErrorInformation(string curId, int curLineNumber, string curType, string usage, string correctType);
// //添加个数不匹配错误信息
// void addNumberErrorInformation(string curId, int curLineNumber, int curNumber, int correctNumber, string usage);
// //添加标识符种类错误信息
// void addPreFlagErrorInformation(string curId, int curLineNumber, string curFlag, int preLineNumber, string preFlag);
// //添加表达式类型错误信息
// void addExpressionTypeErrorInformation(_Expression *exp, string curType, string correctType, string description);
// //添加赋值语句左值和右值类型不匹配错误信息
// void addAssignTypeMismatchErrorInformation(_VariantReference *leftVariantReference, _Expression *rightExpression);
// //添加数组下标越界错误信息
// void addArrayRangeOutOfBoundErrorInformation(_Expression *expression, string arrayId, int X, pair<int,int> range);
// //添加数组下界比上界大的错误信息
// void addArrayRangeUpSideDownErrorInformation(string curId, int curLineNumber, int X, int lowBound, int highBound);
// //添加运算符两边的操作数类型不一致的错误信息
// void addOperandExpressionsTypeMismatchErrorInformation(_Expression *exp1, _Expression *exp2);
// //添加某个操作数类型错误的信息
// void addSingleOperandExpressionTypeMismatchErrorInformation(_Expression *exp, string correctType);
// //添加read的实参错误信息
// void addactualParameterOfReadErrorInformation(int curLineNumber, string procedureId, int X, _Expression *exp);
// //添加除0错误信息
// void addDivideZeroErrorInformation(string operation, _Expression *exp);
// //添加read读取boolean类型变量错误的信息
// void addReadBooleanErrorInformation(_Expression *exp, int X);
//将错误信息直接添加到错误信息的列表中
void addGeneralErrorInformation(string errorInformation);

void SemanticAnalyse(_Program *ASTRoot)
{
	createSymbolTableAndInit();
	SemanticAnalyseProgram(ASTRoot);
}

void createSymbolTableAndInit()
{										  //创建主符号表
	mainSymbolTable = new _SymbolTable(); //指定为主符号表后，会自动加入read, write等库函数
}

//对程序进行语义分析
void SemanticAnalyseProgram(_Program *program)
{
	if (program == NULL)
	{
		cout << "[SemanticAnalyseProgram] pointer of _Program is null" << endl;
		return;
	}
	//库函数名、主程序名、主程序参数，在检查是否重定义时，优先级按照前面列举的顺序，
	//即主程序名不能和库函数名，主程序参数不能和库函数名、主程序名同名
	//添加主程序名、行号、参数个数等信息
	set<string> lib;
	lib.insert("read");
	lib.insert("write");
	//lib.insert("writeln");
	//lib.insert("exit");
	if (lib.count(program->programId.first)) //检查有没有用到库函数中的名字
		addGeneralErrorInformation("[Duplicate defined error!] <Line " + itos(program->programId.second) + "> Name of program \"" + program->programId.first + "\" has been defined as a lib program.");

	mainSymbolTable->addProgram(program->programId.first, program->programId.second, int(program->paraList.size()), "");

	//将主程序的参数添加到主符号表中，flag定为"parameter of program"
	for (int i = 0; i < program->paraList.size(); i++)
	{
		if (program->paraList[i].first == program->programId.first) //检查主程序参数是否和主程序名同名
			addGeneralErrorInformation("[Duplicate defined error!] <Line " + itos(program->programId.second) + "> parameter of program \"" + program->programId.first + "\" is the same as name of program.");
		else if (lib.count(program->paraList[i].first)) //检查主程序参数是否和库函数名同名
			addGeneralErrorInformation("[Dulicate defined error!] <Line " + itos(program->paraList[i].second) + "> parameter of program \"" + program->paraList[i].first + "\" has been defined as a lib program.");
		mainSymbolTable->addVoidPara(program->paraList[i].first, program->paraList[i].second);
	}

	//主符号表需提前加入read、write、exit等库函数
	//对于库函数来说，setProcedure的参数,lineNumber,amount是没有用的
	// lineNumber=-1表示是库函数
	// amount=-1表示变参
	//添加read过程，该过程变参
	mainSymbolTable->addProcedure("read", -1, -1);
	//添加write过程，该过程变参
	mainSymbolTable->addProcedure("write", -1, -1);
	//添加writeln过程，该过程变参
	//mainSymbolTable->addProcedure("writeln", -1, -1);
	//添加exit过程，该过程的参数个数需要分情况讨论，程序里会有特判，这里指定为0没有特殊含义
	//mainSymbolTable->addProcedure("exit", -1, 0);
	SemanticAnalyseSubprogram(program->subProgram);
}

//对子program进行语义分析
void SemanticAnalyseSubprogram(_SubProgram *subprogram)
{
	if (subprogram == NULL)
	{ //分程序的指针为空
		cout << "[SemanticAnalyseSubprogram] pointer of _Subprogram is null" << endl;
		return;
	}
	for (int i = 0; i < subprogram->constList.size(); i++)
		SemanticAnalyseConst(subprogram->constList[i]);
	for (int i = 0; i < subprogram->typedefList.size(); i++)
		SemanticAnalyseTypedef(subprogram->typedefList[i]);
	for (int i = 0; i < subprogram->variantList.size(); i++)
		SemanticAnalyseVariant(subprogram->variantList[i]);
	for (int i = 0; i < subprogram->subprogramDefinitionList.size(); i++)
	{
		SemanticAnalyseSubprogramDefinition(subprogram->subprogramDefinitionList[i]);
		relocation();
	}
	SemanticAnalyseStatement(reinterpret_cast<_Statement *>(subprogram->compound));
}

//对常量定义进行语义分析
void SemanticAnalyseConst(_Constant *constant)
{
	if (constant == NULL)
	{
		cout << "[SemanticAnalyseConst] pointer of _Constant is null" << endl;
		return;
	}
	std::pair<string, int> CID = constant->constId;
	if (mainSymbolTable->idToLoc.count(CID.first)) //判断id是否已经使用过
	{
		semanticErrorInformation.push_back((string) "line:" + char('0' + CID.second) + "Error: Duplicate identifier" + CID.first);
		return;
	}
	if (constant->type == "integer") //若常量整数值超出int范围，则将其化为float
	{
		long long maxint = 2147483647;
		long long res = 0;
		int len = int(constant->strOfVal.length());
		bool ff = 0;
		for (int i = 0; i < len; ++i)
		{
			res = res * 10;
			res += int(constant->strOfVal[i] - '0');
			if ((res > maxint && !constant->isMinusShow) || res > (maxint + 1))
			{
				ff = 1;
				break;
			}
		}
		if (ff)
		{
			float z = 0;
			for (int i = 0; i < len; ++i)
			{
				z = z * 10;
				z += int(constant->strOfVal[i] - '0');
			}
			constant->type = "real";
			constant->realValue = z;
		}
	}
	mainSymbolTable->addConst(CID.first, CID.second, constant->type, constant->isMinusShow, constant->strOfVal);
}

//对自定义类型进行语义分析
void SemanticAnalyseTypedef(_TypeDef *typedefi)
{
	if (typedefi == NULL)
	{
		cout << "[SemanticAnalyseTypedef] pointer of _TypeDef is null" << endl;
		return;
	}
	std::pair<string, int> TID = typedefi->typedefId;
	if (mainSymbolTable->idToLoc.count(TID.first))
	{
		semanticErrorInformation.push_back((string) "line:" + char('0' + TID.second) + "Error: Duplicate identifier" + TID.first);
		return;
	}
}

//对变量定义进行语义分析
void SemanticAnalyseVariant(_Variant *variant)
{
	if (variant == NULL)
	{
		cout << "[SemanticAnalyseVariant] pointer of _Variant is null" << endl;
		return;
	}
	std::pair<string, int> VID = variant->variantId;
	if (mainSymbolTable->idToLoc.count(VID.first))
	{
		semanticErrorInformation.push_back((string) "line:" + char('0' + VID.second) + "Error: Duplicate identifier" + VID.first);
		return;
	}
	if (variant->type->flag)
		mainSymbolTable->addArray(VID.first, VID.second, variant->type->type.first, variant->type->arrayRangeList.size(), variant->type->arrayRangeList);
	else
		mainSymbolTable->addVar(VID.first, VID.second, variant->type->type.first);
}

//对子程序定义进行语义分析
void SemanticAnalyseSubprogramDefinition(_FunctionDefinition *functionDefinition)
{
	if (functionDefinition == NULL)
	{
		cout << "[SemanticAnalyseSubprogramDefinition] pointer of _FunctionDefinition is null" << endl;
		return;
	}

	_SymbolRecord *record = findSymbolRecord(mainSymbolTable, functionDefinition->functionID.first);
	if (record != NULL) //重定义检查
	{
		addDuplicateDefinitionErrorInformation(record->id, record->lineNumber, record->flag, record->type, functionDefinition->functionID.second);
		return;
	}

	string subprogramType;
	if (functionDefinition->type.first == "")
		subprogramType = "procedure";
	else
		subprogramType = "function";

	//根据type是否为NULL，分为addProcedure()和addFunction()，添加到主程序表中
	if (functionDefinition->type.first == "") //如果是过程
		mainSymbolTable->addProcedure(functionDefinition->functionID.first, functionDefinition->functionID.second, int(functionDefinition->formalParaList.size()));
	else //如果是函数
		mainSymbolTable->addFunction(functionDefinition->functionID.first, functionDefinition->functionID.second, functionDefinition->type.first, int(functionDefinition->formalParaList.size()),functionDefinition->formalParaList);

	//对形式参数列表进行语义分析，并将形式参数添加到子符号表中
	for (int i = 0; i < functionDefinition->formalParaList.size(); i++)
		SemanticAnalyseFormalParameter(functionDefinition->formalParaList[i]);
	//对常量定义进行语义分析
	for (int i = 0; i < functionDefinition->constList.size(); i++)
		SemanticAnalyseConst(functionDefinition->constList[i]);
	//对自定义类型进行语义分析
	for (int i = 0; i < functionDefinition->typedefList.size(); i++)
		SemanticAnalyseTypedef(functionDefinition->typedefList[i]);
	//对变量定义进行语义分析
	for (int i = 0; i < functionDefinition->variantList.size(); i++)
		SemanticAnalyseVariant(functionDefinition->variantList[i]);
	//对compound进行语义分析
	SemanticAnalyseStatement(reinterpret_cast<_Statement *>(functionDefinition->compound));
}

//对形式参数进行语义分析，形式参数一定是基本类型
void SemanticAnalyseFormalParameter(_FormalParameter *formalParameter)
{
	if (formalParameter == NULL)
	{
		cout << "[SemanticAnalyseFormalParameter] pointer of _FormalParameter is null" << endl;
		return;
	}
	std::pair<string, int> PID = formalParameter->paraId;
	if (mainSymbolTable->idToLoc.count(PID.first))
	{
		semanticErrorInformation.push_back((string) "line:" + char('0' + PID.second) + "Error: Duplicate identifier" + PID.first);
		return;
	}
	if (formalParameter->flag == 0) //传值调用
	{
		mainSymbolTable->addPara(PID.first, PID.second, formalParameter->type);
	}
	else if (formalParameter->flag == 1) //引用调用
	{
		mainSymbolTable->addVarPara(PID.first, PID.second, formalParameter->type);
	}
	else
	{ //无法识别的调用类型
		semanticErrorInformation.push_back((string) "line:" + char('0' + PID.second) + "Error: Unrecognized call type.");
	}
	return;
}

//对语句进行语义分析
void SemanticAnalyseStatement(_Statement *statement)
{
	if (statement == NULL)
	{
		cout << "[SemanticAnalyseStatement] pointer of _Statement is null" << endl;
		return;
	}
	if (statement->type == "compound")
	{
		_Compound *compound = reinterpret_cast<_Compound *>(statement); //对复合语句块中的每一条语句进行语义分析
		for (int i = 0; i < compound->statementList.size(); i++)
			SemanticAnalyseStatement(compound->statementList[i]);
	}
	else if (statement->type == "repeat")
	{

		// SemanticAnalyseStatement(repeatStatement->_do);//对循环体语句进行语义分析
	}
	else if (statement->type == "while")
	{

		// SemanticAnalyseStatement(whileStatement->_do);//对循环体语句进行语义分析
	}
	else if (statement->type == "for")
	{

		//对循环体语句进行语义分析
		// SemanticAnalyseStatement(forStatement->_do);
	}
	else if (statement->type == "if")
	{

		// SemanticAnalyseStatement(ifStatement->then);//对then语句进行语义分析
		// if(ifStatement->els!=NULL)//对else语句进行语句分析
		//     SemanticAnalyseStatement(ifStatement->els);
	}
	else if (statement->type == "assign")
	{ //左值特判
	}
	else if (statement->type == "procedure")
	{ // read的参数只能是变量或数组元素; 这段比较难写
	}
}

//对函数调用进行语义分析
string SemanticAnalyseFunctionCall(_FunctionCall *functionCall)
{
	if (functionCall == NULL)
	{
		cout << "[SemanticAnalyseFunctionCall] pointer of _FunctionCall is null" << endl;
		return "";
	}

	std::pair<string, int> FCID = functionCall->functionId;
	if (mainSymbolTable->idToLoc.count(FCID.first) == 0) //找不到函数声明
	{
		semanticErrorInformation.push_back((string) "line:" + char('0' + FCID.second) + "Error: Identifier not found " + FCID.first);
		return;
	}

	int decID = mainSymbolTable->idToLoc[FCID.first].top();
	functionCall->returnType = mainSymbolTable->recordList[decID]->type;

	int decParaSize = mainSymbolTable->recordList[decID]->amount;
	if (decParaSize != functionCall->actualParaList.size()) //参数表数目不对
	{
		semanticErrorInformation.push_back((string) "line:" + char('0' + FCID.second) + "Error: Wrong number of parameters specified for call to " + FCID.first);
	}

	int ParaSize = functionCall->actualParaList.size();
	for (int i = 0; i < ParaSize; ++i) //判断每个参数的类型
	{
		SemanticAnalyseExpression(functionCall->actualParaList[i]);
		string decType = mainSymbolTable->recordList[decID + i + 1]->type;
		string expType = functionCall->actualParaList[i]->expressionType;
		if (expType != decType && !(expType == "integer" && decType == "real"))
		{
			semanticErrorInformation.push_back((string) "line:" + char('0' + FCID.second) + "Error: Incompatible type for arg no. " + char('1' + i) + ": Got \"" + expType + "\", expected \"" + decType + "\"");
		}
	}
	return;
}

//对表达式进行语义分析
string SemanticAnalyseExpression(_Expression *expression)
{
	if (expression == NULL)
	{
		cout << "[SemanticAnalyseExpression] pointer of _Expression is null" << endl;
		return "";
	}
}

string SemanticAnalyseVariantReference(_VariantReference *variantReference)
{
	if (variantReference == NULL)
	{
		cout << "[SemanticAnalyseVariantReference] pointer of _VariantReference is null" << endl;
		return "";
	}
}

//符号表重定位
void relocation()
{
	int top = mainSymbolTable->indexTable.back(); //此时最近的block索引位置
	int sizeTable = mainSymbolTable->recordList.size();
	for (int i = sizeTable - 1; i > top; i--)
	{
		string id = mainSymbolTable->recordList[i]->id;
		if (mainSymbolTable->idToLoc[id].size() > 0 && mainSymbolTable->idToLoc[id].top() >= top)
		{
			mainSymbolTable->idToLoc[id].pop();
		}
		if (mainSymbolTable->custom[id].size() > 0 && mainSymbolTable->custom[id].top() >= top)
		{
			mainSymbolTable->custom[id].pop();
		}
		mainSymbolTable->recordList.pop_back();
	}
	mainSymbolTable->indexTable.pop_back();
}

void addDuplicateDefinitionErrorInformation(string preId, int preLineNumber, string preFlag, string preType, int curLineNumber)
{
	string errorInformation = "[Duplicate defined error!] <Line " + itos(curLineNumber) + "> ";
	if (preLineNumber != -1)
		errorInformation += "\"" + preId + "\"" + " has already been defined as a " + preFlag + " at line " + itos(preLineNumber) + ".";
	else
		errorInformation += "\"" + preId + "\"" + " has already been defined as a lib program.";
	semanticErrorInformation.push_back(errorInformation);
	// CHECK_ERROR_BOUND
}