/*
符号表头文件
*/

//#include <llvm/IR/Value.h>

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "ASTnodes.h"
using namespace std;

class _SymbolTable;

//存储符号表记录
class _SymbolRecord
{
public:
	string flag; //"value parameter"表示传值参数,"var parameter"表示传引用参数,"constant"表示常量,
	//"array"表示数组,"procedure"表示过程,"function"表示函数,"record"表示record集
	//"normal variant"表示普通变量 或者 自定义类型 或者record类型
	//"program"表示该条记录是主程序
	//"parameter of program"表示主程序的参数
	string id;		//表示标识符名称或者程序函数过程名
	int lineNumber; //定义位置的行号
	string type;	//如果是变量/常量，则表示变量/常量类型；
	//如果是数组，则表示数组元素的类型；
	//如果是函数，则表示函数返回值类型，类型本身只能为基本类型,"integer","real","char","boolean"
	string value;	  //如果是常量，则表示常量取值
	bool isMinusShow; //常量前是否带负号
	int amount;		  //数组维数/参数个数。
	//如果是数组，则表示数组维数，如果是函数/过程，则表示参数个数
	vector<pair<int, int> > arrayRangeList; //数组各维上下界
	vector<_SymbolRecord *> records;		//记录records中各个记录
	vector<_FormalParameter*> paras;	//function的形参列表 

	//llvm::Value* llValue;		//LLVM类型值
	
	void setPara(string id, int lineNumber, string type);	//设置传值参数
	void setVarPara(string id, int lineNumber, string type);//设置传引用参数
	void setVar(string id, int lineNumber, string type);	//设置普通变量
	void setConst(string id, int lineNumber, string type, bool isMinusShow, string value);	//设置常量
	void setArray(string id, int lineNumber, string type, int amount, vector<pair<int, int> > arrayRangeList);
	void setProcedure(string id, int lineNumber, int amount, vector<_FormalParameter *> paras);
	void setFunction(string id, int lineNumber, string type, int amount, vector<_FormalParameter*> paras);
	//这是一条特殊的记录，表示当前符号表对应的程序名、行号、"procedure"还是"function"、参数个数
	void setProgram(string id, int lineNumber, int amount, string returnType);
	// void setSubProgram(string id, int lineNumber,int amount, string returnType);
	void setVoidPara(string id, int lineNumber);	//当前符号表所在的程序 中的参数
	void setRecords(string id, int lineNumber, vector<_SymbolRecord *> records);
	void setCustom(string id, int lineNumber, string type); //增加自定义类型

	bool checkArrayXthIndexRange(int X, int index); //检查第X维下标是否越界，true表示越界，false表示未越界
	string findXthFormalParaType(int X); //找到第X个形式参数的类型
	bool isXthFormalParaRefered(int X);	 //检查第X个形式参数是否是引用调用

	_SymbolRecord()
	{
		arrayRangeList.clear();
		// subSymbolTable = NULL;
	}
	~_SymbolRecord() {}
};

class _SymbolTable
{
public:
	// string tableType;//"main"或者"sub"

	vector<_SymbolRecord *> recordList; //记录符号表记录
	vector<int> indexTable;				//建立索引表 indexTable[i]=j : 过程i的记录在符号表中第j个位置
	map<string, stack<int> > idToLoc;	//加快查询速度的map  为什么要用stack ???
	map<string, stack<int> > custom;	//存储自定义类型的名字

	void addPara(string id, int lineNumber, string type);	//传值参数												   //增加参数
	void addVarPara(string id, int lineNumber, string type);//传引用参数												   //增加变量参数
	void addVar(string id, int lineNumber, string type);	//普通变量												   //增加变量类型
	void addConst(string id, int lineNumber, string type, bool isMinusShow, string value);					   //增加常量类型
	void addArray(string id, int lineNumber, string type, int amount, vector<pair<int, int> > arrayRangeList); //增加数组类型
	void addProcedure(string id, int lineNumber, int amoun, vector<_FormalParameter *> paras);				   //增加过程类型
	void addFunction(string id, int lineNumber, string type, int amount,vector<_FormalParameter*> paras);									   //增加函数类型
	void addVoidPara(string id, int lineNumber);															   //表示主程序的参数
	void addRecords(string id, int lineNumber, vector<_SymbolRecord *> records);							   //增加record类型
	void addProgram(string id, int lineNumber, int amount, string returnType);
	void addCustom(string id, int lineNumber, string type); //增加自定义类型
	// void addSubProgram(string id, int lineNumber,int amount, string returnType);


	_SymbolTable();
	~_SymbolTable() {}
};

#endif //! SYMBOLTABLE_H