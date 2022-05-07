#include "CodeGen.h"

//static Value* CastToBoolean()...  //用于if-else语句
//static llvm::Value* calcArrayIndex()...   //用于数组

//初始化中间代码生成相关参数
void CodeGenContext::InitCodeGen(){
    vector<llvm::Type*> SysArgs;
	llvm::FunctionType* PrgmType = llvm::FunctionType:get(llvm::Type::getVoidTy(context.llvmContext),SysArgs,false);
    llvm::Function* Program = llvm::Function::Create(PrgmType, GlobalValue::ExternalLinkage, "Program");
    llvm::BasicBlock* block = llvm::BasicBlock::Create(context.llvmContext, "entry");
}

//各种AST节点的CodeGen()----------

//主program
//void _Program::codeGen() {}
//主program begin和end之间的程序体
//void _SubProgram::codeGen() {}
//复合语句，使用statement的codeGen
//llvm::Value* _Compound::codeGen() {}

llvm::Value* _Constant::codeGen() {
    llvm::Value* ret;
    
    if(this->type == "integer"){
        ret = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context.llvmContext), this->intValue, true);//IsSigned=true
    }
    else if(this->type == "real"){
        ret = llvm::ConstantFP::get(llvm::Type::getDoubleTy(context.llvmContext), this->realValue, true);
    }
    else if(this->type == "char"){
        ret = llvm::ConstantInt::get(llvm::Type::getInt8Ty(context.llvmContext), this->charValue, true);
    }
    else if(this->type == "boolean"){
        ret = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context.llvmContext), this->boolValue, true);
    }
    else{
        semanticErrorInformation.push_back((string) "line:" + char('0' + valueId.second) + "Error: Unknown Constant_Type " + valueId.first + ", Type: " + this->type);
    }

    return ret;
}

llvm::Value* _Variant::codeGen() {
    string varType = this->type->type->first;
    llvm::Type* type = context.typeSystem.getllType(varType);
    llvm::Value* addr = nullptr;

    //如果是数组
    if(this->type->flag == 1)
    {
        int arraySize = 1;
        int div = this->type->arrayRangeList.size();
        int range;
        for(int i=0; i<div; i++)
        {
            range = this->type->arrayRangeList[i]->second;
            arraySize *= range;
        }
        _Constant size;
        size.type = "integer";
        size.intValue = arraySize;
        llvm::Value* arraySizeValue = size.codeGen();
        auto arrayType = llvm::ArrayType::get(type, arraySize);
        addr = context.builder.CreateAlloca(arrayType, arraySizeValue, "arraytmp");
    }
    else    //普通变量
    {
        addr = context.builder.CreateAlloca(type);  //在栈上分配空间
    }

    return addr;
}

llvm::Value* _Type::codeGen() {
    
}

llvm::Value* _TypeDef::codeGen(){

}

llvm::Value* _FunctionDefinition::codeGen(_SymbolRecord* funcRec) {
    //获取形参类型
    vector<llvm::Type*> argTypes;
    string type_str;
    //遍历形参列表，获取每个形参对应的LLVM类型
    for(auto it = formalParaList.begin(); it!=formalParaList.end(); it++){
        if(/*是数组*/){
            
        }
        else{
            type_str = it->type;
            argTypes.push_back(context.typeSystem.getllType(type_str));
        }
    }
    
    //获取函数返回值的LLVM类型
    llvm::Type* retType = nullptr;
    if(this->type->first != ""){   //若为函数
        type_str = this->type->second;
    }
    else{   //若为过程，看作返回值类型为void的函数
        type_str = "void";
    }
    retType = context.typeSystem.getllType(type_str); 

    llvm::FunctionType* funcType = llvm::FunctionType::get(retType, argTypes, false);
    llvm::Function* function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, functionID->first, context.module.get());
    
    //函数基本块相关
    auto funcBlock = llvm::BasicBlock::Create(context.llvmContext, "", function);
    context.builder.SetInsertPoint(funcBlock);
    //❓教程中有不同的写法(test2)

    //函数形参相关
    auto formalArg = formalParaList.begin();  //函数定义中的形参列表
    for(auto &llFuncArg: function->args()){     //调用API生成的function的参数列表
        llFuncArg.setName(formalArg->paraId->first);    //将形参的名字一一对应上
        llvm::Value* argAlloc;  //待获取：当前形参对应的LLVM Value*
        if(/*类型为数组*/){

        }
        else{
            //形参的llValue已经在语义分析中获得了，直接查符号表
            int argLoc = mainSymbolTable->idToLoc[formalArg->paraId->first].top();
            argAlloc = mainSymbolTable->recordList[argLoc]->llValue;
        }
        
        context.builder.CreateStore(&llFuncArg, argAlloc, false);
        formalArg++;
    }
    //返回值：在对compound stmt的分析中遇到形如"funcName:="的赋值语句，特判
    if(!funcRec->llValue){   //在对函数体的分析中 未成功获取返回值
        semanticErrorInformation.push_back((string) "line:" + char('0' + funcRec->lineNumber) + "Error: Function ReturnValue not found, " + funcRec->id);
    }
    else{
        context.builder.CreateRet(funcRec->llValue);
    }

    return function;
}

//函数/过程的形参
llvm::Value* _FormalParameter::codeGen() {
    //应该和普通变量的codeGen差不多
    //形参不考虑record类型和数组吗？ASTNode的定义里没有相关字段
}

llvm::Value* _VariantReference::codeGen(){
    
}

llvm::Value* _Expression::codeGen(){

}

llvm::Value* _FunctionCall::codeGen(){
    int loc = mainSymbolTable->idToLoc[this->functionId->first].top();
    llvm::Function* callee = mainSymbolTable->recordList[loc]->llValue;

    if(!callee){
        //报错：函数未找到
        semanticErrorInformation.push_back((string) "line:" + char('0' + functionId->second) + " Error: Function definition not found: " + functionId->first);
    }

    vector<llvm::Value*> argsv;
    //对每一个参数进行codeGen
    //❓是否应该放在SemanticAnalyseFunctionCall中对参数进行分析的部分？
    //(在SemanticAnalyseExpression中)
    for(auto it = actualParaList.begin(); it!= actualParaList.end(); it++){
        argsv.push_back((*it)->codeGen());
        if(!argsv.back())   //若某个参数codeGen失败，立即返回
        {
            //报错：参数解析失败
            pair<string,int> varId = it->variantReference->variantId;
            semanticErrorInformation.push_back((string) "line:" + char('0' + varId->second) + " Error: Function arg codeGen failed: " + varId->first);
            return nullptr;
        }
    }

    return context.builder.CreateCall(callee, argsv, "Calltmp");
}

llvm::Value* _ProcedureCall::codeGen(){
    //重用functionCall的codeGen
    _FunctionCall* procedure = new _FunctionCall;
    procedure->functionId = this->procedureId;
    procedure->actualParaList = this->actualParaList;
    procedure->returnType = "void";

    return procedure->codeGen();
}

llvm::Value* _AssignStatement::codeGen(){
    //获取赋值表达式的左值（LLVM Value*）
    int loc = mainSymbolTable->idToLoc[variantReference->variantId->first].top();
    llvm::Value* lValue = mainSymbolTable->recordList[loc]->llValue;
    //获取右值
    llvm::Value* rValue = this->expression->codeGen();
    
    //若右值和左值的类型不相同，且存在合法转换，则将右值转换为左值的类型
    string lType, rType;
    lType = mainSymbolTable->recordList[loc]->type;
    rType = this->expression->type;
    //若从右值到左值的类型转换合法
    //if(context.typeSystem.isCastLegal(rType, lType)){
    if(rType == "integer" && lType == "real"){
        //进行类型转换

    }
    
    context.builder.CreateStore(rValue, lValue);
    return lValue;
}

llvm::Value* _IfStatement::codeGen(){
    
}

llvm::Value* _ForStatement::codeGen(){

}

llvm::Value* _WhileStatement::codeGen(){
    //应该和_ForStatement类似
}

llvm::Value* _RepeatStatement::codeGen(){
    //应该和_ForStatement类似
}

llvm::Value* _Idvpart::codeGen(){

}