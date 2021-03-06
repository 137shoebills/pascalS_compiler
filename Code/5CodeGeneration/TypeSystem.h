#ifndef __TYPESYSTEM_H__
#define __TYPESYSTEM_H__

//#include <llvm/IR/Type.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

#include<string>
#include<map>
#include<vector>
#include "ASTnodes.h"
#include "symbolTable.h"
using namespace std;

using NameType = pair<string, string>;  //record成员，pair<id,type>

//extern vector<string> semanticErrorInformation;   //存储错误信息的列表
extern _SymbolTable* mainSymbolTable;
extern llvm::Value* LogErrorV(string str);
extern _SymbolRecord* findSymbolRecord(string);

class TypeSystem{
public:
    llvm::LLVMContext& llvmContext;
    map<string, vector<NameType>> recordMembers;    //record列表
    map<string, llvm::StructType*> recordTypes;     //record类型的名称对应的LLVM Type

    map<string, string> arrayMemberTypes;           //数组元素类型列表
    map<string, vector<pair<int, int>> > arrayRangeLists; //数组上下界列表
    map<string, llvm::ArrayType*> arrayTypes;       //数组变量对应的LLVM Type

public:
    llvm::Type* intTy = llvm::Type::getInt32Ty(llvmContext);
    //llvm::Type* realTy = llvm::Type::getDoubleTy(llvmContext);
    llvm::Type* realTy = llvm::Type::getFloatTy(llvmContext);
    llvm::Type* charTy = llvm::Type::getInt8Ty(llvmContext);
    llvm::Type* boolTy = llvm::Type::getInt1Ty(llvmContext);
    llvm::Type* voidTy = llvm::Type::getVoidTy(llvmContext);
    llvm::Type* int64Ty = llvm::Type::getInt64Ty(llvmContext);

    TypeSystem(llvm::LLVMContext& context);

    //获取string表示的类型对应的LLVM类型（四种基本类型，以及recordName代表的不同record类型）
    llvm::Type* getllType(string type);

     //新增record对应的LLVM类型（处理record声明时）
    void addRecordType(string name, llvm::StructType* type);
    //新增record成员（处理record声明时）
    void addRecordMember(string recName, string memName, string memType);
    //获取record中成员所在的位置（第几个）（处理record成员/record成员赋值时）
    long getRecordMemberIndex(string recName, string memName);
    //获取record成员类型名
    string getRecordMemberType(string recName, string memName);

    //新增数组变量对应的LLVM类型
    void addArrayType(string typeName, llvm::ArrayType* ArrayType, string memberType,
        vector<pair<int,int>> ArrayRangeList);
    //获取数组元素类型名
    string getArrayMemberType(string arrName);

    //判断是否为基本类型
    bool isBasicType(string type);

};

#endif
