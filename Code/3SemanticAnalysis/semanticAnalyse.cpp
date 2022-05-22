   
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
// #include "CodeGen.h"

using namespace std;

int layer=0; //标记程序的嵌套层数

set<string> lib; //存放库函数名
extern _SymbolTable *mainSymbolTable;
extern _SymbolTable *currentSymbolTable;
// extern CodeGenContext context;																							//主符号表
extern _SymbolRecord *findSymbolRecord(_SymbolTable *currentSymbolTable, string id); //从符号表中找出id对应的记录
// extern void inputFunctionCall(_FunctionCall *functionCallNode, string &functionCall, int mode = 0);									//获取函数调用
// extern int inputExpression(_Expression *expressionNode, string &expression, int mode = 0, bool isReferedActualPara = false);		//获取表达式
// extern void inputVariantRef(_VariantReference *variantRefNode, string &variantRef, int mode = 0, bool isReferedActualPara = false); //获取变量引用
extern int str2int(string str);
// extern void returnExistedCheckFunctionDefinition(_FunctionDefinition *functionDefinitionNode);

vector<string> semanticErrorInformation;   //存储错误信息的列表
vector<string> semanticWarningInformation; //存储警告信息的列表

void SemanticAnalyse(_Program *ASTRoot);
void createSymbolTableAndInit(); //创建主符号表并初始化

void SemanticAnalyseSubprogram(_SubProgram *subprogram);									   //对分程序进行语义分析
void SemanticAnalyseProgram(_Program *program);												   //对程序进行语义分析
void SemanticAnalyseConst(_Constant *constant);												   //对常量定义进行语义分析
void SemanticAnalyseTypedef(_TypeDef *typedefi);											   //对自定义进行语义分析
void SemanticAnalyseVariant(_Variant *variant);												   //对变量定义进行语义分析
void SemanticAnalyseSubprogramDefinition(_FunctionDefinition *functionDefinition);			   //对子程序定义进行语义分析
void SemanticAnalyseFormalParameter(_FormalParameter *formalParameter);						   //对形式参数进行语义分析
void SemanticAnalyseStatement(_Statement *statement);										   //对语句进行语义分析

vector<_SymbolRecord *> SemanticAnalyseRecord(vector<_Variant *> recordList, pair<string, int> VID, int is_type); //对record类型进行语义分析

string SemanticAnalyseVariantReference(_VariantReference *variantReference); //对变量引用进行语义分析
string SemanticAnalyseFunctionCall(_FunctionCall *functionCall);			 //对函数调用进行语义分析
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
void addReadBooleanErrorInformation(_Expression *exp, int X);
//添加变量引用错误信息
void addVariantReferenceErrorInformation(int line,string info);
//将错误信息直接添加到错误信息的列表中
void addGeneralErrorInformation(string errorInformation);

void SemanticAnalyse(_Program *ASTRoot)
{
	createSymbolTableAndInit();
	// context.InitCodeGen();	 //初始化中间代码生成相关参数
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
	lib.clear();
	lib.insert("read");
	lib.insert("write");
	//库函数名、主程序名、主程序参数，在检查是否重定义时，优先级按照前面列举的顺序，
	//即主程序名不能和库函数名，主程序参数不能和库函数名、主程序名同名
	//添加主程序名、行号、参数个数等信息
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

	// lineNumber=-1表示是库函数
	// amount=-1表示变参
	//添加read过程，该过程变参
	vector<_FormalParameter *> tmp;
	mainSymbolTable->addProcedure("read", -1, -1, tmp);
	//添加write过程，该过程变参
	mainSymbolTable->addProcedure("write", -1, -1, tmp);

	SemanticAnalyseSubprogram(program->subProgram);

	cout << "code generation success" << endl;

	//[refer]这个pass是用来干嘛的？
	/* llvm::PassManager pm;
	pm.add(llvm::createPrintModulePass(llvm::outs()));
	pm.run(*(this->module.get())); */
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
	if (lib.count(CID.first)) //判断id是否为库函数
	{
		addDuplicateDefinitionErrorInformation(CID.first, -1, "", "", CID.second);
	}
	// else if (mainSymbolTable->idToLoc.count(CID.first)) //判断id是否已经使用过
	// {
	// 	int IDloc = mainSymbolTable->idToLoc[CID.first].top();
	// 	addDuplicateDefinitionErrorInformation(CID.first, mainSymbolTable->recordList[IDloc]->lineNumber, mainSymbolTable->recordList[IDloc]->flag, mainSymbolTable->recordList[IDloc]->type, CID.second);
	// 	return;
	// }
	else if (mainSymbolTable->idToLoc[CID.first].size() > 0) //如果此常量已经出现过
	{
		int loc = mainSymbolTable->idToLoc[CID.first].top(); //获取最近一次出现的位置
		if (mainSymbolTable->indexTable.back() < loc)		 //如果和当前变量在同一个作用域
		{
			addDuplicateDefinitionErrorInformation(CID.first, mainSymbolTable->recordList[loc]->lineNumber, mainSymbolTable->recordList[loc]->flag, mainSymbolTable->recordList[loc]->type, CID.second);
			return;
		}
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

	// codeGen
	int loc = mainSymbolTable->recordList.size() - 1;
	// llvm::Value* value = constant->codeGen();
	// mainSymbolTable->recordList[loc]->llValue = value;
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
	if (lib.count(TID.first)) //判断id是否为库函数
	{
		addDuplicateDefinitionErrorInformation(TID.first, -1, "", "", TID.second);
	}
	// else if (mainSymbolTable->idToLoc.count(TID.first)) //判断id是否已经使用过
	// {
	// 	int IDloc = mainSymbolTable->idToLoc[TID.first].top();
	// 	addDuplicateDefinitionErrorInformation(TID.first, mainSymbolTable->recordList[IDloc]->lineNumber, mainSymbolTable->recordList[IDloc]->flag, mainSymbolTable->recordList[IDloc]->type, TID.second);
	// 	return;
	// }
	else if (mainSymbolTable->idToLoc[TID.first].size() > 0) //如果此常量已经出现过
	{
		int loc = mainSymbolTable->idToLoc[TID.first].top(); //获取最近一次出现的位置
		if (mainSymbolTable->indexTable.back() < loc)		 //如果和当前变量在同一个作用域
		{
			addDuplicateDefinitionErrorInformation(TID.first, mainSymbolTable->recordList[loc]->lineNumber, mainSymbolTable->recordList[loc]->flag, mainSymbolTable->recordList[loc]->type, TID.second);
			return;
		}
	}

	if (typedefi->type->type.first == "record") //如果此时是record
	{
		SemanticAnalyseRecord(typedefi->type->recordList, TID, 1);
	}
	else if (typedefi->type->flag) //如果此时是数组
	{
		mainSymbolTable->addArray(TID.first, TID.second, typedefi->type->type.first, typedefi->type->arrayRangeList.size(), typedefi->type->arrayRangeList);
	}
	else
	{
		mainSymbolTable->addVar(TID.first, TID.second, typedefi->type->type.first);
		mainSymbolTable->custom[TID.first].push(int(mainSymbolTable->recordList.size() - 1));
	}
}

//对record类型进行语义分析
vector<_SymbolRecord *> SemanticAnalyseRecord(vector<_Variant *> recordList, pair<string, int> VID, int type)
{
	vector<_SymbolRecord *> records;
	map<string, int> ids;
	for (int i = 0; i < recordList.size(); i++)
	{
		_SymbolRecord *tmpRecord = new _SymbolRecord;
		if (recordList[i] == NULL)
		{
			cout << "[SemanticAnalyseRecord] pointer of _Variant is null" << endl;
			return records;
		}
		pair<string, int> aVID = recordList[i]->variantId;
		if (lib.count(aVID.first)) //判断id是否为库函数
		{
			addDuplicateDefinitionErrorInformation(aVID.first, -1, "", "", aVID.second);
		}
		// else if (mainSymbolTable->idToLoc.count(aVID.first)) //判断id是否已经使用过
		// {
		// 	int IDloc = mainSymbolTable->idToLoc[aVID.first].top();
		// 	addDuplicateDefinitionErrorInformation(aVID.first, mainSymbolTable->recordList[IDloc]->lineNumber, mainSymbolTable->recordList[IDloc]->flag, mainSymbolTable->recordList[IDloc]->type, aVID.second);
		// 	return records;
		// }
		else if (mainSymbolTable->idToLoc[aVID.first].size() > 0) //如果此常量已经出现过
		{
			int loc = mainSymbolTable->idToLoc[aVID.first].top(); //获取最近一次出现的位置
			if (mainSymbolTable->indexTable.back() < loc)		 //如果和当前变量在同一个作用域
			{
				addDuplicateDefinitionErrorInformation(aVID.first, mainSymbolTable->recordList[loc]->lineNumber, mainSymbolTable->recordList[loc]->flag, mainSymbolTable->recordList[loc]->type, aVID.second);
			}
		}

		if (recordList[i]->type->type.first == "record")
		{
			tmpRecord->setRecords(aVID.first+"_", aVID.second, SemanticAnalyseRecord(recordList[i]->type->recordList, aVID, 2));
			tmpRecord->setVar(aVID.first, aVID.second, aVID.first+"_");
		}
		else if (recordList[i]->type->flag)
		{
			tmpRecord->setArray(aVID.first+"_", aVID.second, recordList[i]->type->type.first, recordList[i]->type->arrayRangeList.size(), recordList[i]->type->arrayRangeList);
			records.push_back(tmpRecord);
			tmpRecord->setVar(aVID.first, aVID.second, aVID.first+"_");
		}
		else
			tmpRecord->setVar(aVID.first, aVID.second, recordList[i]->type->type.first);

		records.push_back(tmpRecord);
		ids[aVID.first] = aVID.second;
	}

	if (type == 0) //表示此时不是type中定义新类型，而是声明语句
	{
		mainSymbolTable->addRecords(VID.first + "_", VID.second, records); // id名后加_下划线表示record类型名
		mainSymbolTable->addVar(VID.first, VID.second, VID.first + "_");
	}
	else if(type == 2) //表示此时是嵌套record
	{
		return records;
	}
	else if (type == 1) //表示此时是自定义类型
	{
		mainSymbolTable->addRecords(VID.first, VID.second, records);
	}
	return records;
	// codeGen
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

	if (lib.count(VID.first)) //判断变量id是否与库函数同名
	{
		addDuplicateDefinitionErrorInformation(VID.first, -1, "", "", VID.second);
	}
	// else if (mainSymbolTable->idToLoc.count(VID.first)) //判断id是否已经使用过
	// {
	// 	int IDloc = mainSymbolTable->idToLoc[VID.first].top();
	// 	addDuplicateDefinitionErrorInformation(VID.first, mainSymbolTable->recordList[IDloc]->lineNumber, mainSymbolTable->recordList[IDloc]->flag, mainSymbolTable->recordList[IDloc]->type, VID.second);
	// 	return;
	// }
	else if(mainSymbolTable->idToLoc[VID.first].size() > 0) //如果此变量已经出现过
	{
		int loc = mainSymbolTable->idToLoc[VID.first].top(); //获取最近一次出现的位置
		if (mainSymbolTable->indexTable.back() < loc)	//如果和当前变量在同一个作用域
		{
			addDuplicateDefinitionErrorInformation(VID.first, mainSymbolTable->recordList[loc]->lineNumber, mainSymbolTable->recordList[loc]->flag, mainSymbolTable->recordList[loc]->type, VID.second);
			return;
		}
	}

	if (variant->type->type.first == "record")
	{
		SemanticAnalyseRecord(variant->type->recordList, VID, 0);
	}
	else if (variant->type->flag)
	{
		mainSymbolTable->addArray(VID.first+"_", VID.second, variant->type->type.first, variant->type->arrayRangeList.size(), variant->type->arrayRangeList);
		mainSymbolTable->addVar(VID.first, VID.second,VID.first+"_");
	}
	else
		mainSymbolTable->addVar(VID.first, VID.second, variant->type->type.first);

	// codeGen
	int loc = mainSymbolTable->recordList.size() - 1;
	// llvm::Value* value = variant->codeGen();
	// mainSymbolTable->recordList[loc]->llValue = value;
}

//对子程序定义进行语义分析
void SemanticAnalyseSubprogramDefinition(_FunctionDefinition *functionDefinition)
{
	layer++; //层数++
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

	//根据有无返回值确定是function还是procedure
	string subprogramType;
	if (functionDefinition->type.first == "")
		subprogramType = "procedure";
	else
		subprogramType = "function";

	//根据type是否为NULL，分为addProcedure()和addFunction()，添加到主程序表中
	if (subprogramType == "procedure") //如果是过程
		mainSymbolTable->addProcedure(functionDefinition->functionID.first, functionDefinition->functionID.second, int(functionDefinition->formalParaList.size()), functionDefinition->formalParaList);
	else //如果是函数
		mainSymbolTable->addFunction(functionDefinition->functionID.first, functionDefinition->functionID.second, functionDefinition->type.first, int(functionDefinition->formalParaList.size()), functionDefinition->formalParaList);

	int loc = mainSymbolTable->recordList.size() - 1;

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

	if(layer <= 3)
	{
		for(int i=0;i<functionDefinition->subprogramDefinitionList.size();i++)
			SemanticAnalyseSubprogramDefinition(functionDefinition->subprogramDefinitionList[i]);		
	}
	else
	{
		addGeneralErrorInformation("too much layers in" + functionDefinition->functionID.first + itos(functionDefinition->functionID.second));
	}

	//对compound进行语义分析(在这一步获取函数返回值的llValue)
	SemanticAnalyseStatement(reinterpret_cast<_Statement *>(functionDefinition->compound));

	// _SymbolRecord* funcRec = mainSymbolTable->recordList[loc];
	// llvm::Value* value = functionDefinition->codeGen(funcRec);
	//❓需要区分：函数返回值的LLVM Value 和 函数本身的LLVM Value，上面的codeGen得到的是后者
	//如果不会用到函数本身的LLVM Value，则函数的codeGen可以不设返回值

	layer--; //层数--
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
	if (lib.count(PID.first)) //判断id是否为库函数
	{
		addDuplicateDefinitionErrorInformation(PID.first, -1, "", "", PID.second);
		return;
	}
	// else if (mainSymbolTable->idToLoc.count(PID.first)) //判断id是否已经使用过
	// {
	// 	int IDloc = mainSymbolTable->idToLoc[PID.first].top();
	// 	semanticWarningInformation.push_back("[Duplicate defined warning!] <Line " + itos(PID.second) + ">" + "\"" + PID.first + "\"" + " has already been defined as a " + mainSymbolTable->recordList[IDloc]->flag + " at line " + itos(mainSymbolTable->recordList[IDloc]->lineNumber) + ".");
	// 	return;
	// }
	else if(mainSymbolTable->idToLoc[PID.first].size() > 0) //此处默认所有函数、过程不得同名
	{
		int loc = mainSymbolTable->idToLoc[PID.first].top();
		string Tname = mainSymbolTable->recordList[loc]->flag;
		if (Tname == "program" || Tname == "procedure" || Tname == "function")
		{
			semanticWarningInformation.push_back("[Invalid formal parameter name!] <Line " + itos(PID.second) + ">" + "The formal parameter \"" + PID.first + "\"" + " has the same name as the " + Tname  + " \"" + mainSymbolTable->recordList[loc]->id + "\" at line " + itos(mainSymbolTable->recordList[loc]->lineNumber) + ".");
			return;
		}
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

	int loc = mainSymbolTable->recordList.size() - 1;
	// llvm::Value* value = formalParameter->codeGen();
	// mainSymbolTable->recordList[loc]->llValue = value;
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
		_RepeatStatement *repeatStatement = reinterpret_cast<_RepeatStatement *>(statement);
		string type = SemanticAnalyseExpression(repeatStatement->condition);
		if (type != "boolean")
		{ // repeat语句类型检查,condition表达式类型检查 checked
			addExpressionTypeErrorInformation(repeatStatement->condition, type, "boolean", "condition of repeat-until statement");
			repeatStatement->statementType = "error";
		}
		else
			repeatStatement->statementType = "void";
		for (int i = 0; i < repeatStatement->_do.size(); ++i)
			SemanticAnalyseStatement(repeatStatement->_do[i]); //对循环体语句进行语义分析
	}
	else if (statement->type == "while")
	{
		_WhileStatement *whileStatement = reinterpret_cast<_WhileStatement *>(statement);
		string type = SemanticAnalyseExpression(whileStatement->condition);
		if (type != "boolean")
		{ // while语句类型检查,condition表达式类型检查 checked
			addExpressionTypeErrorInformation(whileStatement->condition, type, "boolean", "condition of while statement");
			whileStatement->statementType = "error";
		}
		else
			whileStatement->statementType = "void";
		SemanticAnalyseStatement(whileStatement->_do); //对循环体语句进行语义分析
	}
	else if (statement->type == "for")
	{
		_ForStatement *forStatement = reinterpret_cast<_ForStatement *>(statement);
		//检查循环变量是否已经定义，如已经定义，是否为integer型变量
		_SymbolRecord *record = findSymbolRecord(currentSymbolTable, forStatement->id.first);
		if (record == NULL)
		{ //循环变量未定义，错误信息 checked
			addUndefinedErrorInformation(forStatement->id.first, forStatement->id.second);
			return;
		}
		//如果无法作为循环变量 checked
		if (!(record->flag == "value parameter" || record->flag == "var parameter" || record->flag == "normal variant"))
		{ //如果当前符号种类不可能作为循环变量
			addPreFlagErrorInformation(forStatement->id.first, forStatement->id.second, "value parameter, var parameter or normal variant", record->lineNumber, record->flag);
			return;
		}
		//如果类型不是整型 checked
		if (record->type != "integer")
		{
			addUsageTypeErrorInformation(forStatement->id.first, forStatement->id.second, record->type, "cyclic variable of for statement", "integer");
			return;
		}
		// for语句类型检查,start和end表达式类型检查
		forStatement->statementType = "void";
		string type = SemanticAnalyseExpression(forStatement->start);
		if (type != "integer")
		{ // checked
			addExpressionTypeErrorInformation(forStatement->start, type, "integer", "start value of for statement");
			forStatement->statementType = "error";
		}
		type = SemanticAnalyseExpression(forStatement->end);
		if (type != "integer")
		{ // checked
			addExpressionTypeErrorInformation(forStatement->end, type, "integer", "end value of for statement");
			forStatement->statementType = "error";
		}
		//对循环体语句进行语义分析
		SemanticAnalyseStatement(forStatement->_do);
	}
	else if (statement->type == "if")
	{
		_IfStatement *ifStatement = reinterpret_cast<_IfStatement *>(statement);
		string type = SemanticAnalyseExpression(ifStatement->condition);
		if (type != "boolean")
		{ // if语句类型检查,condition表达式类型检查 checked
			addExpressionTypeErrorInformation(ifStatement->condition, type, "boolean", "condition of if statement");
			ifStatement->statementType = "error";
		}
		else
			ifStatement->statementType = "void";
		SemanticAnalyseStatement(ifStatement->then); //对then语句进行语义分析
		if (ifStatement->els != NULL)				 //对else语句进行语句分析
			SemanticAnalyseStatement(ifStatement->els);
	}
	else if (statement->type == "assign")
	{ //左值特判
		_AssignStatement *assignStatement = reinterpret_cast<_AssignStatement *>(statement);
		//对左值变量引用进行语义分析,获得leftType
		assignStatement->statementType = "void";
		assignStatement->variantReference->locFlag = -1; //标记为左值
		string leftType = SemanticAnalyseVariantReference(assignStatement->variantReference);
		if (assignStatement->variantReference->kind == "constant")
		{
			//左值不能为常量 checked
			addGeneralErrorInformation("[Constant as l-value error!] <Line" + itos(assignStatement->variantReference->variantId.second) + "> Costant \"" + assignStatement->variantReference->variantId.first + "\" can't be referenced as l-value.");
			return;
		}
		//对右值表达式进行类型检查,获得rightType
		string rightType = SemanticAnalyseExpression(assignStatement->expression);
		if (assignStatement->variantReference->kind == "function return reference")
		{ //为函数返回值赋值
			//需检查返回值表达式是否和函数返回值类型一致
			if (assignStatement->variantReference->variantType != rightType && !(assignStatement->variantReference->variantType == "real" && rightType == "integer"))
			{
				// checked
				addGeneralErrorInformation("[Return type of funciton mismatch!] <Line " + itos(assignStatement->expression->lineNo) + "> The type of return expression is " + rightType + " ,but not " + assignStatement->variantReference->variantType + " as function \"" + assignStatement->variantReference->variantId.first + "\" defined.");
				assignStatement->statementType = "error";
			}
			assignStatement->isReturnStatement = true;
			return;
		}
		//左值和右值类型不同时
		if (leftType != rightType)
		{
			//只支持整型到实型的隐式转换
			if (leftType == "real" && rightType == "integer")
			{
				semanticWarningInformation.push_back("[Implicit type conversion waring!] <Line" + itos(assignStatement->lineNo) + "> Assign a integer varible to a real variable.\n");
			}
			else
			{
				addAssignTypeMismatchErrorInformation(assignStatement->variantReference, assignStatement->expression);
				assignStatement->statementType = "error";
			}
		}
	}
	else if (statement->type == "procedure") //过程调用
	{
		_ProcedureCall *procedureCall = reinterpret_cast<_ProcedureCall *>(statement);
		//通过procedureId查表，获得参数个数、参数类型等信息
		_SymbolRecord *record = findSymbolRecord(mainSymbolTable, procedureCall->procedureId.first);
		procedureCall->statementType = "void";

		if (record == NULL)
		{ //未定义 checked
			addUndefinedErrorInformation(procedureCall->procedureId.first, procedureCall->procedureId.second);
			procedureCall->statementType = "error";
			return;
		}
		if (record->flag != "procedure")
		{ //如果不是过程 checked
			addPreFlagErrorInformation(procedureCall->procedureId.first, procedureCall->procedureId.second, "procedure", record->lineNumber, record->flag);
			procedureCall->statementType = "error";
			return;
		}

		if (record->id == "read" || record->id == "write")
		{
			if (procedureCall->actualParaList.size() == 0)
			{ // read、write的参数个数不能为 0 checked
				string tmp = record->id;
				// tmp[0] -= 'a' - 'A';
				addGeneralErrorInformation("[" + tmp + " actual parameter missing!] <Line " + itos(procedureCall->procedureId.second) + "> procedure \"" + record->id + "\" must have at least one actual parameter.");
				procedureCall->statementType = "error";
			}
		}
		if (record->id == "read")
		{ //参数只能是变量或数组元素，不能是常量、表达式等
			for (int i = 0; i < procedureCall->actualParaList.size(); i++)
			{
				string actualType = SemanticAnalyseExpression(procedureCall->actualParaList[i]);
				// checked
				if (!(procedureCall->actualParaList[i]->type == "var" && (procedureCall->actualParaList[i]->variantReference->kind == "var" || procedureCall->actualParaList[i]->variantReference->kind == "array")))
					addactualParameterOfReadErrorInformation(procedureCall->actualParaList[i]->lineNo, record->id, i + 1, procedureCall->actualParaList[i]);
				if (procedureCall->actualParaList[i]->expressionType == "boolean")
					addReadBooleanErrorInformation(procedureCall->actualParaList[i], i + 1);
				if (actualType == "error")
					procedureCall->statementType = "error";
			}
			return;
		}
		if (record->amount == -1) //如果是变参过程 本编译器支持从变参过程只有read和write
		{
			for (int i = 0; i < procedureCall->actualParaList.size(); i++)
			{
				string actualType = SemanticAnalyseExpression(procedureCall->actualParaList[i]);
				if (actualType == "error") //要求支持变参过程的参数类型不能为error
					procedureCall->statementType = "error";
			}
			return;
		}
		if (procedureCall->actualParaList.size() != record->amount)
		{ // checked
			addNumberErrorInformation(procedureCall->procedureId.first, procedureCall->procedureId.second, int(procedureCall->actualParaList.size()), record->amount, "procedure");
			procedureCall->statementType = "error";
			return;
		}
		// 形参在符号表中的定位
		for (int i = 0; i < procedureCall->actualParaList.size(); i++)
		{ //检查实参和形参的类型一致性
			string actualType = SemanticAnalyseExpression(procedureCall->actualParaList[i]);
			string formalType = record->findXthFormalParaType(i);
			bool isRefered = record->isXthFormalParaRefered(i); //是否是引用调用
			if (isRefered && !(procedureCall->actualParaList[i]->type == "var" && (procedureCall->actualParaList[i]->variantReference->kind == "var" || procedureCall->actualParaList[i]->variantReference->kind == "array")))
			{
				//引用参数对应的实参只能是变量、参数或者数组元素 不能为常数、复杂表达式等 checked
				addGeneralErrorInformation("[Referenced actual parameter error!] <Line " + itos(procedureCall->actualParaList[i]->lineNo) + "> The " + itos(i + 1) + "th actual parameter expression should be a normal variable、value parameter、referenced parameter or array element.");
				continue;
			}
			if (!isRefered)
			{
				if (actualType != formalType) //如果类型不一致
				{
					//传值参数支持integer到real的隐式类型转换
					if (actualType == "integer" && formalType == "real")
					{
						semanticWarningInformation.push_back("[Implicit type conversion waring!] <Line" + itos(procedureCall->lineNo) + "> The " + itos(i + 1) + "th actual parameter of procedure call is integer while the corresponding formal parameter is real.\n");
					}
					else
					{
						addExpressionTypeErrorInformation(procedureCall->actualParaList[i], actualType, formalType, itos(i + 1) + "th actual parameter of procedure call of \"" + procedureCall->procedureId.first + "\"");
						procedureCall->statementType = "error";
					}
				}
			}
			else
			{ //引用参数需保持类型强一致
				if (actualType != formalType)
				{ //如果类型不一致
					// checked
					addExpressionTypeErrorInformation(procedureCall->actualParaList[i], actualType, formalType, itos(i + 1) + "th actual parameter of procedure call of \"" + procedureCall->procedureId.first + "\"");
					procedureCall->statementType = "error";
				}
			}
		}
	}
	else
	{
		cout << "[SemanticAnalyseStatement] statement type error" << endl;
		return;
	}
}

//对函数调用进行语义分析
string SemanticAnalyseFunctionCall(_FunctionCall *functionCall)
{
	if (functionCall == NULL)
	{
		cout << "[SemanticAnalyseFunctionCall] pointer of _FunctionCall is null" << endl;
		return "error";
	}

	std::pair<string, int> FCID = functionCall->functionId;
	if (mainSymbolTable->idToLoc.count(FCID.first) == 0) //找不到函数声明
	{
		addUndefinedErrorInformation(FCID.first, FCID.second);
		return "error";
	}

	int decID = mainSymbolTable->idToLoc[FCID.first].top();
	functionCall->returnType = mainSymbolTable->recordList[decID]->type;

	int decParaSize = mainSymbolTable->recordList[decID]->amount;
	if (decParaSize != functionCall->actualParaList.size()) //参数表数目不对
	{
		addNumberErrorInformation(FCID.first, FCID.second, functionCall->actualParaList.size(), decParaSize, "function");
	}

	int ParaSize = functionCall->actualParaList.size();
	for (int i = 0; i < ParaSize; ++i) //判断每个参数的类型
	{
		SemanticAnalyseExpression(functionCall->actualParaList[i]);
		string decType = mainSymbolTable->recordList[decID + i + 1]->type;
		string expType = functionCall->actualParaList[i]->expressionType;
		if (expType != decType && !(expType == "integer" && decType == "real"))
		{
			addUsageTypeErrorInformation("arg no." + itos(i + 1), FCID.second, expType, FCID.first, decType);
		}
	}
	return functionCall->returnType;
	//❓codeGen
	// llvm::Value* value = functionCall->codeGen();
}

//对表达式进行语义分析
string SemanticAnalyseExpression(_Expression *expression)
{
	if (expression == NULL)
	{
		cout << "[SemanticAnalyseExpression] pointer of _Expression is null" << endl;
		return "error";
	}

	//表达式类型为变量 <ok>
	if (expression->type == "var")
	{
		string variantReferenceType = SemanticAnalyseVariantReference(expression->variantReference);
		// int类型的常量则记录值
		if (variantReferenceType == "integer" && expression->variantReference->kind == "constant")
		{
			_SymbolRecord *record = findSymbolRecord(currentSymbolTable, expression->variantReference->variantId.first);
			if (record == NULL)
			{
				cout << "[SemanticAnalyseExpression] pointer of record is null" << endl;
				return "error";
			}
			if (record->flag != "constant")
			{
				cout << " : [SemanticAnalyseExpression] the record should be a constant" << endl;
				return "error";
			}
			expression->totalIntValue = str2int(record->value);
			if (record->isMinusShow)
				expression->totalIntValue = -expression->totalIntValue;
			expression->totalIntValueValid = true;
		}
		return expression->expressionType = variantReferenceType;
	}

	//表达式类型为integer <ok>
	else if (expression->type == "integer")
	{
		expression->totalIntValue = expression->intNum;
		expression->totalIntValueValid = true;
		return expression->expressionType = "integer";
	}

	//表达式类型为real <ok>
	else if (expression->type == "real")
		return expression->expressionType = "real";

	//表达式类型为char <ok>
	else if (expression->type == "char")
		return expression->expressionType = "char";

	//表达式类型为布尔类型boolean
	else if (expression->type == "boolean")
		return expression->expressionType = "boolean";

	//表达式类型为函数调用 <ok>
	else if (expression->type == "function") //获得函数调用的返回值类型
		return expression->expressionType = SemanticAnalyseFunctionCall(expression->functionCall);

	//含有运算符的表达式
	else if (expression->type == "compound")
	{

		//表达式中含有关系运算符relop ||注意：此处的relop包含了词法分析的（relop（>=|>|<=|<|<>）和=），同词法定义不一致
		if (expression->operationType == "relop")
		{
			string epType1 = SemanticAnalyseExpression(expression->operand1);
			string epType2 = SemanticAnalyseExpression(expression->operand2);
			//类型兼容
			if ((epType1 == epType2 && epType1 != "error") || (epType1 == "integer" && epType2 == "real") || (epType1 == "real" && epType2 == "integer"))
				return expression->expressionType = "boolean";
			//类型错误或类型不兼容
			else
			{
				if (epType1 != epType2 && epType1 != "error" && epType2 != "error")
					addOperandExpressionsTypeMismatchErrorInformation(expression->operand1, expression->operand2);
				return expression->expressionType = "error";
			}
		}

		//单目运算符not
		else if (expression->operation == "not")
		{
			string type = SemanticAnalyseExpression(expression->operand1);
			//类型兼容
			if (type == "boolean")
				return expression->expressionType = "boolean";
			//类型错误或类型不兼容
			else
			{
				if (type != "error" && type != "boolean")
					addSingleOperandExpressionTypeMismatchErrorInformation(expression->operand1, "boolean");
				return expression->expressionType = "error";
			}
		}

		//负号minus
		else if (expression->operation == "minus")
		{
			string epType = SemanticAnalyseExpression(expression->operand1);
			if (epType == "integer" && expression->operand1->totalIntValueValid)
			{
				expression->totalIntValue = -expression->operand1->totalIntValue;
				expression->totalIntValueValid = true;
			}
			if (epType == "integer" || epType == "real")
				return expression->expressionType = epType;
			else
			{
				if (epType != "error" && epType != "integer" && epType != "real")
					addSingleOperandExpressionTypeMismatchErrorInformation(expression->operand1, "integer or real");
				return expression->expressionType = "error";
			}
		}

		//括号表达式( expression )
		else if (expression->operation == "bracket")
		{
			expression->expressionType = SemanticAnalyseExpression(expression->operand1);
			//如果为整型则记录结果
			if (expression->expressionType == "integer" && expression->operand1->totalIntValueValid)
			{
				expression->totalIntValue = expression->operand1->totalIntValue;
				expression->totalIntValueValid = true;
			}
			return expression->expressionType;
		}

		//算术运算符+ - * /
		else if (expression->operation == "+" || expression->operation == "-" || expression->operation == "*" || expression->operation == "/")
		{
			string epType1 = SemanticAnalyseExpression(expression->operand1);
			string epType2 = SemanticAnalyseExpression(expression->operand2);
			//除数为0
			if (expression->operation == "/" && epType2 == "integer" && expression->operand2->totalIntValueValid && expression->operand2->totalIntValue == 0)
				addDivideZeroErrorInformation(expression->operation, expression->operand2);
			//整数运算记录结果
			if (epType1 == "integer" && epType2 == "integer" && expression->operand1->totalIntValueValid && expression->operand2->totalIntValueValid)
			{
				expression->totalIntValueValid = true;
				if (expression->operation == "+")
					expression->totalIntValue = expression->operand1->totalIntValue + expression->operand2->totalIntValue;
				else if (expression->operation == "-")
					expression->totalIntValue = expression->operand1->totalIntValue - expression->operand2->totalIntValue;
				else if (expression->operation == "*")
					expression->totalIntValue = expression->operand1->totalIntValue * expression->operand2->totalIntValue;
				else
					expression->totalIntValue = expression->operand1->totalIntValue / expression->operand2->totalIntValue;
			}
			//类型处理
			if ((epType1 == "integer" || epType1 == "real") && (epType2 == "integer" || epType2 == "real"))
			{
				if (epType1 == "integer" && epType2 == "integer")
					return expression->expressionType = "integer";
				return expression->expressionType = "real";
			}
			if (epType1 != "error" && epType1 != "integer" && epType1 != "real")
				addSingleOperandExpressionTypeMismatchErrorInformation(expression->operand1, "integer or real");
			if (epType2 != "error" && epType2 != "integer" && epType2 != "real")
				addSingleOperandExpressionTypeMismatchErrorInformation(expression->operand2, "integer or real");
			return expression->expressionType = "error";
		}

		//算术运算符 div mod
		else if (expression->operation == "div" || expression->operation == "mod")
		{
			string epType1 = SemanticAnalyseExpression(expression->operand1);
			string epType2 = SemanticAnalyseExpression(expression->operand2);
			//除数为0
			if (epType2 == "integer" && expression->operand2->totalIntValueValid && expression->operand2->totalIntValue == 0)
				addDivideZeroErrorInformation(expression->operation, expression->operand2);
			//整数运算记录结果
			if (epType1 == "integer" && epType2 == "integer")
			{
				if (expression->operand1->totalIntValueValid && expression->operand2->totalIntValueValid)
				{
					if (expression->operation == "div")
						expression->totalIntValue = expression->operand1->totalIntValue / expression->operand2->totalIntValue;
					else
						expression->totalIntValue = expression->operand1->totalIntValue % expression->operand2->totalIntValue;
					expression->totalIntValueValid = true;
				}
				return expression->expressionType = "integer";
			}
			if (epType1 != "error" && epType1 != "integer")
				addSingleOperandExpressionTypeMismatchErrorInformation(expression->operand1, "integer");
			if (epType2 != "error" && epType2 != "integer")
				addSingleOperandExpressionTypeMismatchErrorInformation(expression->operand2, "integer");
			return expression->expressionType = "error";
		}

		//逻辑运算符 and or
		else if (expression->operation == "and" || expression->operation == "or")
		{
			string epType1 = SemanticAnalyseExpression(expression->operand1);
			string epType2 = SemanticAnalyseExpression(expression->operand2);
			if (epType1 == "boolean" && epType2 == "boolean")
				return expression->expressionType = "boolean";
			if (epType1 != "error" && epType1 != "boolean")
				addSingleOperandExpressionTypeMismatchErrorInformation(expression->operand1, "boolean");
			if (epType2 != "error" && epType2 != "boolean")
				addSingleOperandExpressionTypeMismatchErrorInformation(expression->operand2, "boolean");
			return expression->expressionType = "error";
		}

		//出现未知运算符
		else
		{
			cout << "[_Expression::SemanticAnalyseExpression] ERROR: operation not found" << endl;
			return "error";
		}
	}

	else
	{
		cout << "[_Expression::SemanticAnalyseExpression] ERROR: expression type not found" << endl;
		return "error";
	}
}

//对变量引用进行语义分析
//可能是传值参数、引用参数、普通变量、数组元素、结构体.属性、函数名
//只有当前函数名可以作为左值
string SemanticAnalyseVariantReference(_VariantReference *variantReference)
{
	
	if (variantReference == NULL)
	{
		cout << "[SemanticAnalyseVariantReference] pointer of _VariantReference is null" << endl;
		return "";
	}

	_SymbolRecord *record = findSymbolRecord(mainSymbolTable, variantReference->variantId.first);

	//未定义
	if (record == NULL)
	{
		addUndefinedErrorInformation(variantReference->variantId.first, variantReference->variantId.second);
		return variantReference->variantType = "error";
	}

	//非数组元素、非结构体.属性（传值参数、引用参数、普通变量、常量、函数名）<ok>
	if (variantReference->IdvpartList.size() == 0)
	{
		//函数名：只有当前函数可以作为左值，必须作为右值，且形参个数必须为0 ||注意：被识别为variantReference的函数调用一定不含实参，所以需要检查形参个数
		if (record->flag == "function")
		{
			variantReference->kind = "function";
			//如果是左值
			if (variantReference->locFlag == -1)
			{
				string curFunction = mainSymbolTable->recordList[mainSymbolTable->indexTable[mainSymbolTable->indexTable.size() - 1]]->id;
				if (curFunction == variantReference->variantId.first)
				{
					variantReference->kind = "function return reference";
					return variantReference->variantType = record->type;
				}
				addGeneralErrorInformation("[Invalid reference!] <Line " + itos(variantReference->variantId.second) + "> function name \"" + record->id + "\" can't be referenced as l-value.");
				return variantReference->variantType = "error";
			}
			//如果是右值
			if (variantReference->locFlag == 1 && record->amount != 0)
			{
				addNumberErrorInformation(variantReference->variantId.first, variantReference->variantId.second, 0, record->amount, "function");
				return variantReference->variantType = record->type;
			}
			return variantReference->variantType = record->type;
		}

		//其余只能是传值参数、传引用参数、普通变量、常量
		if (!(record->flag == "value parameter" || record->flag == "var parameter" || record->flag == "normal variant" || record->flag == "constant"))
		{
			addGeneralErrorInformation("[Invalid reference!] <Line " + itos(variantReference->variantId.second) + "> \"" + variantReference->variantId.first + "\" is a " + record->flag + ", it can't be referenced.");
			return variantReference->variantType = "error";
		}
		
		//记录类型
		variantReference->kind = "var";
		if (record->flag == "constant")
			variantReference->kind = "constant";

		//类型等价检查，将类型别名转为基本类型	
		if(record->flag=="normal variant"){
			//获取类型别名记录
			_SymbolRecord *recordSource = findSymbolRecord(mainSymbolTable, record->type);
			//若存在别名记录则进行替换
			if(recordSource!=NULL){
				return variantReference->variantType = recordSource->type;
			}
		}
		return variantReference->variantType = record->type;
	}

	//数组元素
	else if(record->flag =="array"){
		variantReference->kind = "array";

			//引用时的下标维数和符号表所存不一致
			if (variantReference->IdvpartList.size() != record->amount)
			{
				addNumberErrorInformation(variantReference->variantId.first, variantReference->variantId.second, int(variantReference->IdvpartList.size()), record->amount, "array");
				variantReference->variantType = "error";
				return variantReference->variantType = "error";
			}

			variantReference->variantType = record->type;
			
			for (int i = 0; i < variantReference->IdvpartList.size(); i++)
			{

				for (int j = 0; j < variantReference->IdvpartList[i]->expressionList.size(); j++)
				{
					string type = SemanticAnalyseExpression(variantReference->IdvpartList[i]->expressionList[j]);
					//检查每一维下标表达式的类型是否是整型
					if (type != "integer")
					{
						addExpressionTypeErrorInformation(variantReference->IdvpartList[i]->expressionList[j], type, "integer", itos(i + 1) + "th index of array \"" + variantReference->variantId.first + "\"");
						variantReference->variantType = "error";
						return variantReference->variantType = "error";
					}
					//检查越界
					if (variantReference->IdvpartList[i]->expressionList[j]->totalIntValueValid)
					{
						if (!record->checkArrayXthIndexRange(i, variantReference->IdvpartList[i]->expressionList[j]->totalIntValue))
						{
							addArrayRangeOutOfBoundErrorInformation(variantReference->IdvpartList[i]->expressionList[j], variantReference->variantId.first, i, record->arrayRangeList[i]);
							return variantReference->variantType = "error";
						}
					}
				}
			}
			return record->type;
	}

	//结构体属性
	else if(record->flag =="records")
	{
		//检查属性是否与结构体匹配
		for (int i = 0; i < record->records.size(); i++)
		{
			if (record->records[i]->id == variantReference->IdvpartList[0]->IdvpartId.first)
			{				
				return variantReference->variantType = record->records[i]->type;
			}
		}

		//被引用的属性不存在
		addVariantReferenceErrorInformation(variantReference->variantId.second,"record member reference is illegal");
		return variantReference->variantType = "error";
	}

	//结构体.属性、数组元素作类型检查与替换(即处理结构中含数组、数组中含结构体的嵌套)
	else if (record->flag == "normal variant")
	{	
		string curType=record->type;
		
		for(int n=0;n<variantReference->IdvpartList.size();n++){

			if(curType=="integer"||curType=="real"||curType=="char"||curType=="boolean"){
				addVariantReferenceErrorInformation(variantReference->variantId.second,"illegal array element access or illegal record attribute access");
				return variantReference->variantType="error";
			}
			//获取类型定义记录
			_SymbolRecord *recordSource=mainSymbolTable->recordList[mainSymbolTable->idToLoc[curType].top()];
			
			if(recordSource==NULL)
			{
				string info= variantReference->variantId.first;
				info +=": type ";
				info+=record->type;
				info+=" is not defined;";
				addVariantReferenceErrorInformation(variantReference->variantId.second,info);
				return  variantReference->variantType ="error";
			}

			//构造变量引用
			 _VariantReference *curVariantReference = new _VariantReference();
			curVariantReference->variantId=make_pair(curType, variantReference->variantId.second);
		    curVariantReference->locFlag=0;

			//属性字段
			if(recordSource->flag=="records"){
				curVariantReference->IdvpartList.push_back(variantReference->IdvpartList[n]);
			}
			//数组元素字段
			else if(recordSource->flag=="array")
			{
				for(int s=0;s<recordSource->amount&&n+s<variantReference->IdvpartList.size();s++)
					curVariantReference->IdvpartList.push_back(variantReference->IdvpartList[n+s]);
				n+=recordSource->amount-1;
			}
			else{
				cout<<"flag of " <<variantReference->variantId.first<<" is not record or array"<<endl;
			}

			//获取当前类型
			curType=SemanticAnalyseVariantReference(curVariantReference);
			
			//错误信息已经记录在了递归子程序中，此处不重复记录
			if(curType=="error")
				return variantReference->variantType="error";
		}
		return variantReference->variantType=curType;

	}

	else
	{
		cout << "[SemanticAnalyseVariantReference] flag of variantReference symbolRecord is illegal: " <<endl;
		return variantReference->variantType = "error";
	}
}

//符号表重定位
void relocation()
{
	// cout << "\nBefore Poping:\n";
	// mainSymbolTable->putTable();
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
	// mainSymbolTable->putTable();
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

void addExpressionTypeErrorInformation(_Expression *exp, string curType, string correctType, string description)
{
	string errorInformation = "[Expression type error!] <Line " + itos(exp->lineNo) + "> ";
	string expression;
	// inputExpression(exp, expression, 1);  获取表达式的具体内容 是否要用到代码生成部分？
	errorInformation += "Expression \"" + expression + "\" used for " + description + " should be " + correctType + " but not " + curType + ".";
	semanticErrorInformation.push_back(errorInformation);
	// CHECK_ERROR_BOUND
}

void addUndefinedErrorInformation(string id, int curLineNumber)
{
	string errorInformation = "[Undefined identifier!] <Line " + itos(curLineNumber) + "> ";
	errorInformation += "\"" + id + "\" has not been defined.";
	semanticErrorInformation.push_back(errorInformation);
	// CHECK_ERROR_BOUND
}

void addPreFlagErrorInformation(string curId, int curLineNumber, string curFlag, int preLineNumber, string preFlag)
{
	string errorInformation = "[Symbol kinds mismatch!] <Line " + itos(curLineNumber) + "> ";
	errorInformation += "\"" + curId + "\"" + " defined at line " + itos(preLineNumber) + " is a " + preFlag + " but not a " + curFlag + ".";
	semanticErrorInformation.push_back(errorInformation);
	// CHECK_ERROR_BOUND
}

void addUsageTypeErrorInformation(string curId, int curLineNumber, string curType, string usage, string correctType)
{
	string errorInformation = "[Usage type error!] <Line " + itos(curLineNumber) + "> ";
	errorInformation += "\"" + curId + "\"" + " used for " + usage + " should be " + correctType + " but not " + curType + ".";
	semanticErrorInformation.push_back(errorInformation);
	// CHECK_ERROR_BOUND
}

void addGeneralErrorInformation(string errorInformation)
{
	semanticErrorInformation.push_back(errorInformation);
	// CHECK_ERROR_BOUND
}

void addAssignTypeMismatchErrorInformation(_VariantReference *leftVariantReference, _Expression *rightExpression)
{
	string errorInformation;
	errorInformation += "[Assign statement type mismatch!] ";
	errorInformation += "<Left at line " + itos(leftVariantReference->variantId.second) + ", right at line " + itos(rightExpression->lineNo) + "> ";
	string varRef, exp;
	// inputVariantRef(leftVariantReference, varRef, 1);
	// inputExpression(rightExpression, exp, 1);
	errorInformation += "Left \"" + varRef + "\" type is " + leftVariantReference->variantType + " while right \"" + exp + "\" type is " + rightExpression->expressionType + ".";
	semanticErrorInformation.push_back(errorInformation);
	// CHECK_ERROR_BOUND
}

void addactualParameterOfReadErrorInformation(int curLineNumber, string procedureId, int X, _Expression *exp)
{
	string errorInformation = "[Actual parameter of read procedure type error!] ";
	errorInformation += "<Line " + itos(curLineNumber) + "> ";
	string expression;
	// inputExpression(exp, expression, 1);
	errorInformation += "\"" + procedureId + "\" " + itos(X) + "th expression parameter \"" + expression + "\" is not a variant or an array element.";
	semanticErrorInformation.push_back(errorInformation);
	// CHECK_ERROR_BOUND
}

void addReadBooleanErrorInformation(_Expression *exp, int X)
{
	string errorInformation = "[Read boolean error!] <Line " + itos(exp->lineNo) + "> ";
	string expression;
	// inputExpression(exp, expression, 1);
	errorInformation += "The " + itos(X) + "th actual parameter of read \"" + expression + "\" is boolean, it can't be read.";
	semanticErrorInformation.push_back(errorInformation);
	// CHECK_ERROR_BOUND
}

//数组下标个数不匹配、函数或过程的实参和形参的个数不匹配
void addNumberErrorInformation(string curId, int curLineNumber, int curNumber, int correctNumber, string usage)
{
	string errorInformation;
	if (usage == "array")
	{
		errorInformation += "[Array index number mismatch!] ";
		errorInformation += "<Line " + itos(curLineNumber) + "> ";
		errorInformation += "Array \"" + curId + "\"" + " should have " + itos(correctNumber) + " but not " + itos(curNumber) + " indices.";
	}
	else if (usage == "procedure")
	{
		errorInformation += "[Procedure parameter number mismatch!] ";
		errorInformation += "<Line " + itos(curLineNumber) + "> ";
		errorInformation += "Procedure \"" + curId + "\"" + " should have " + itos(correctNumber) + " but not " + itos(curNumber) + " parameters.";
	}
	else if (usage == "function")
	{
		errorInformation += "[Function parameter number mismatch!] ";
		errorInformation += "<Line " + itos(curLineNumber) + "> ";
		errorInformation += "Function \"" + curId + "\"" + " should have " + itos(correctNumber) + " but not " + itos(curNumber) + " parameters.";
	}
	else
	{
		cout << "[addNumberErrorInformation] usage error" << endl;
		return;
	}
	semanticErrorInformation.push_back(errorInformation);
	// CHECK_ERROR_BOUND
}

//添加数组下标越界错误信息
void addArrayRangeOutOfBoundErrorInformation(_Expression *expression, string arrayId, int X, pair<int, int> range)
{
	string errorInformation;
	errorInformation += "[Array range out of bound!] ";
	errorInformation += "<Line " + itos(expression->lineNo) + "> ";
	string exp;
	// inputExpression(expression, exp, 1);
	errorInformation += "The value of expression \"" + exp + "\"" + " is " + itos(expression->totalIntValue);
	errorInformation += ", but the range of array \"" + arrayId + "\" " + itos(X) + "th index is " + itos(range.first) + " to " + itos(range.second) + ".";
	semanticErrorInformation.push_back(errorInformation);
}

//添加运算符两边的操作数类型不一致的错误信息
void addOperandExpressionsTypeMismatchErrorInformation(_Expression *exp1, _Expression *exp2)
{
	string errorInformation;
	errorInformation += "[Operands expression type mismatch!] ";
	errorInformation += "<Left at line " + itos(exp1->lineNo) + ", right at line " + itos(exp2->lineNo) + "> ";
	string expStr1, expStr2;
	// inputExpression(exp1, expStr1, 1);
	// inputExpression(exp2, expStr2, 1);
	errorInformation += "Left \"" + expStr1 + "\" type is " + exp1->expressionType + " while right " + "\"" + expStr2 + "\" type is " + exp2->expressionType + ".";
	semanticErrorInformation.push_back(errorInformation);
}

//添加除0错误信息
void addDivideZeroErrorInformation(string operation, _Expression *exp)
{
	string errorInformation;
	errorInformation += "[Divide zero error!] ";
	errorInformation += "<Line " + itos(exp->lineNo) + "> ";
	string expression;
	// inputExpression(exp, expression, 1);
	errorInformation += "The value of expression \"" + expression + "\" is 0, which is the second operand of operation \"" + operation + "\".";
	semanticErrorInformation.push_back(errorInformation);
}

//添加某个操作数类型错误的信息
void addSingleOperandExpressionTypeMismatchErrorInformation(_Expression *exp, string correctType)
{
	string errorInformation;
	errorInformation += "[Operand expression type error!] ";
	errorInformation += "<Line " + itos(exp->lineNo) + "> ";
	string expStr;
	// inputExpression(exp, expStr, 1);
	errorInformation += "Expression \"" + expStr + "\" type should be " + correctType + " but not " + exp->expressionType + ".";
	semanticErrorInformation.push_back(errorInformation);
}

//添加变量引用错误信息
void addVariantReferenceErrorInformation(int line,string info){
	string errorInformation="[variable reference error]";
	errorInformation += "<Line " + itos(line) + "> ";
	errorInformation += info;
	semanticErrorInformation.push_back(errorInformation);
}
