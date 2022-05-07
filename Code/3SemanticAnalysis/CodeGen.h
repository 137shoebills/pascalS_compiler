#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/Support/raw_ostream.h>

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

class CodeGenBlock{
public:
    llvm::BasicBlock* bb;
    llvm::Value* retValue;

    //[refer]
    //map<string, llvm::Value*> name_Value;
    //直接通过llvm::Value* getType()获取对应的LLVM类型？
    //map<string, shared_ptr<Identifier>> name_type;
    //对于函数参数传递过来的数组要当做指针处理
    //map<string, bool> isFuncArg;
    //map<string, vector<uint64_t>> arraySizes;
}

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

    //根据名称查找LLVM值
    llvm::Value* getIdValue(string name){
        int loc = mainSymbolTable->idToLoc[name].top(); //找到name的最近一次定义所在的位置
        llvm::Value* val = mainSymbolTable->recordList[loc]->llValue;
        return val;
    }

    //[refer]向符号表中插入新记录：标识符的值、类型、是否为函数参数
    /* void setSymbolValue(string name, llvm::Value* value){
        blockStack.back()->name_Value[name] = value;
    }
    void setSymbolType(string name, shared_ptr<Identifier> value){
        blockStack.back()->name_type[name] = value;
    }
    void setFuncArg(string name, bool value){
        //cout << "Set " << name << " as func arg" << endl;
        blockStack.back()->isFuncArg[name] = value;
    } */
    /*⭐
    在SymbolRecord中新增一个字段：llvm::Value* llValue
    在语义分析过程中遇到变量声明时:
    ①根据类型的string表示 获取对应的LLVM类型；
    ②调用context.builder.CreateAlloca()，创建局部变量，返回LLVM Value*类型；
    ③mainSymbolTable->recordList[loc]->llValue = ②的返回值
    */


    //[refer]遇到返回语句，在符号表中记录返回值
    /* void setCurRetValue(llvm::Value* value){
        blockStack.back()->retValue = value;
    }
    //[refer]获取当前(函数)返回值
    llvm::Value* getCurRetValue(){
        return blockStack.back()->retValue;
    } */
    //⭐符号表表项中，flag为函数时，type即为函数返回值类型，llValue即为函数返回值
    //语义分析遇到函数声明时，需要构造LLVM返回值类型：
    /*
    //func:=expr，则func的LLVM返回值=expr.codeGen()
    //通过函数名获取返回值对应的LLVM Value*
    int loc = mainSymbolTable->idToLoc[funcName].top();
    llvm::Value* retVal = mainSymbolTable->recordList[loc]->llValue;
    context.builder.CreateRet(retVal);
    */

    //[refer]CodeGen.cpp, line 95 cast() 获取当前块
    /* llvm::BasicBlock* curBlock() {  //[refer]currentBlock
        return blockStack.back()->bb;
    } */
    //[refer]将当前块压入/弹出
    /* void pushBlock(llvm::BasicBlock* block) {
        CodeGenBlock* codeGenBlock = new CodeGenBlock();
        codeGenBlock->bb = block;
        codeGenBlock->retValue = nullptr;
        blockStack.push_back(codeGenBlock);
    }
    void popBlock(){
        CodeGenBlock * codeGenBlock = blockStack.back();
        blockStack.pop_back();
        delete codeGenBlock;
    } */

    //[refer]检查数组是否作为参数
    /* bool isFuncArg(string name) const{  //判断name是否是函数参数
        for(auto it=blockStack.rbegin(); it!=blockStack.rend(); it++){
            if( (*it)->isFuncArg.find(name) != (*it)->isFuncArg.end() ){
                return (*it)->isFuncArg[name];
            }
        }
        return false;
    } */
    //[refer]数组作为参数
    /* void setArraySize(string name, vector<uint64_t> value){
        //cout << "setArraySize: " << name << ": " << value.size() << endl;
        blockStack.back()->arraySizes[name] = value;
    }
    vector<uint64_t> getArraySize(string name){
        for(auto it=blockStack.rbegin(); it!=blockStack.rend(); it++){
            if( (*it)->arraySizes.find(name) != (*it)->arraySizes.end())
                return (*it)->arraySizes[name];
        }
        return blockStack.back()->arraySizes[name];
    } */
}

#endif