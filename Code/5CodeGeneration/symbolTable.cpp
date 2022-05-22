/*
符号表实现代码
*/
#include "symbolTable.h"
#include <iostream>
using namespace std;

_SymbolTable *mainSymbolTable; //主符号表
_SymbolTable *currentSymbolTable;

_SymbolRecord* findSymbolRecord(_SymbolTable *currentSymbolTable, string id);

void _SymbolTable::addPara(string id, int lineNumber, string type)
{
	_SymbolRecord *tmpRecord = new _SymbolRecord;
	tmpRecord->setPara(id, lineNumber, type);
	this->recordList.push_back(tmpRecord);
	this->idToLoc[id].push(int(recordList.size() - 1));
}

void _SymbolTable::addVarPara(string id, int lineNumber, string type)
{
	_SymbolRecord *tmpRecord = new _SymbolRecord;
	tmpRecord->setVarPara(id, lineNumber, type);
	this->recordList.push_back(tmpRecord);
	this->idToLoc[id].push(int(recordList.size() - 1));
}

void _SymbolTable::addVar(string id, int lineNumber, string type)
{
	_SymbolRecord *tmpRecord = new _SymbolRecord;
	tmpRecord->setVar(id, lineNumber, type);
	this->recordList.push_back(tmpRecord);
	this->idToLoc[id].push(int(recordList.size() - 1));
}

void _SymbolTable::addConst(string id, int lineNumber, string type, bool isMinusShow, string value)
{
	_SymbolRecord *tmpRecord = new _SymbolRecord;
	tmpRecord->setConst(id, lineNumber, type, isMinusShow, value);
	this->recordList.push_back(tmpRecord);
	this->idToLoc[id].push(int(recordList.size() - 1));
}

void _SymbolTable::addArray(string id, int lineNumber, string type, int amount, vector<pair<int, int> > arrayRangeList)
{
	_SymbolRecord *tmpRecord = new _SymbolRecord;
	tmpRecord->setArray(id, lineNumber, type, amount, arrayRangeList);
	this->recordList.push_back(tmpRecord);
	this->idToLoc[id].push(int(recordList.size() - 1));
	this->custom[id].push(int(recordList.size()-1));	//将array类型记录到自定义类型表custom中
}

void _SymbolTable::addProcedure(string id, int lineNumber, int amount, vector<_FormalParameter *> paras)
{
	_SymbolRecord *tmpRecord = new _SymbolRecord;
	tmpRecord->setProcedure(id, lineNumber, amount, paras);
	this->recordList.push_back(tmpRecord);
	this->idToLoc[id].push(int(recordList.size() - 1));
	//过程入栈，索引表记录
	indexTable.push_back(int(recordList.size() - 1));
}

void _SymbolTable::addFunction(string id, int lineNumber, string type, int amount,vector<_FormalParameter*> paras)
{
	_SymbolRecord *tmpRecord = new _SymbolRecord;
	tmpRecord->setFunction(id, lineNumber, type, amount, paras);
	this->recordList.push_back(tmpRecord);
	this->idToLoc[id].push(int(recordList.size() - 1));
	//函数入栈，索引表记录
	indexTable.push_back(int(recordList.size() - 1));
}

//增加子符号表
// void _SymbolTable::addSubSymbolTable(string id, _SymbolTable* symbolTable) {
// 	if (idToLoc.count(id)) {
// 		int loc = idToLoc[id];
// 		recordList[loc]->subSymbolTable = symbolTable;
// 	}
// 	else
// 		cout << "[_SymbolTable::addSubSymbolTable] ERROR: subprogram id not found" << endl;
// }

void _SymbolTable::addVoidPara(string id, int lineNumber)
{
	_SymbolRecord *tmpRecord = new _SymbolRecord;
	tmpRecord->setVoidPara(id, lineNumber);
	this->recordList.push_back(tmpRecord);
	this->idToLoc[id].push(int(recordList.size() - 1));
}

void _SymbolTable::addRecords(string id, int lineNumber, vector<_SymbolRecord *> records)
{
	_SymbolRecord *tmpRecord = new _SymbolRecord;
	tmpRecord->setRecords(id, lineNumber, records);
	this->recordList.push_back(tmpRecord);
	this->idToLoc[id].push(int(recordList.size() - 1));
	this->custom[id].push(int(recordList.size()-1));	//将record类型记录到自定义类型表custom中
}

void _SymbolTable::addProgram(string id, int lineNumber, int amount, string returnType)
{
	_SymbolRecord *tmpRecord = new _SymbolRecord;
	tmpRecord->setProgram(id, lineNumber, amount, returnType);
	this->recordList.push_back(tmpRecord);
	this->idToLoc[id].push(int(recordList.size() - 1));
	indexTable.push_back(int(recordList.size() - 1));
}

// void _SymbolTable::addSubProgram(string id, int lineNumber,int amount, string returnType){
// 	_SymbolRecord* tmpRecord = new _SymbolRecord;
// 	tmpRecord->setSubProgram(id, lineNumber,amount,returnType);
// 	this->recordList.push_back(tmpRecord);
// 	this->idToLoc[id].push(int(recordList.size() - 1));
//     indexTable.push_back(int(recordList.size() - 1));
// }

_SymbolTable::_SymbolTable()
{
	recordList.clear();
	idToLoc.clear();
	indexTable.clear();
}

//有关符号记录的函数

void _SymbolRecord::setPara(string id, int lineNumber, string type)
{
	flag = "value parameter";
	this->id = id;
	this->lineNumber = lineNumber;
	this->type = type;
}

void _SymbolRecord::setVarPara(string id, int lineNumber, string type)
{
	flag = "var parameter";
	this->id = id;
	this->lineNumber = lineNumber;
	this->type = type;
}

void _SymbolRecord::setVar(string id, int lineNumber, string type)
{
	flag = "normal variant";
	this->id = id;
	this->lineNumber = lineNumber;
	this->type = type;
}

void _SymbolRecord::setConst(string id, int lineNumber, string type, bool isMinusShow, string value)
{
	flag = "constant";
	this->id = id;
	this->lineNumber = lineNumber;
	this->type = type;
	this->isMinusShow = isMinusShow;
	this->value += value;
}

void _SymbolRecord::setArray(string id, int lineNumber, string type, int amount, vector<pair<int, int> > arrayRangeList)
{
	flag = "array";
	this->id = id;
	this->lineNumber = lineNumber;
	this->type = type;
	this->amount = amount;
	this->arrayRangeList = arrayRangeList;
}

void _SymbolRecord::setProcedure(string id, int lineNumber, int amount, vector<_FormalParameter *> paras)
{
	flag = "procedure";
	this->id = id;
	this->lineNumber = lineNumber;
	this->amount = amount;
	this->paras = paras;
}

void _SymbolRecord::setFunction(string id, int lineNumber, string type, int amount, vector<_FormalParameter*> paras)
{
	flag = "function";
	this->id = id;
	this->lineNumber = lineNumber;
	this->type = type;
	this->amount = amount;
	this->paras = paras;
}

//这是一条特殊的记录，表示当前符号表对应的程序名、行号、"procedure"还是"function"、参数个数
//如果是"function"，则returnType为其返回值类型
//如果是"procedure"，则returnType为""
void _SymbolRecord::setProgram(string id, int lineNumber, int amount, string returnType)
{
	flag = "program";
	this->id = id;
	this->lineNumber = lineNumber;
	this->amount = amount;
	this->type = returnType;
}

// void _SymbolRecord::setSubProgram(string id, int lineNumber, int amount, string returnType) {
// 	flag = "sub program";
// 	this->id = id;
// 	this->lineNumber = lineNumber;
// 	this->amount = amount;
// 	this->type = returnType;
// }

void _SymbolRecord::setVoidPara(string id, int lineNumber)
{
	flag = "parameter of program";
	this->id = id;
	this->lineNumber = lineNumber;
}

void _SymbolRecord::setRecords(string id, int lineNumber, vector<_SymbolRecord *> records)
{
	flag = "records";
	this->id = id;
	this->lineNumber = lineNumber;
	this->records = records;
}

bool _SymbolRecord::checkArrayXthIndexRange(int X, int index)
{
	if ((flag == "array") && X >= 0 && X < amount)
		return index >= arrayRangeList[X].first && index <= arrayRangeList[X].second;
	else
	{
		cout << "[_SymbolRecord::checkArrayXthIndexRange] record is not a array, or index out of range" << endl;
		return false;
	}
}

//找出标识符在符号表中的位置
_SymbolRecord* findSymbolRecord(_SymbolTable *currentSymbolTable, string id)
{
	if (mainSymbolTable->idToLoc[id].size() > 0)
	{
		int loc = mainSymbolTable->idToLoc[id].top();
		return mainSymbolTable->recordList[loc];
	}

	return NULL;
}

//找到第X个形式参数的类型
string _SymbolRecord :: findXthFormalParaType(int X) 
{
	return paras[X]->type;
}

//检查第X个形式参数是否是引用调用
bool _SymbolRecord :: isXthFormalParaRefered(int X)
{
	return paras[X]->flag;
}
