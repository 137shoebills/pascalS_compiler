/*
语义分析实现代码
*/

#include "semanticAnalyse.h"
#include "CodeGen.h"

using namespace std;
extern bool codeGen_error;
int layer = 0; //标记程序的嵌套层数
int expflag = 1;

set<string> lib; //存放库函数名
extern _SymbolTable *mainSymbolTable;
extern CodeGenContext context;				
extern _SymbolRecord* findSymbolRecord(string id); //从符号表中找出id对应的记录
extern int str2int(string str);
vector<string> semanticErrorInformation;   //存储错误信息的列表
vector<string> semanticWarningInformation; //存储警告信息的列表

void SemanticAnalyse(_Program *ASTRoot)
{
	createSymbolTableAndInit();
	context.InitCodeGen();
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
	//即主程序名不能和库函数同名，主程序参数不能和库函数名、主程序名同名
	//添加主程序名、行号、参数个数等信息
	if (lib.count(program->programId.first)) //主程序与库函数同名
		addGeneralErrorInformation("[Duplicate defined error!] <Line " + itos(program->programId.second) + "> Name of program \"" + program->programId.first + "\" has been defined as a lib program.");

	mainSymbolTable->addProgram(program->programId.first, program->programId.second, int(program->paraList.size()), "");

	//将主程序的参数添加到主符号表中，flag定为"parameter of program"
	for (int i = 0; i < program->paraList.size(); i++)
	{
		int nameError = isLibName(program->paraList[i].first);
		if(nameError < 0)
			addDuplicateDefinitionErrorInformation(program->paraList[i].first, nameError, "", "", program->paraList[i].second);
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

	context.builder->CreateRet(nullptr);
	
	if(codeGen_error || semanticErrorInformation.size() > 0){	//若语义分析或codeGen出错，不打印ir
		cout<<"\n\ncodeGen_error!\n\n";
		return;
	}

	cout<<"------------------LLVM IR-------------------"<<endl;
	//llvm::PassManager pm;
	llvm::legacy::PassManager pm;
	pm.add(llvm::createPrintModulePass(llvm::outs()));
	pm.run(*(context.Module));
	cout<<"--------------------------------------------"<<endl;
}

//对子program进行语义分析
void SemanticAnalyseSubprogram(_SubProgram *subprogram)
{
	if (subprogram == NULL)
	{ //分程序的指针为空
		cout << "[SemanticAnalyseSubprogram] pointer of _Subprogram is null" << endl;
		return;
	}

	//test
	for (int i = 0; i < subprogram->subprogramDefinitionList.size(); i++)
	{
		SemanticAnalyseSubprogramDefinition(subprogram->subprogramDefinitionList[i]);
		relocation();
	}
	
	for (int i = 0; i < subprogram->constList.size(); i++)
		SemanticAnalyseConst(subprogram->constList[i]);
	for (int i = 0; i < subprogram->typedefList.size(); i++)
		SemanticAnalyseTypedef(subprogram->typedefList[i]);
	
	//创建主函数
	subprogram->codeGen();

	for (int i = 0; i < subprogram->variantList.size(); i++)
		SemanticAnalyseVariant(subprogram->variantList[i]);

	SemanticAnalyseStatement(reinterpret_cast<_Statement *>(subprogram->compound),1);
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
	int nameError = isLibName(CID.first);
	int loc;
	if(nameError < 0)
		addDuplicateDefinitionErrorInformation(CID.first, nameError, "", "", CID.second);
	else if (isReDef(CID.first,loc))
		addDuplicateDefinitionErrorInformation(CID.first, mainSymbolTable->recordList[loc]->lineNumber, mainSymbolTable->recordList[loc]->flag, mainSymbolTable->recordList[loc]->type, CID.second);

	if (constant->type == "integer") //若常量整数值超出int范围，则将其化为float
	{
		long long maxint = 32767;
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
			semanticWarningInformation.push_back("[Implicit type conversion warning!] <Line" + itos(CID.second) + "> The integer constant " + CID.first + "is out of range.Automatically converted to real!\n");
		}
	}
	mainSymbolTable->addConst(CID.first, CID.second, constant->type, constant->isMinusShow, constant->strOfVal);
	
	//codeGen
	llvm::Value* value = constant->codeGen();	//返回常量对应的LLVM Value*
	mainSymbolTable->recordList[mainSymbolTable->recordList.size()-1]->llValue = value;
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
	int nameError = isLibName(TID.first);
	int loc;
	if(nameError < 0)
		addDuplicateDefinitionErrorInformation(TID.first, nameError, "", "", TID.second);
	else if (isReDef(TID.first,loc))
		addDuplicateDefinitionErrorInformation(TID.first, mainSymbolTable->recordList[loc]->lineNumber, mainSymbolTable->recordList[loc]->flag, mainSymbolTable->recordList[loc]->type, TID.second);

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

	typedefi->codeGen();	//创建数组/record的LLVM Type
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
		int nameError = isLibName(aVID.first);
		int loc;
		if(nameError < 0)
			addDuplicateDefinitionErrorInformation(aVID.first, nameError, "", "", aVID.second);
		else if (isReDef(aVID.first,loc))
			addDuplicateDefinitionErrorInformation(aVID.first, mainSymbolTable->recordList[loc]->lineNumber, mainSymbolTable->recordList[loc]->flag, mainSymbolTable->recordList[loc]->type, aVID.second);

		if (recordList[i]->type->type.first == "record")
		{
			tmpRecord->setRecords(aVID.first + "_", aVID.second, SemanticAnalyseRecord(recordList[i]->type->recordList, aVID, 2));
			tmpRecord->setVar(aVID.first, aVID.second, aVID.first + "_");
		}
		else if (recordList[i]->type->flag)
		{
			tmpRecord->setArray(aVID.first + "_", aVID.second, recordList[i]->type->type.first, recordList[i]->type->arrayRangeList.size(), recordList[i]->type->arrayRangeList);
			records.push_back(tmpRecord);
			tmpRecord->setVar(aVID.first, aVID.second, aVID.first + "_");
		}
		else
			tmpRecord->setVar(aVID.first, aVID.second, recordList[i]->type->type.first);

		records.push_back(tmpRecord);
		ids[aVID.first] = aVID.second;
	}

	if (type == 0) //表示此时不是type中定义新类型，而是声明语句
	{
		//cout<<"define new record type in var:"<<VID.first+"_"<<"\n\n";
		mainSymbolTable->addRecords(VID.first + "_", VID.second, records); // id名后加_下划线表示record类型名
		mainSymbolTable->addVar(VID.first, VID.second, VID.first + "_");
	}
	else if (type == 2) //表示此时是嵌套record
	{
		return records;
	}
	else if (type == 1) //表示此时是自定义类型
	{
		mainSymbolTable->addRecords(VID.first, VID.second, records);
	}
	return records;
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
	int nameError = isLibName(VID.first);
	int loc;
	if(nameError < 0)
		addDuplicateDefinitionErrorInformation(VID.first, nameError, "", "", VID.second);
	else if (isReDef(VID.first,loc))
		addDuplicateDefinitionErrorInformation(VID.first, mainSymbolTable->recordList[loc]->lineNumber, mainSymbolTable->recordList[loc]->flag, mainSymbolTable->recordList[loc]->type, VID.second);

	string type = variant->type->type.first;

	if (type == "record") //判断是否为record
	{
		SemanticAnalyseRecord(variant->type->recordList, VID, 0);
	}
	else if (variant->type->flag) //判断是否为数组
	{
		mainSymbolTable->addArray(VID.first+"_", VID.second, variant->type->type.first, variant->type->arrayRangeList.size(), variant->type->arrayRangeList);
		mainSymbolTable->addVar(VID.first, VID.second,VID.first+"_");
	}
	 //非标准类型时进行类型检查
	//else if (type != "integer" && type != "char" && type != "real" && type != "boolean")
	else if(!context.typeSystem.isBasicType(type))
	{
		_SymbolRecord *recordSource = findSymbolRecord(type);	
		if(recordSource==NULL){
			addUndefinedErrorInformation(type,variant->variantId.second);
		}
		mainSymbolTable->addVar(VID.first, VID.second, type);
	}
	else //普通变量
	{
		mainSymbolTable->addVar(VID.first, VID.second, type);
	}

	//codeGen
	llvm::Value* value = variant->codeGen();	//返回局部变量地址（CreateAlloca的返回值）

	loc = mainSymbolTable->idToLoc[VID.first].top();
	mainSymbolTable->recordList[loc]->llValue = value;
}

//对子程序定义进行语义分析
void SemanticAnalyseSubprogramDefinition(_FunctionDefinition *functionDefinition)
{
	layer++; //层数++
	//test
	//cout<<"layer="<<layer<<", funcName="<<functionDefinition->functionID.first<<"\n\n";

	if (functionDefinition == NULL)
	{
		cout << "[SemanticAnalyseSubprogramDefinition] pointer of _FunctionDefinition is null" << endl;
		return;
	}

	_SymbolRecord* record = findSymbolRecord(functionDefinition->functionID.first);
	if (record != NULL) //重定义检查 所有function、procedure不得同名 不得与任何标识符同名
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

	//为什么在这里判断原因：因为返回后要做重定位操作，需要把此时的函数先在符号表里存下来，否则返回后重定位会超前做上一层的
	if(layer > 5)
	{
		addGeneralErrorInformation("[too much layers!] in \"" + functionDefinition->functionID.first + "\" at line " + itos(functionDefinition->functionID.second));
		layer--;
		return;		
	}

	//test
	if(functionDefinition->subprogramDefinitionList.size() > 0 && layer <= 5){
		for (int i = 0; i < functionDefinition->subprogramDefinitionList.size(); i++)
		{
			SemanticAnalyseSubprogramDefinition(functionDefinition->subprogramDefinitionList[i]);	
			relocation();		
		}
	
	}

	//对形式参数列表进行语义分析，并将形式参数添加到子符号表中
	for (int i = 0; i < functionDefinition->formalParaList.size(); i++)
		SemanticAnalyseFormalParameter(functionDefinition->formalParaList[i]);

	//对常量定义进行语义分析
	for (int i = 0; i < functionDefinition->constList.size(); i++)
		SemanticAnalyseConst(functionDefinition->constList[i]);
	//对自定义类型进行语义分析
	for (int i = 0; i < functionDefinition->typedefList.size(); i++)
		SemanticAnalyseTypedef(functionDefinition->typedefList[i]);
	
	//test
	//先获得函数指针
	record = findSymbolRecord(functionDefinition->functionID.first);
	record->functionPtr = functionDefinition->codeGen();	//返回函数指针（Function::Create的返回值）

	//对变量定义进行语义分析
	for (int i = 0; i < functionDefinition->variantList.size(); i++)
		SemanticAnalyseVariant(functionDefinition->variantList[i]);

	// if(functionDefinition->subprogramDefinitionList.size() > 0 && layer <= 5){
	// 	for (int i = 0; i < functionDefinition->subprogramDefinitionList.size(); i++)
	// 	{
	// 		SemanticAnalyseSubprogramDefinition(functionDefinition->subprogramDefinitionList[i]);	
	// 		relocation();		
	// 	}
	
	// }
	
	// //先获得函数指针
	// record = findSymbolRecord(functionDefinition->functionID.first);
	// record->functionPtr = functionDefinition->codeGen();	//返回函数指针（Function::Create的返回值）
	
	//对compound进行语义分析(在这一步获取函数返回值funcRetValue)
	SemanticAnalyseStatement(reinterpret_cast<_Statement *>(functionDefinition->compound),1);

	layer--; //层数--
	//CreateRet
	functionDefinition->CreateFuncRet(record->funcRetValue);
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
	if (mainSymbolTable->idToLoc[PID.first].size() > 0) //此处默认所有函数、过程不得同名
	{
		int loc = mainSymbolTable->idToLoc[PID.first].top();
		string Tname = mainSymbolTable->recordList[loc]->flag;
		if (Tname == "program" || Tname == "procedure" || Tname == "function") //形参不与三者program、function、procedure即可
		{
			semanticWarningInformation.push_back("[Invalid formal parameter name!] <Line " + itos(PID.second) + ">" + "The formal parameter \"" + PID.first + "\"" + " has the same name as the " + Tname + " \"" + mainSymbolTable->recordList[loc]->id + "\" at line " + itos(mainSymbolTable->recordList[loc]->lineNumber) + ".");
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
}

//对语句进行语义分析
void SemanticAnalyseStatement(_Statement *statement, int flag)
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
			SemanticAnalyseStatement(compound->statementList[i],1);
	}
	else if (statement->type == "repeat")
	{
		_RepeatStatement *repeatStatement = reinterpret_cast<_RepeatStatement *>(statement);
		expflag = 0;
		string type = SemanticAnalyseExpression(repeatStatement->condition);
		expflag = 1;
		if (type != "boolean")
		{ // repeat语句类型检查,condition表达式类型检查 checked
			addExpressionTypeErrorInformation(repeatStatement->condition, type, "boolean", "condition of repeat-until statement");
			repeatStatement->statementType = "error";
		}
		else
			repeatStatement->statementType = "void";
		expflag = 0;
		for (int i = 0; i < repeatStatement->_do.size(); ++i)
			SemanticAnalyseStatement(repeatStatement->_do[i],0); //对循环体语句进行语义分析
		expflag = 1;
		if(flag == 1)
			repeatStatement->codeGen();
	}
	else if (statement->type == "while")
	{
		_WhileStatement *whileStatement = reinterpret_cast<_WhileStatement *>(statement);
		expflag = 0;
		string type = SemanticAnalyseExpression(whileStatement->condition);
		expflag = 1;
		if (type != "boolean")
		{ // while语句类型检查,condition表达式类型检查 checked
			addExpressionTypeErrorInformation(whileStatement->condition, type, "boolean", "condition of while statement");
			whileStatement->statementType = "error";
		}
		else
			whileStatement->statementType = "void";
		expflag = 0;
		SemanticAnalyseStatement(whileStatement->_do,0); //对循环体语句进行语义分析
		expflag = 1;
		if(flag == 1 && whileStatement->statementType != "error")
			whileStatement->codeGen();
	}
	else if (statement->type == "for")
	{
		_ForStatement *forStatement = reinterpret_cast<_ForStatement *>(statement);
		//检查循环变量是否已经定义，如已经定义，是否为integer型变量
		_SymbolRecord *record = findSymbolRecord(forStatement->id.first);
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
		expflag = 0;
		string type = SemanticAnalyseExpression(forStatement->start);
		expflag = 1;
		if (type != "integer")
		{ // checked
			addExpressionTypeErrorInformation(forStatement->start, type, "integer", "start value of for statement");
			forStatement->statementType = "error";
		}
		expflag = 0;
		type = SemanticAnalyseExpression(forStatement->end);
		expflag = 1;
		if (type != "integer")
		{ // checked
			addExpressionTypeErrorInformation(forStatement->end, type, "integer", "end value of for statement");
			forStatement->statementType = "error";
		}
		//对循环体语句进行语义分析
		expflag = 0;
		SemanticAnalyseStatement(forStatement->_do,0);
		expflag = 1;
		if(flag == 1 && forStatement->statementType != "error")
			forStatement->codeGen();
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
		SemanticAnalyseStatement(ifStatement->then,0); //对then语句进行语义分析
		if (ifStatement->els != NULL)				 //对else语句进行语句分析
			SemanticAnalyseStatement(ifStatement->els,0);
		if(flag == 1 && ifStatement->statementType != "error")
			ifStatement->codeGen();
	}
	else if (statement->type == "case")
	{
		cout<<"caseseanti"<<endl;
		_CaseStatement *caseStatement = reinterpret_cast<_CaseStatement *>(statement);
		string type = SemanticAnalyseExpression(caseStatement->caseid);
		if (type != "integer" && type != "char" && type != "boolean" && type != "real")
		{	//非基本类型不能作case判断
			addExpressionTypeErrorInformation(caseStatement->caseid, type, "integer,char,real or boolean", "case statement");
			caseStatement->statementType = "error";
			return;
		}
		else
			caseStatement->statementType = "void";

		for (int i = 0; i < caseStatement->branch.size();++i)
		{
			SemanticAnalyseStatement(caseStatement->branch[i]->_do,0);
		}
		if (flag == 1 && caseStatement->statementType != "error")
			caseStatement->codeGen();
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
		else if (mainSymbolTable->custom[assignStatement->variantReference->variantId.first].size() > 0)
		{	//左值为自定义类型名
			addGeneralErrorInformation("[Type name as l-value error!] <Line" + itos(assignStatement->variantReference->variantId.second) + "> Type name \"" + assignStatement->variantReference->variantId.first + "\" can't be referenced as l-value.");
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
			
			if(leftType != "error" && rightType != "error" && assignStatement->statementType != "error" && flag == 1)
				assignStatement->codeGen(leftType, rightType);

			return;
		}
		//左值和右值类型不同时
		if (leftType != rightType)
		{
			//只支持整型到实型的隐式转换
			if (leftType == "real" && rightType == "integer")
			{
				semanticWarningInformation.push_back("[Implicit type conversion warning!] <Line" + itos(assignStatement->lineNo) + "> Assign a integer varible to a real variable.\n");
			}
			else
			{
				addAssignTypeMismatchErrorInformation(assignStatement->variantReference, assignStatement->expression);
				assignStatement->statementType = "error";
			}
		}

		//codeGen
		if(leftType != "error" && rightType != "error" && assignStatement->statementType != "error" && flag == 1){
			_SymbolRecord* leftVar = findSymbolRecord(assignStatement->variantReference->variantId.first);
			leftVar->llValue = assignStatement->codeGen(leftType, rightType);	//返回左值的llValue
		}
	}

	else if (statement->type == "procedure") //过程调用
	{
		_ProcedureCall *procedureCall = reinterpret_cast<_ProcedureCall *>(statement);
		//通过procedureId查表，获得参数个数、参数类型等信息
		_SymbolRecord *record = findSymbolRecord(procedureCall->procedureId.first);
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
				return;
			}
		}
		if (record->id == "read")
		{ //参数只能是变量或数组元素，不能是常量、表达式等
			bool f = 0;
			int type_arr[procedureCall->actualParaList.size()];
			for (int i = 0; i < procedureCall->actualParaList.size(); i++)
			{
				expflag = 0;
				string actualType = SemanticAnalyseExpression(procedureCall->actualParaList[i]);
				expflag = 1;
				// checked
				if (!(procedureCall->actualParaList[i]->type == "var" && (procedureCall->actualParaList[i]->variantReference->kind == "var" || procedureCall->actualParaList[i]->variantReference->kind == "array")))
					addactualParameterOfReadErrorInformation(procedureCall->actualParaList[i]->lineNo, record->id, i + 1, procedureCall->actualParaList[i]);
				if (actualType == "error")
				{
					procedureCall->statementType = "error";
					f = 1;
				}
				if (actualType == "integer")
					type_arr[i] = 4;
				else if (actualType == "real")
					type_arr[i] = 5;
				else if (actualType == "char")
					type_arr[i] = 6;
				else if (actualType == "boolean")
					type_arr[i] = 7;
			}
			if (!f)
				procedureCall->readcodeGen(type_arr);
			return;
		}
		if (record->id == "write")
		{
			int f = 0;
			int type_arr[procedureCall->actualParaList.size()];
			for (int i = 0; i < procedureCall->actualParaList.size(); i++)
			{
				expflag = 0;
				string actualType = SemanticAnalyseExpression(procedureCall->actualParaList[i]);
				expflag = 1;
				// checked
				if (actualType == "error")
				{
					procedureCall->statementType = "error";
					f = 1;
				}
				if (actualType == "integer")
					type_arr[i] = 0;
				else if (actualType == "real")
					type_arr[i] = 1;
				else if (actualType == "char")
					type_arr[i] = 2;
				else if (actualType == "boolean")
					type_arr[i] = 3;
			}
			if (!f)
				procedureCall->writecodeGen(type_arr);
			return;
		}

		if (record->amount == -1) //如果是变参过程 本编译器支持从变参过程只有read和write
		{
			for (int i = 0; i < procedureCall->actualParaList.size(); i++)
			{
				expflag = 0;
				string actualType = SemanticAnalyseExpression(procedureCall->actualParaList[i]);
				expflag = 1;
				if (actualType == "error") //要求支持变参过程的参数类型不能为error
					procedureCall->statementType = "error";
			}
			procedureCall->codeGen();
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
			expflag = 0;
			string actualType = SemanticAnalyseExpression(procedureCall->actualParaList[i]);
			expflag = 1;
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
						semanticWarningInformation.push_back("[Implicit type conversion warning!] <Line" + itos(procedureCall->lineNo) + "> The " + itos(i + 1) + "th actual parameter of procedure call is integer while the corresponding formal parameter is real.\n");
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

		//codeGen
		procedureCall->codeGen();
	}
	else
	{
		cout << "[SemanticAnalyseStatement] statement type error" << endl;
		return;
	}
}

//对函数调用进行语义分析
string SemanticAnalyseFunctionCall(_FunctionCall *functionCall, int line)
{
	//cout<<"SemanticAnalyseFunctionCall: "<<functionCall->functionId.first<<endl;
	if (functionCall == NULL)
	{
		cout << "[SemanticAnalyseFunctionCall] pointer of _FunctionCall is null" << endl;
		return "error";
	}

	std::pair<string, int> FCID = functionCall->functionId;
	//if (mainSymbolTable->idToLoc.count(FCID.first) == 0) //找不到函数声明
	//[hmz]
	//if (mainSymbolTable->idToLoc[FCID.first].size() == 0) //找不到函数声明
	//[pl]
	_SymbolRecord *record = findSymbolRecord(FCID.first);
	if (record == NULL)
	{ //未定义
		cout<<"[SemanticAnalyseFunctionCall]	function undefined in this scope: "<<FCID.first<<endl;
		addUndefinedErrorInformation(FCID.first, FCID.second);
		return "error";
	}
	
	//int decID = mainSymbolTable->idToLoc[FCID.first].top();
	//functionCall->returnType = mainSymbolTable->recordList[decID]->type;
	if (record->flag != "function")
	{ //标识符对应的记录不是function
		addPreFlagErrorInformation(FCID.first, FCID.second, "function", record->lineNumber, record->flag);
		return "error";
	}

	//int decParaSize = mainSymbolTable->recordList[decID]->amount;
	//if (decParaSize != functionCall->actualParaList.size()) //参数表数目不对
	int decParaSize = record->amount;						//形参个数
	int ParaSize = functionCall->actualParaList.size();
	if (decParaSize != ParaSize) //参数表数目不对
	{
		addNumberErrorInformation(FCID.first, FCID.second, functionCall->actualParaList.size(), decParaSize, "function");
		return "error";
	}

	//int ParaSize = functionCall->actualParaList.size();
	//if(ParaSize > decParaSize)
	//	ParaSize = decParaSize;
	for (int i = 0; i < ParaSize; ++i) //判断每个参数的类型
	{
		//SemanticAnalyseExpression(functionCall->actualParaList[i]);
		//string decType = mainSymbolTable->recordList[decID + i + 1]->type;
		//string expType = functionCall->actualParaList[i]->expressionType;
		//string flag = mainSymbolTable->recordList[decID + i + 1]->flag;
		//if (flag == "var parameter")
		//{
		//	if (expType != decType)
		//		addUsageTypeErrorInformation("arg no." + itos(i + 1), FCID.second, expType, FCID.first, decType);
		expflag = 0;
		string actualType = SemanticAnalyseExpression(functionCall->actualParaList[i]);
		
		expflag = 1;
		string formalType = record->findXthFormalParaType(i);
		bool isRefered = record->isXthFormalParaRefered(i); //检查是否是传引用参数
		if (isRefered)
		{ //引用参数对应的实参不能是复杂表达式或者常量
			if (!(functionCall->actualParaList[i]->type == "var" && (functionCall->actualParaList[i]->variantReference->kind == "var" || functionCall->actualParaList[i]->variantReference->kind == "array")))
			{
				addGeneralErrorInformation("[Referenced actual parameter error!] <Line " + itos(functionCall->actualParaList[i]->lineNo) + "> The " + itos(i + 1) + "th actual parameter expression should be a normal variable、value parameter、referenced parameter or array element.");
				return "error";
			}
			else if (actualType != formalType)
			{
				addExpressionTypeErrorInformation(functionCall->actualParaList[i], actualType, formalType, itos(i + 1) + "th actual parameter of function call of \"" + FCID.first + "\"");
				return "error";
			}
		}
		else
		{
			//if (expType != decType && !(expType == "integer" && decType == "real"))
			//{
			//	addUsageTypeErrorInformation("arg no." + itos(i + 1), FCID.second, expType, FCID.first, decType);
			//}
			//else if (expType == "integer" && decType == "real")
			if (actualType != formalType) //如果类型不一致
			{
				//semanticWarningInformation.push_back("[Implicit type conversion warning!] <Line" + itos(FCID.second) + "> The " + itos(i + 1) + "th actual parameter of function call is integer while the corresponding formal parameter is real.\n");
				//传值参数支持integer到real的隐式类型转换
				if (actualType == "integer" && formalType == "real")
				{
					semanticWarningInformation.push_back("[Implicit type conversion warning!] <Line" + itos(line) + "> The " + itos(i + 1) + "th actual parameter of function call is integer while the corresponding formal parameter is real.\n");
				}
				else
				{
					addExpressionTypeErrorInformation(functionCall->actualParaList[i], actualType, formalType, itos(i + 1) + "th actual parameter of function call of \"" + FCID.first + "\"");
					return "error";
				}
			}
		}
	}
	//return functionCall->returnType;
	return record->type;
}

//对表达式进行语义分析
string SemanticAnalyseExpression(_Expression *&expression)
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
			_SymbolRecord *record = findSymbolRecord(expression->variantReference->variantId.first);
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
		if(variantReferenceType=="boolean"&&expression->variantReference->kind=="constant"){
			_SymbolRecord *record = findSymbolRecord(expression->variantReference->variantId.first);
			expression->boolValue=record->value;
		}
		
		if(expflag == 1)
			expression->llvalue = expression->codeGen();
		return expression->expressionType = variantReferenceType;
	}

	//表达式类型为integer <ok>
	else if (expression->type == "integer")
	{
		expression->totalIntValue = expression->intNum;
		expression->totalIntValueValid = true;
		if(expflag == 1)
			expression->llvalue = expression->codeGen();
		return expression->expressionType = "integer";
	}

	//表达式类型为real <ok>
	else if (expression->type == "real"){
		if(expflag == 1)
			expression->llvalue = expression->codeGen();
		return expression->expressionType = "real";
	}

	//表达式类型为char <ok>
	else if (expression->type == "char"){
		if(expflag == 1)
			expression->llvalue = expression->codeGen();
		return expression->expressionType = "char";
	}

	//表达式类型为布尔类型boolean
	else if (expression->type == "boolean"){
		if(expflag == 1)
			expression->llvalue = expression->codeGen();
		return expression->expressionType = "boolean";
	}

	//表达式类型为函数调用 <ok>
	else if (expression->type == "function") //获得函数调用的返回值类型
	{
		if(expflag == 1)
			expression->llvalue = expression->codeGen();
		return expression->expressionType = SemanticAnalyseFunctionCall(expression->functionCall, expression->lineNo);
	}

	//含有运算符的表达式
	else if (expression->type == "compound")
	{

		//表达式中含有关系运算符relop ||注意：此处的relop包含了词法分析的（relop（>=|>|<=|<|<>）和=），同词法定义不一致
		if (expression->operationType == "relop")
		{
			string epType1 = SemanticAnalyseExpression(expression->operand1);
			string epType2 = SemanticAnalyseExpression(expression->operand2);
			//类型兼容
			if ((epType1 == epType2 && epType1 != "error") || (epType1 == "integer" && epType2 == "real") || (epType1 == "real" && epType2 == "integer")){
				if(expflag == 1)
					expression->llvalue = expression->codeGen()	;
				return expression->expressionType = "boolean";
			}
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
			if (type == "boolean"){
				if(expression->operand1->boolValue=="false")
					expression->boolValue="true";
				else{
					expression->boolValue="false";
				}
				if(expflag == 1)
					expression->llvalue = expression->codeGen();
				
				return expression->expressionType = "boolean";
			}
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
			if (epType == "integer" || epType == "real"){
				if(expflag == 1)
					expression->llvalue = expression->codeGen();
				return expression->expressionType = epType;
			}
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
			//bool类型记录值
			if(expression->expressionType=="boolean"){
				expression->boolValue=expression->operand1->boolValue;
			}
			if(expflag == 1)
				expression->llvalue = expression->codeGen();
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
				if(expflag == 1)
					expression->llvalue = expression->codeGen();
				// if (epType1 == "integer" && epType2 == "integer")
				// 	return expression->expressionType = "integer";
				//如果是除法（"/"），则运算结果一定为浮点型
				if (epType1 == "integer" && epType2 == "integer"){
					if(expression->operation == "/")
						return expression->expressionType = "real";
					return expression->expressionType = "integer";
				}
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
				if(expflag == 1)
					expression->llvalue = expression->codeGen();
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
			//bool值记录结果
			if (epType1 == "boolean" && epType2 == "boolean"){
				if(expression->operation=="and"){
					if(expression->operand1->boolValue=="true"&&expression->operand2->boolValue=="true")
						expression->boolValue="true";
					else 
						expression->boolValue="false";
				}
				else{
					if(expression->operand1->boolValue=="true"||expression->operand2->boolValue=="true")
						expression->boolValue="true";
					else 
						expression->boolValue="false";
				}
				if(expflag == 1)
					expression->llvalue = expression->codeGen();
				return expression->expressionType = "boolean";
			}
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

	_SymbolRecord *record = findSymbolRecord(variantReference->variantId.first);

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
		if (record->flag == "normal variant")
		{
			//获取类型别名记录
			_SymbolRecord *recordSource = findSymbolRecord(record->type);
			//若存在别名记录则进行替换
			if (recordSource != NULL)
			{
				//数组、结构体变量不作替换
				if(recordSource->flag=="array"||recordSource->flag=="records"){
					variantReference->kind=recordSource->flag;
					return variantReference->variantType = record->type;
				}
				return variantReference->variantType = recordSource->type;
			}


		}
		return variantReference->variantType = record->type;
	}

	//数组元素
	else if(record->flag =="array"){
		variantReference->kind = "var";

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
				variantReference->kind="var";	
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
		variantReference->kind="var";
		string curType = record->type;

		for (int n = 0; n < variantReference->IdvpartList.size(); n++)
		{

			//if (curType == "integer" || curType == "real" || curType == "char" || curType == "boolean")
			if(context.typeSystem.isBasicType(curType))
			{
				addVariantReferenceErrorInformation(variantReference->variantId.second, "illegal array element access or illegal record attribute access");
				return variantReference->variantType = "error";
			}
			//获取类型定义记录
			_SymbolRecord *recordSource = mainSymbolTable->recordList[mainSymbolTable->idToLoc[curType].top()];

			if (recordSource == NULL)
			{
				string info = variantReference->variantId.first;
				info += ": type ";
				info += record->type;
				info += " is not defined;";
				addVariantReferenceErrorInformation(variantReference->variantId.second, info);
				return variantReference->variantType = "error";
			}

			//构造变量引用
			_VariantReference *curVariantReference = new _VariantReference();
			curVariantReference->variantId = make_pair(curType, variantReference->variantId.second);
			curVariantReference->locFlag = 0;

			//属性字段
			if (recordSource->flag == "records")
			{
				curVariantReference->IdvpartList.push_back(variantReference->IdvpartList[n]);
			}
			//数组元素字段
			else if (recordSource->flag == "array")
			{
				for (int s = 0; s < recordSource->amount && n + s < variantReference->IdvpartList.size(); s++)
					curVariantReference->IdvpartList.push_back(variantReference->IdvpartList[n + s]);
				n += recordSource->amount - 1;
			}
			else
			{
				cout << "[SemanticAnalyseVariantReference]	flag of " << variantReference->variantId.first << " is not record or array" << endl;
			}

			//获取当前类型
			curType = SemanticAnalyseVariantReference(curVariantReference);

			//错误信息已经记录在了递归子程序中，此处不重复记录
			if (curType == "error")
				return variantReference->variantType = "error";
		}
		//if(!(curType == "integer" || curType == "real" || curType == "char" || curType == "boolean"))
		if(!context.typeSystem.isBasicType(curType))
			variantReference->kind=curType;

		return variantReference->variantType = curType;
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
	if (preLineNumber >= 0)
		errorInformation += "\"" + preId + "\"" + " has already been defined as a " + preFlag + " at line " + itos(preLineNumber) + ".";
	else if(preLineNumber == -1)
		errorInformation += "\"" + preId + "\"" + " has already been defined as a lib program.";
	else 
		errorInformation += "\"" + preId + "\"" + " has the same name as the main program.";

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
	string errorInformation  = "[Usage type error!] <Line " + itos(curLineNumber) + "> ";
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
	//errorInformation += "Left \"" + varRef + "\" type is " + leftVariantReference->variantType + " while right \"" + exp + "\" type is " + rightExpression->expressionType + ".";
	errorInformation += "Left type is " + leftVariantReference->variantType + " while right type is " + rightExpression->expressionType + ".";
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

// void addReadBooleanErrorInformation(_Expression *exp, int X)
// {
// 	string errorInformation = "[Read boolean error!] <Line " + itos(exp->lineNo) + "> ";
// 	string expression;
// 	// inputExpression(exp, expression, 1);
// 	errorInformation += "The " + itos(X) + "th actual parameter of read \"" + expression + "\" is boolean, it can't be read.";
// 	semanticErrorInformation.push_back(errorInformation);
// 	// CHECK_ERROR_BOUND
// }

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
void addVariantReferenceErrorInformation(int line, string info)
{
	string errorInformation = "[variable reference error]";
	errorInformation += "<Line " + itos(line) + "> ";
	errorInformation += info;
	semanticErrorInformation.push_back(errorInformation);
}

int isLibName(string id)
{
	if (lib.count(id)) //检查是否与库函数同名
		return -1;
	else if(id == mainSymbolTable->recordList[0]->id)	//检查是否与主程序同名
		return -2;
	return 0;
}

bool isReDef(string id,int& loc)
{
	if (mainSymbolTable->idToLoc[id].size() > 0) //如果此常量已经出现过
	{
		loc = mainSymbolTable->idToLoc[id].top(); //获取最近一次出现的位置
		if (mainSymbolTable->indexTable.back() < loc)		 //如果和当前变量在同一个作用域
			return 1;
	}
	return 0;
}
