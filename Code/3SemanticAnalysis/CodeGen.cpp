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
//llvm::Value* _Program::codeGen() {}
//主program begin和end之间的程序体
//llvm::Value* _SubProgram::codeGen() {}
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

    else if(this->type->type.first == "record")   //如果是record类型
    {
        //新建record类型
        string recName = this->variantId.first+"_";
        int lineNo = this->variantId.second;

        vector<llvm::Type*> memberTypes;
        auto recordType = llvm::StructType::create(context.llvmContext, recName);
        context.typeSystem.addRecordType(recName, recordType);

        for(auto& member: *this->type->recordList){
            context.typeSystem.addRecordMember(recName, lineNo, member->variantId.first, member->type->type);
            memberTypes.push_back(context.typeSystem.getllType(member->type->type));
        }
        recordType->setBody(memberTypes);

        //新建record变量
        //教程上的写法：module->getTypeByName()
        //auto addr = context.builder->CreateAlloca(context.module->getTypeByName(recName), nullptr);
        auto addr = context.builder->CreateAlloca(recordType, nullptr);

        //对record成员进行codeGen
        vector<_SymbolRecord*> recMembers = this->type->recordList;
        for(int i=0; i<recMembers.size(); i++){
            codeGenRecMember(addr, recName, recMembers[i]);
        }
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
    vector<llvm::Type*> argTypes;
    string type_str;
    //遍历形参列表，获取每个形参对应的LLVM类型（只考虑基本类型）
    for(auto it = formalParaList.begin(); it!=formalParaList.end(); it++){
        type_str = it->type;
        argTypes.push_back(context.typeSystem.getllType(type_str));
    }
    
    //获取函数返回值的LLVM类型
    llvm::Type* retType = nullptr;
    if(this->type->first != ""){   //若为函数
        type_str = this->type->first;
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
        //形参的llValue已经在语义分析中获得了，直接查符号表
        int argLoc = mainSymbolTable->idToLoc[formalArg->paraId->first].top();
        argAlloc = mainSymbolTable->recordList[argLoc]->llValue;
        
        context.builder.CreateStore(&llFuncArg, argAlloc, false);
        formalArg++;
    }
    //返回值：在对assign_stmt的分析中遇到形如"funcName:="的赋值语句，特判
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
    
}

llvm::Value* _VariantReference::codeGen(){
    
}

llvm::Value* _Expression::codeGen(){
      llvm::Value* ret;
      if(this->type == "integer"){
          ret = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context.llvmContext), this->intValue, true);
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
      else if(this->type=="var"){
          //!!!!!!!!!
          int loc = mainSymbolTable->idToLoc[this->variantReference->variantId.first].top();
          ret = mainSymbolTable->recordList[loc]->llValue;
      }
      else if(this->type=="function"){
          string funId = this->functionCall->functionId.first;
          int loc = mainSymbolTable->idToLoc[funId].top();
          ret = mainSymbolTable->recordList[loc]->llValue;
      }
      else if(this->type=="compound" && this->operation="bracket"){
          ret this->operand1->codeGen();
      }
      else if(this->type=="compound" && this->operation="not"){
          ret = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context.llvmContext), !(this->operand1->llValue->getValue()), true);  
      }
      else if(this->type==_expression->operation="minus"){
          ret = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context.llvmContext), -(this->operand1->llValue->getValue()), true);
      }
      else if(this->type="compound"){
          if(this->expressionType != "error"){
              Value* L = this->operand1->codeGen();
              Value* R = this->operand2->codeGen();
              bool fp = false;

              if( (L->getType()->getTypeID() == Type::DoubleTyID) || (R->getType()->getTypeID() == Type::DoubleTyID) ){  // type upgrade
                  fp = true;
                  if( (R->getType()->getTypeID() != Type::DoubleTyID) ){
                      R = context.builder.CreateUIToFP(R, Type::getDoubleTy(context.llvmContext), "ftmp");
                  }
                  if( (L->getType()->getTypeID() != Type::DoubleTyID) ){
                      L = context.builder.CreateUIToFP(L, Type::getDoubleTy(context.llvmContext), "ftmp");
                  }
              }
              if( !L || !R ){
              return nullptr;
              }
              if(this->operation == "+")
                  ret = fp ? context.builder.CreateFAdd(L, R, "addftmp") : context.builder.CreateAdd(L, R, "addtmp");
              else if(this->operation == "-")
                  ret = fp ? context.builder.CreateFSub(L, R, "subftmp") : context.builder.CreateSub(L, R, "subtmp");
              else if(this->operation == "*")
                  ret = fp ? context.builder.CreateFMul(L, R, "mulftmp") : context.builder.CreateMul(L, R, "multmp");
              else if(this->operation == "/")
                  ret = fp ? context.builder.CreateFDiv(L, R, "divftmp") : context.builder.CreateSDiv(L, R, "divtmp");
              else if(this->operation == "and")
                  ret = fp ? LogErrorV("Double type has no AND operation") : context.builder.CreateAnd(L, R, "andtmp");
              else if(this->operation == "or")
                  ret = fp ? LogErrorV("Double type has no OR operation") : context.builder.CreateOr(L, R, "ortmp");
              else if(this->operation == "div"){
                  ret = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context.llvmContext), int(L->getValue()/R->getValue()), true);
              }
              else if(this->operation == "mod")
                  ret = fp ? LogErrorV("Double type has no Mod operation") : context.builder.CreateSRem(L, R, "modtmp");
              else if(this->operation == ">=")
                  ret = fp ? context.builder.CreateFCmpOGE(L, R, "cmpftmp") : context.builder.CreateICmpSGE(L, R, "cmptmp");
              else if(this->operation == ">")
                  ret = fp ? context.builder.CreateFCmpOGT(L, R, "cmpftmp") : context.builder.CreateICmpSGT(L, R, "cmptmp");
              else if(this->operation == "<=")
                  ret = fp ? context.builder.CreateFCmpOLE(L, R, "cmpftmp") : context.builder.CreateICmpSLE(L, R, "cmptmp");    
              else if(this->operation == "<")
                  ret = fp ? context.builder.CreateFCmpOLT(L, R, "cmpftmp") : context.builder.CreateICmpSLT(L, R, "cmptmp");
              else if(this->operation == "<>")
                  ret = fp ? context.builder.CreateFCmpONE(L, R, "cmpftmp") : context.builder.CreateICmpNE(L, R, "cmptmp");
              else if(this->operation == "=")
                  ret = fp ? context.builder.CreateFCmpOEQ(L, R, "cmpftmp") : context.builder.CreateICmpEQ(L, R, "cmptmp");
              else
                  ret = LogErrorV("Unknown binary operator");

          }
      }
      this->llValue == ret;
      return ret;
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
    //若从右值到左值的类型转换合法(integer->real)
    if(lType == "real" && lType != rType){
        //若右值是integer常量
        if(rType == "integer"){
            _Constant tmpReal;
            tmpReal.type = "real";
            tmpReal.realValue = this->expression->realNum;
            rValue = tmpReal.codeGen();
        }
        //若右值是integer变量
        else if(rType == "var"){
            string varId = this->expression->_VariantReference->variantId->first;
            int loc = mainSymbolTable->idToLoc[varId].top();
            string varType = mainSymbolTable->recordList[loc]->type;
            if(varType == "integer")
                rValue = mainSymbolTable->recordList[loc]->llValue;
        }
        //若右值是函数返回值
        else if(rType == "function" && this->expression->functionCall->returnType == "integer"){
            string funcId = this->expression->functionCall->functionId->first;
            int loc = mainSymbolTable->idToLoc[funcId].top();
            rValue = mainSymbolTable->recordList[loc]->llValue;
        }
    }
    
    context.builder.CreateStore(rValue, lValue);
    return lValue;
}

llvm::Value* _IfStatement::codeGen(){
      llvm::Value* condValue = this->condition->codeGen();
      if( !condValue )
          return nullptr;
      condValue = CastToBoolean(context, condValue);
      llvm::Function* theFunction = context.builder.GetInsertBlock()->getParent();//得到if语句所属函数
      llvm::BasicBlock *thenBB = BasicBlock::Create(context.llvmContext, "then", theFunction);
      llvm::BasicBlock *elsBB = BasicBlock::Create(context.llvmContext, "else");//else部分
      llvm::BasicBlock *mergeBB = BasicBlock::Create(context.llvmContext, "ifcont");
      if( this->els ){
        //condvalue为条件判断结果值。如果为true，就进入thenBB分支，如果为false，就进入elsBB分支。
          context.builder.CreateCondBr(condValue, thenBB, elsBB);
      } else{
          context.builder.CreateCondBr(condValue, thenBB, mergeBB);
      }
      //插入指令
      context.builder.SetInsertPoint(thenBB);
      context.pushBlock(thenBB);
      this->then->codeGen();
      context.popBlock();
      thenBB = context.builder.GetInsertBlock();
      if( thenBB->getTerminator() == nullptr ){
          context.builder.CreateBr(mergeBB);
      }
      if( this->els ){
          theFunction->getBasicBlockList().push_back(elsBB);
          context.builder.SetInsertPoint(elsBB);
          context.pushBlock(thenBB);
          this->els->codeGen();
          context.popBlock();
          context.builder.CreateBr(mergeBB);
      }
      theFunction->getBasicBlockList().push_back(mergeBB);
      context.builder.SetInsertPoint(mergeBB);
      return nullptr;
}

llvm::Value* _ForStatement::codeGen(){
      llvm::Function* theFunction = context.builder.GetInsertBlock()->getParent();
      llvm::BasicBlock *block = BasicBlock::Create(context.llvmContext, "forloop", theFunction);
      llvm::BasicBlock *after = BasicBlock::Create(context.llvmContext, "forcont");

      // execute the initial
      if( this->initial )
          this->initial->codeGen(context);

      llvm::Value* condValue = this->condition->codeGen();
      if( !condValue )
          return nullptr;

      condValue = CastToBoolean(context, condValue);

      // fall to the block
      context.builder.CreateCondBr(condValue, block, after);

      context.builder.SetInsertPoint(block);

      context.pushBlock(block);

      this->_do->codeGen(context);

      context.popBlock();

      // do increment
      if( this->increment ){
          this->increment->codeGen(context);
      }

      // execute the again or stop
      condValue = this->condition->codeGen(context);
      condValue = CastToBoolean(context, condValue);
      context.builder.CreateCondBr(condValue, block, after);

      // insert the after block
      theFunction->getBasicBlockList().push_back(after);
      context.builder.SetInsertPoint(after);

      return nullptr;
}

llvm::Value* _WhileStatement::codeGen(){
      //应该和_ForStatement类似
      llvm::Function* theFunction = context.builder.GetInsertBlock()->getParent();
  	llvm::BasicBlock *block = BasicBlock::Create(context, "while_body", sum_fun);
  	llvm::BasicBlock *after = BasicBlock::Create(context, "while_end", sum_fun);
      llvm::Value* condValue = this->condition->codeGen(context);
      if( !condValue )
          return nullptr;

      condValue = CastToBoolean(context, condValue);
      // fall to the block
      context.builder.CreateCondBr(condValue, block, after);
      context.builder.SetInsertPoint(block);
      context.pushBlock(block);
      this->_do->codeGen(context);
      context.popBlock();
      // execute the again or stop
      condValue = this->condition->codeGen(context);
      condValue = CastToBoolean(context, condValue);
      context.builder.CreateCondBr(condValue, block, after);
      // insert the after block
      theFunction->getBasicBlockList().push_back(after);
      context.builder.SetInsertPoint(after)
      return nullptr;
}

llvm::Value* _RepeatStatement::codeGen(){
      //应该和_ForStatement类似
      llvm::Function* theFunction = context.builder.GetInsertBlock()->getParent();
  	llvm::BasicBlock *block = llvm::BasicBlock::Create(context, "while_body", sum_fun);
  	llvm::BasicBlock *after = llvm::BasicBlock::Create(context, "while_end", sum_fun);

      // fall to the block
      context->builder.CreateBr(block);
      context.builder.SetInsertPoint(block);
      context.pushBlock(block);
      this->_do->codeGen(context);
      context.popBlock();
      // execute the again or stop
      condValue = this->condition->codeGen(context);
      condValue = CastToBoolean(context, condValue);
      context.builder.CreateCondBr(condValue, block, after);
      // insert the after block
      theFunction->getBasicBlockList().push_back(after);
      context.builder.SetInsertPoint(after)
      return nullptr;
}

llvm::Value* _Idvpart::codeGen(){

}

llvm::Value* codeGenRecMember(llvm::Value* addr, string recName, _SymbolRecord* member){
    auto recPtr = context.builder.CreateLoad(addr, "recPtr");
    recPtr->setAlignment(4);    //按4字节对齐

    long index = context.typeSystem.getRecordMemberIndex(recName, member->id);
    vector<llvm::Value *> indices;
    indices.push_back(llvm::ConstantInt::get(context.typeSystem.intTy, 0));                     //基地址
    indices.push_back(llvm::ConstantInt::get(context.typeSystem.intTY, (uint64_t)index)); //偏移量
    auto ptr = context.builder.CreateInBoundsGEP(addr, indices, "memberPtr");

    return context.builder.CreateLoad(ptr);
}
