#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include <llvm/IR/Value.h>
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
using legacy::PassManager;

extern _SymbolTable *mainSymbolTable;
extern vector<string> semanticErrorInformation;   //存储错误信息的列表
extern vector<string> semanticWarningInformation; //存储警告信息的列表

CodeGenContext context;

class CodeGenContext {
private:
    vector<CodeGenBlock*> blockStack;

public:
    llvm::LLVMContext llvmContext;
    llvm::IRBuilder<> builder;
    unique_ptr<llvm::Module> module;

    TypeSystem typeSystem;    //自定义类型系统

    CodeGenContext(): builder(llvmContext), typeSystem(llvmContext) {
        module = unique_ptr<llvm::Module>(new llvm::Module("main", this->llvmContext));
    }

    void InitCodeGen();
    //[refer]生成中间代码
    //void generatecode(NBlock&);
    void GenerateCode(_Program&);
}

unique_ptr<_Expression> LogError(const char *str)
{
    fprintf(stderr, "LogError: %s\n", str);
    return nullptr;
}

llvm::Value* LogErrorV(string str)
{
    return LogError(str.c_str());
}

//获取数组元素的指针
llvm::Value* getArrayItemPtr(_SymbolRecord* array, vector<_Expression*> indices);

//获取record成员的指针
llvm::Value* getRecordItemPtr(_SymbolRecord* record, string memberId);

#endif
