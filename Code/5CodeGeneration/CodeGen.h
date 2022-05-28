#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/Support/raw_ostream.h>

#include<stdio.h>   //LogError: fprintf
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<memory>    //shared_ptr, unique_ptr

#include "ASTnodes.h"
#include "symbolTable.h"
#include "TypeSystem.h"
//#include "yacc.tab.h"
//#include "yacc.tab.cpp"

#define ISTYPE(value, id) (value->getType()->getTypeID() == id)

using namespace std;
//using legacy::PassManager;

class CodeGenContext;

extern CodeGenContext context;
extern _SymbolTable *mainSymbolTable;
extern _SymbolRecord* findSymbolRecord(string id);

extern string itos(int num); //将int转化为string

class CodeGenContext {
public:
    llvm::LLVMContext llvmContext;
    //llvm::IRBuilder<> builder;
    unique_ptr<llvm::IRBuilder<>> builder;
    unique_ptr<llvm::Module> Module;
    TypeSystem typeSystem;    //自定义类型系统

    //CodeGenContext(): builder(llvmContext), typeSystem(llvmContext) {
    CodeGenContext(): typeSystem(llvmContext) {
        Module = unique_ptr<llvm::Module>(new llvm::Module("Program", this->llvmContext));
    }

    void InitCodeGen();
};

//用于报错的函数：
//unique_ptr<_Expression> LogError(const char *str)
llvm::Value* LogError(const char *str);
llvm::Value * LogErrorV(string str);

//获取数组元素的指针
llvm::Value* getArrayItemPtr(string varType, llvm::Value* addr, int loc);

//获取record成员的指针
llvm::Value* getRecordItemPtr(string varType, llvm::Value* addr, string memberId);

//获取数组元素/record成员的指针     (调用getArrayItemPtr和getRecordItemPtr)
llvm::Value* getItemPtr(_VariantReference* varRef);

//计算N维数组下标（N>=1）   (按C标准，从0开始)
int calcArrayIndex(string arrTypeName, vector<_Idvpart*> IdvpartList);

#endif
