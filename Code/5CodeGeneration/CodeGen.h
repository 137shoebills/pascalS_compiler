#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/IRPrintingPasses.h>
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/TargetSelect.h"

#include <llvm/Support/raw_ostream.h>
// #include "llvm/Support/TargetSelect.h"
// #include "llvm/Support/TargetRegistry.h"
// #include "llvm/Support/TargetSelect.h"
// #include "llvm/Target/TargetMachine.h"
// #include "llvm/Target/TargetOptions.h"
// #include "llvm/ExecutionEngine/ExecutionEngine.h"

#include <stdio.h> //LogError: fprintf
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory> //shared_ptr, unique_ptr

#include "ASTnodes.h"
#include "symbolTable.h"
#include "TypeSystem.h"
//#include "yacc.tab.h"
//#include "yacc.tab.cpp"

#define ISTYPE(value, id) (value->getType()->getTypeID() == id)

using namespace std;
// using legacy::PassManager;

class CodeGenContext;

extern bool have_error;

extern CodeGenContext context;
extern _SymbolTable *mainSymbolTable;
extern _SymbolRecord *findSymbolRecord(string id);

extern string itos(int num); //将int转化为string

class CodeGenContext
{
public:
    llvm::LLVMContext llvmContext;
    // llvm::IRBuilder<> builder;
    unique_ptr<llvm::IRBuilder<>> builder;
    unique_ptr<llvm::Module> Module;
    TypeSystem typeSystem; //自定义类型系统

    // CodeGenContext(): builder(llvmContext), typeSystem(llvmContext) {
    CodeGenContext() : typeSystem(llvmContext)
    {
        // Module = make_unique<llvm::Module>("Program", this->llvmContext);
        Module = make_unique<llvm::Module>("main", this->llvmContext);
        // builder = make_unique<llvm::IRBuilder<>>(llvmContext);
    }

    void InitCodeGen();
};

//用于报错的函数：
// unique_ptr<_Expression> LogError(const char *str)
llvm::Value *LogError(const char *str);
llvm::Value *LogErrorV(string str);

//获取数组元素的指针
llvm::Value *getArrayItemPtr(string varType, llvm::Value *addr, int loc);

//获取record成员的指针
llvm::Value *getRecordItemPtr(string varType, llvm::Value *addr, string memberId);

//获取数组元素/record成员的指针     (调用getArrayItemPtr和getRecordItemPtr)
llvm::Value *getItemPtr(_VariantReference *varRef);

//计算N维数组下标（N>=1）   (按C标准，从0开始)
int calcArrayIndex(string arrTypeName, vector<_Idvpart *> IdvpartList);

#endif
