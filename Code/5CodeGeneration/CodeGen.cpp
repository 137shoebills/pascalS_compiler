#include "CodeGen.h"

//static Value* CastToBoolean()...  //用于if-else语句

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

//变量声明codeGen
//返回：局部变量地址（CreateAlloca的返回值）
llvm::Value* _Variant::codeGen() {
    string varType = this->type->type.first;
    llvm::Type* lltype = context.typeSystem.getllType(varType);
    llvm::Value* addr = nullptr;

    //如果是数组
    if(this->type->flag == 1)
    {
        int arraySize = 1;
        int div = this->type->arrayRangeList.size();
        int range;
        for(int i=0; i<div; i++)
        {
            range = this->type->arrayRangeList[i].second - this->type->arrayRangeList[i].first + 1;
            arraySize *= range;
        }
        _Constant size;
        size.type = "integer";
        size.intValue = arraySize;
        llvm::Value* arraySizeValue = size.codeGen();
        auto arrayType = llvm::ArrayType::get(lltype, arraySize);
        context.typeSystem.addArrayType(this->variantId.first, arrayType);  //以数组变量名作为索引
        //在栈上创建局部变量arraytmp
        addr = context.builder.CreateAlloca(arrayType, arraySizeValue, "arraytmp");

        /*[refer]   NArrayInitialization::codeGen
        for(int index = 0; index < div; index++){
            // shared_ptr<NInteger> indexValue = make_shared<NInteger>(index);
            // shared_ptr<NArrayIndex> arrayIndex = make_shared<NArrayIndex>(this->declaration->id, indexValue);
            // NArrayAssignment assignment(arrayIndex, this->expressionList->at(index));
            // assignment.codeGen(context);
        }
        */
    }

    else if(this->type->recordList.size()!=0)   //如果是record类型
    {
        int loc = mainSymbolTable->custom[this->variantId.first].top();
        string recName = mainSymbolTable->recordList[loc]->type;
        int lineNo = this->variantId.second;
        auto recordType = nullptr;

        //获取record对应的LLVM Type
        //已在type中定义过：直接查表
        recordType = context.typeSystem.getllType(recName);
        if(recordType == nullptr){
           //在var中直接定义+声明：新建record类型
            recordType = llvm::StructType::create(context.llvmContext, recName);
            context.typeSystem.addRecordType(recName, recordType);
            vector<llvm::Type *> memberTypes;
            for (auto it = this->type->recordList.begin(); it != this->type->recordList.end(); it++)
            {
                context.typeSystem.addRecordMember(recName, lineNo, (*it)->variantId.first, (*it)->type->type);
                memberTypes.push_back(context.typeSystem.getllType((*it)->type->type));
            }
            recordType->setBody(memberTypes);
        }

        //在栈上分配一个record变量
        addr = context.builder->CreateAlloca(recordType, nullptr);
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
    if(this->type.first != ""){   //若为函数
        type_str = this->type.first;
    }
    else{   //若为过程，看作返回值类型为void的函数
        type_str = "void";
    }
    retType = context.typeSystem.getllType(type_str); 

    llvm::FunctionType* funcType = llvm::FunctionType::get(retType, argTypes, false);
    llvm::Function* function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, functionID.first, context.module.get());
    
    //函数基本块相关
    auto funcBlock = llvm::BasicBlock::Create(context.llvmContext, "", function);
    context.builder.SetInsertPoint(funcBlock);
    //❓教程中有不同的写法(test2)

    //函数形参相关
    auto formalArg = formalParaList.begin();  //函数定义中的形参列表
    for(auto &it: function->args()){
        it.setName(formalArg->paraId.first);    //将形参的名字一一对应上
        llvm::Value* argAlloc;  //待获取：当前形参对应的LLVM Value*
        //argAlloc = formalArg->codeGen();
        //形参的llValue已经在语义分析中获得了，直接查符号表
        int argLoc = mainSymbolTable->idToLoc[formalArg->paraId.first].top();
        argAlloc = mainSymbolTable->recordList[argLoc]->llValue;
        
        context.builder.CreateStore(&it, argAlloc, false);
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
    //❓可以重用variant的吗？
}

llvm::Value* _VariantReference::codeGen(){

}

llvm::Value* _Expression::codeGen(){
      llvm::Value* ret;
      //常量：返回对应的LLVM常量
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
      //变量：返回地址
      else if(this->type=="var"){
          int loc = mainSymbolTable->idToLoc[this->variantReference->variantId.first].top();
          ret = mainSymbolTable->recordList[loc]->llValue;
        //   string type = mainSymbolTable->recordList[loc]->type;
        //   ret = context.typeSystem.getllType(type);
      }
      //❓函数调用：返回CreateCall的返回值
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
    int loc = mainSymbolTable->idToLoc[this->functionId.first].top();
    llvm::Function* callee = mainSymbolTable->recordList[loc]->llValue;

    if(!callee){
        //报错：函数未找到
        semanticErrorInformation.push_back((string) "line:" + char('0' + functionId.second) + " Error: Function definition not found: " + functionId.first);
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
            semanticErrorInformation.push_back((string) "line:" + char('0' + varId.second) + " Error: Function arg codeGen failed: " + varId.first);
            return nullptr;
        }
    }

    return context.builder.CreateCall(callee, argsv, "Calltmp");
}

llvm::Value* _ProcedureCall::codeGen(){
    //重用functionCall的codeGen
    _FunctionCall* procedure = new _FunctionCall();
    procedure->functionId = this->procedureId;
    procedure->actualParaList = this->actualParaList;
    procedure->returnType = "void";

    return procedure->codeGen();
}

llvm::Value* _AssignStatement::codeGen(){
    //获取左值（LLVM Value*）
    //左值类型：普通变量，数组元素，record成员，函数名
    int loc = mainSymbolTable->idToLoc[variantReference->variantId.first].top();
    _SymbolRecord* leftVar = mainSymbolTable->recordList[loc];
    llvm::Value* lValue = leftVar->llValue;
    //若左值为数组元素/record成员，leftVar对应的只是数组/record变量，并不具体到成员本身
    
    //获取右值（LLVM Value*）
    //右值类型：常量，普通变量，数组元素，record成员，函数调用（返回值）
    llvm::Value* rValue = this->expression->codeGen();

    //获取左值、右值的LLVM Type
    llvm::Type *lType = nullptr, *rType = nullptr;
    lType = context.typeSystem.getVarllType(variantReference);

    map<string, int> types;
    types["integer"] = types["real"] = types["char"] = types["boolean"] = 1;
    types["var"] = 2;
    types["function"] = 3;
    types["compound"] = 4;

    switch(types[this->expression->type]){
        case 1:   //右值为常量
            rType = context.typeSystem.getllType(this->expression->type);
            break;
        case 2:     //右值为变量
            rType = context.typeSystem.getVarllType(this->expression->variantReference);
            break;
        case 3:    //右值为函数调用
            if(this->expression->functionCall->returnType != "error")
                rType = context.typeSystem.getllType(this->expression->functionCall->returnType);
            break;
        case 4:    //右值为复杂表达式
            //TODO
            //rType = rValue->getType();
            break;
    }
    
    //若右值到左值存在合法转换，则将右值转换为左值的类型
    if(lType == context.typeSystem.realTy && rType == context.typeSystem.intTy){
        auto realValue = context.builder.CreateSIToFP(rValue, context.typeSystem.realTy);
        rValue = realValue;
    }

    if(variantReference->IdvpartList.size()!=0){
        if(variantReference->IdvpartList[0]->flag == 0){ //数组元素
            codeGenArrayAssign(leftVar->id, lValue, rValue);
        }
        else{    //record成员
            string memberId = variantReference->IdvpartList[0]->IdvpartId.first;
            codeGenRecordAssign(leftVar->type, memberId, lValue, rValue);
        }
    }
    else{    //普通变量
        context.builder.CreateStore(rValue, lValue);
    }

    return lValue;    //如果是函数返回语句，把lValue填到函数的llValue里
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

llvm::Value* _Idvpart::codeGen(_SymbolRecord* variant){
    //varId: _VariantReference节点的对应字段，表示变量名
    /*
class _VariantReference
    pair<string, int> variantId; //变量或常量标识符和行号
    //如果这个变量是结构体或数组：
    vector<_Idvpart *> IdvpartList; //结构体.属性或数组元素
    
class _Idvpart
    vector<_Expression *> expressionList; // flag = 0;
    pair<string, int> IdvpartId;          // flag = 1
    int flag;  
    */

    if(variant->flag == "array"){        //如果是数组元素

    }

    else if(variant->flag == "normal variant"){   //如果是record成员
        //取对应record类型的llValue
        string recName = variant->type;     //record类型的名称
        int loc = mainSymbolTable->custom[recName].top();
        auto varPtr = mainSymbolTable->recordList[loc]->llValue;  //获取CreateAlloca的返回值(局部变量的指针)
        //CreateLoad
        auto recPtr = context.builder.CreateLoad(varPtr, "recPtr");
        recPtr->setAlignment(4);    //按4字节对齐

        //获取该成员在record中的位置
        long index = context.typeSystem.getRecordMemberIndex(recName, IdvpartId.first);
        vector<llvm::Value *> indices;
        indices.push_back(llvm::ConstantInt::get(context.typeSystem.intTy, 0));               //基地址
        indices.push_back(llvm::ConstantInt::get(context.typeSystem.intTY, (uint64_t)index)); //偏移量
        auto ptr = context.builder.CreateInBoundsGEP(varPtr, indices, "memberPtr");

        return context.builder.CreateLoad(ptr);
    }
}

//数组元素赋值
void _AssignStatement::codeGenArrayAssign(string arrayName, llvm::Value* lValue, llvm::Value* rValue)
{   //lValue是数组的首地址

    //CreateLoad
    //arrayPtr = context.builder.CreateLoad(lValue, "arrayPtr");
    //arrayPtr->setAlignment(16);

/***************按照TinyCompiler的写法****************/
    //获取数组元素地址
    //[refer]   auto index = calcArrayIndex(arrayIndex, context);
    // auto index = calcArrayIndex();  //该函数未写
    // llvm::ArrayRef<llvm::Value*> gep2array{
    //     llvm::ConstantInt::get(context.typeSystem.getllType("int64"), 0), index};
    // auto ptr = context.builder.CreateInBoundsGEP(lValue, gep2array, "elementPtr");

/***************按照教程的写法***************/
    //获取数组对应的LLVM Type
    auto arrayType = context.typeSystem.getllType(arrayName);
    //获取数组元素地址
    vector<pair<int, int>> indices = this->variantReference->IdvpartList[0]->expressionList;
    if(indices.size()>1){   //暂不支持多维数组赋值
        cout<< "[codeGenArrayAssign]    multidimensional array is not available now." << endl;
        return;
    }
    int loc = indices[0].intNum;  //数组元素下标
    auto index_0 = llvm::ConstantInt::get(context.typeSystem.intTy, 0);      //基地址
    auto index_1 = llvm::ConstantInt::get(context.typeSystem.intTy, loc);    //偏移量
    auto ptr = context.builder.CreateGEP(arrayType, lValue, {index_0, index_1});

    //赋值
    context.builder.CreateStore(rValue, ptr);
}

//record成员赋值
void _AssignStatement::codeGenRecordAssign(string recName, string memberId, llvm::Value* lValue, llvm::Value* rValue)
{
    // CreateLoad
    //auto recPtr = context.builder.CreateLoad(lValue, "recPtr");
    //recPtr->setAlignment(4); //按4字节对齐

    //获取该成员在record中的位置
    long index = context.typeSystem.getRecordMemberIndex(recName, memberId);
    vector<llvm::Value*> indices;
    indices.push_back(llvm::ConstantInt::get(context.typeSystem.intTy, 0));               //基地址
    indices.push_back(llvm::ConstantInt::get(context.typeSystem.intTy, (uint64_t)index)); //偏移量
    auto ptr = context.builder.CreateInBoundsGEP(lValue, indices, "RecMemberPtr");
    
    //赋值
    context.builder.CreateStore(rValue, ptr);
}
