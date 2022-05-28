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
    //类型名是type部分起的别名
    else if(mainSymbolTable->custom.find(type) != mainSymbolTable->custom.end()){
        int loc = mainSymbolTable->custom[type].top();
        return this->getllType(mainSymbolTable->recordList[loc]->type);
    }
    else
    {
        //报错：未知类型
        LogErrorV("[getllType]  Unknown type: " + type);
        return nullptr;
    }
}

 //新增record对应的LLVM类型
void TypeSystem::addRecordType(string name, llvm::StructType* type){
    recordTypes[name] = type;
    recordMembers[name] = vector<NameType>();
}

//新增record成员
void TypeSystem::addRecordMember(string recName, string memName, string memType){
    cout << "addRecordMember: " << recName + "."+ memName + ", type: " + memType << endl;
    recordMembers[recName].push_back(make_pair(memName, memType));
}


//获取record中成员所在的位置
long TypeSystem::getRecordMemberIndex(string recName, string memName){
    cout<<"getRecordMemberIndex: "<< recName + "." + memName<<endl;
    auto& members = recordMembers[recName];
    for(auto it=members.begin(); it!=members.end(); it++)
    {
        if(it->first == memName)
            return distance(members.begin(), it);
    }
    //报错：未知的record成员
    LogErrorV("[getRecordMemberIndex]   Unknown record member: " + recName + "." + memName);
    return -1;
}

//获取record成员类型名
string TypeSystem::getRecordMemberType(string recName, string memName)
{
    vector<NameType> members = recordMembers[recName];
    for(int i=0; i < members.size(); i++){
        if(members[i].first == memName)
            return members[i].second;
    }
    LogErrorV("[getRecordMemberType]    Unknown record member: " + recName + "." + memName);
    return "error_type";
}

//新增数组变量对应的LLVM类型
void TypeSystem::addArrayType(string typeName, llvm::ArrayType* ArrayType, string memberType, vector<pair<int,int>> arrayRangeList){
    arrayTypes[typeName] = ArrayType;
    arrayMemberTypes[typeName] = memberType;
    vector<pair<int, int>> rangeList(arrayRangeList);
    arrayRangeLists[typeName] = rangeList;
}

//获取数组元素类型名
string TypeSystem::getArrayMemberType(string arrName){
    _SymbolRecord* record = findSymbolRecord(arrName);
    if(!record){
        LogErrorV("[getArrayMemberType] Cannot find symbol record: " + arrName);
        return "";
    }
    if(this->isBasicType(record->type))   //数组元素类型为基本类型->没有下层嵌套了，直接返回原数组类型
        return arrName;
    return arrayMemberTypes[arrName];   //否则返回下层的数组类型名
}

//判断是否为基本类型
bool TypeSystem::isBasicType(string type)
{
    if(type == "integer" || type == "real" || type == "char" || type == "boolean")
        return true;
    return false;
}
