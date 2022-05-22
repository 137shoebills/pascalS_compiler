#include "TypeSystem.h"

TypeSystem::TypeSystem(llvm::LLVMContext& context): llvmContext(context){}

//获取标识符对应的LLVM类型
llvm::Type* TypeSystem::getllType(string type){
    if(type == "integer")
        return this->intTy;
    if(type == "real")
        return this->realTy;
    if(type == "char")
        return this->charTy;
    if(type == "boolean")
        return this->boolTy;
    if(type == "void")
        return this->voidTy;
    if(type == "int64")
        return this->int64Ty;

    //数组类型(这里type是数组的类型名)
    if (this->arrayTypes.find(type) != this->arrayTypes.end())
        return this->arrayTypes[type];
    // record类型
    else if (this->recordTypes.find(type) != this->recordTypes.end())
        return this->recordTypes[type];
    else
    {
        //报错：未知类型
        return LogErrorV("[getllType]  Unknown type: " + type);
    }
}

 //新增record对应的LLVM类型
void TypeSystem::addRecordType(string name, llvm::StructType* type){
    recordTypes[name] = type;
    recordMembers[name] = vector<NameType>();
}

//新增record成员
void TypeSystem::addRecordMember(string recName, string memName, string memType){
    // if(recordTypes.find(recName) == recordTypes.end()){
    //     //报错：未定义的record类型
    //     //semanticErrorInformation.push_back("Error: Undefined record type: " + recName);
    // }
    recordMembers[recName].push_back(make_pair(memName, memType));
}


//获取record中成员所在的位置
long TypeSystem::getRecordMemberIndex(string recName, string memName){
    auto& members = recordMembers[recName];
    for(auto it=members.begin(); it!=members.end(); it++)
    {
        if(it->second == memName)
            return distance(members.begin(), it);
    }
    //报错：未知的record成员
    LogErrorV("[getRecordMemberIndex]   Unknown record member: " + recName + "." + memName);
    return -1;
}

//新增数组变量对应的LLVM类型
void TypeSystem::addArrayType(string typeName, llvm::ArrayType* type, int lineNo){
    arrayTypes[typeName] = type;
}
