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

    //数组类型(这里type是数组变量的名称)
    if (this->arrayTypes.find(type) != this->arrayTypes.end())
        return this->arrayTypes[type];
    // record类型
    else if (this->recordTypes.find(type) != this->recordTypes.end())
        return this->recordTypes[type];
    else
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

//新增数组变量对应的LLVM类型
void TypeSystem::addArrayType(string name, llvm::ArrayType* type){
    arrayTypes[name] = type;
    //arrayMembers[name] = vector<NameType>();
}

//获取变量对应的LLVM Type(普通变量，数组元素，record成员，函数标识符)
llvm::Type* TypeSystem::getVarllType(_VariantReference* varRef){
    int loc = mainSymbolTable->idToLoc[varRef->variantId.first].top();
    _SymbolRecord* var = mainSymbolTable->recordList[loc];
    llvm::Type* type = nullptr;
    
    if(var->records.size() > 0){   //record成员
        string vpartId = varRef->IdvpartList[0]->IdvpartId.first;
        for(auto it = var->records.begin(); it != var->records.end(); it++)
        {
            if((*it)->id == vpartId)
                type = getllType((*it)->type);
                break;
        }
    }
    else    //普通变量/数组元素/函数返回值类型
        type = getllType(var->type);

    return type;
}
