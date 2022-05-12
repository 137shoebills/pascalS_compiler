#include "TypeSystem.h"

TypeSystem::TypeSystem(llvm::LLVMContext& context): llvmContext(context){}

//获取标识符对应的LLVM类型
llvm::Type* TypeSystem::getllType(string type){
    switch(type){
        case "integer":
            return this->intTy;
        case "real":
            return this->realTy;
        case "char":
            return this->charTy;
        case "boolean":
            return this->boolTy;
        case "void":
            return this->voidTy;
    }
    //record类型
    if(this->recordTypes.find(type) != this->recordTypes.end())
        return this->recordTypes[type];

    return nullptr;
}

 //新增record对应的LLVM类型
void TypeSystem::addRecordType(string name, llvm::StructType* type){
    recordTypes[name] = type;
    recordMembers[name] = vector<NameType>();
}

//新增record成员
void TypeSystem::addRecordMember(string recName, int lineNo, string memName, string memType){
    if(recordTypes.find(recName) == recordTypes.end()){
        //报错：未定义的record类型
        semanticErrorInformation.push_back((string) "line:" + char('0' + lineNo) + "Error: Undefined record type: " + recName);
    }
    recordMembers[recName].push_back(make_pair(memName, memType));
}


//获取record中成员所在的位置下标
long TypeSystem::getRecordMemberIndex(string recName, string memName){
    auto& members = recordMembers[recName];
    for(auto it=members.begin(); it!=members.end(); it++)
    {
        if(it->second == memName)
            return distance(members.begin(), it);
    }
    //报错：未知的record成员
    semanticErrorInformation.push_back("Error: Undefined record member: " + recName + "." + memName);
    return 0;
}
