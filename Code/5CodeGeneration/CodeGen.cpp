#include "CodeGen.h"

//static Value* CastToBoolean()...  //用于if-else语句

//初始化中间代码生成相关参数
void CodeGenContext::InitCodeGen(){
    cout << "InitCodeGen" << endl;
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

//常量codeGen
llvm::Value* _Constant::codeGen() {
    cout << "_Constant::codeGen" << endl;
    llvm::Value* ret;
    bool is_minus = this->isMinusShow;

    if(this->type == "integer"){
        int value = is_minus ? (-this->intValue) : this->intValue;
        ret = llvm::ConstantInt::get(context.typeSystem.intTy, value, true);//IsSigned=true
    }
    else if(this->type == "real"){
        float value = is_minus ? (-this->realValue) : this->realValue;
        ret = llvm::ConstantFP::get(context.typeSystem.realTy, value, true);
    }
    else if(this->type == "char"){
        ret = llvm::ConstantInt::get(context.typeSystem.charTy, this->charValue, true);
    }
    else if(this->type == "boolean"){
        ret = llvm::ConstantInt::get(context.typeSystem.boolTy, this->boolValue, true);
    }
    else{
        //报错：未知的常量类型
        ret = LogErrorV("[_Constant::codeGen] Unknown Constant_Type: " + this->type + ", line " + this->valueId.second);
    }

    return ret;
}

//变量声明codeGen
//返回：局部变量地址（CreateAlloca的返回值）
llvm::Value* _Variant::codeGen() {
    cout << "_Variant::codeGen" << endl;

    _SymbolRecord* record = findSymbolRecord(mainSymbolTable, this->variantId.first);
    llvm::Value* addr = nullptr;    //待获取：局部变量地址
    llvm::Type* varType = context.typeSystem.getllType(record->type);   //变量的llvm类型
    //record->type: 普通变量的这个字段记录了基本类型
    //              数组变量和record变量的这个字段都记录了自定义类型名

    //如果是数组
    if(this->type->flag == 1)
    {
        //获取数组对应的LLVM Type
        auto arrayType = varType;
        //若type部分未定义，是在var中直接声明的，则新建数组类型
        if(arrayType == nullptr)
        {
            //要获取数组元素类型，需要查找符号表中数组定义的表项
            _SymbolRecord* tmp = findSymbolRecord(mainSymbolTable, record->type);
            if(!tmp){
                //报错：未定义的数组类型
                return LogErrorV("[_Variant::codeGen]  Undefined array type: " + record->type + ", line " + this->variantId.second);
            }
            string type = tmp->type;
            arrayType = this->type->InitArrayType(record->type, type);
        }
        //在栈上创建一个局部变量
        addr = context.builder.CreateAlloca(arrayType, "arraytmp");
    }

    else if(this->type->type.first == "record")   //如果是record类型
    {
        //获取record对应的LLVM Type
        auto recordType = varType;
        //如果未在type中定义过，而是在var中直接声明，则新建record类型
        if(recordType == nullptr)
        {
            recordType = this->type->InitRecordType(record->type);
        }
        //在栈上创建一个局部变量
        addr = context.builder->CreateAlloca(recordType, nullptr);
    }

    else    //普通变量
    {
        addr = context.builder.CreateAlloca(varType);  //在栈上分配空间
    }

    return addr;
}

//type部分codeGen
void _TypeDef::codeGen(){
    cout << "_TypeDef::codeGen" << endl;

    //数组类型定义
    if(this->type->flag)
    {
        llvm::Type* tmp = this->type->InitArrayType(this->typedefId.first, this->type->type.first);
    }
    //record类型定义
    else if(this->type->type.first == "record")
    {
        llvm::Type* tmp = this->type->InitRecordType(this->typedefId.first);
    }
}

//函数/过程定义
llvm::Value* _FunctionDefinition::codeGen(_SymbolRecord* funcRec) {
    cout << "_FunctionDefinition::codeGen" << endl;

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
    else{   //若为过程，看作返回值类型为integer的函数（返回无意义的integer值0）
        //type_str = "void";
        type_str = "integer";
    }
    retType = context.typeSystem.getllType(type_str); 

    llvm::FunctionType* funcType = llvm::FunctionType::get(retType, argTypes, false);
    llvm::Function* function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, functionID.first, context.module.get());

    //函数基本块相关
    llvm::BasicBlock* funcBlock = llvm::BasicBlock::Create(context.llvmContext, "", function, nullptr);
    context.builder.SetInsertPoint(funcBlock);

    //函数形参相关
    auto formalArg = formalParaList.begin();  //函数定义中的形参列表
    for(auto &it: function->args()){
        it.setName(formalArg->paraId.first);    //将形参的名字一一对应上
        llvm::Value* argAlloc = formalArg->codeGen();   //获取形参的地址
        //将参数store到形参的地址中
        context.builder.CreateStore(&it, argAlloc, false);
        formalArg++;
    }

    if(this->type.first == ""){   //若为过程，设置一个无意义的返回值（integer值0）
        funcRec->funcRetValue = llvm::ConstantInt::get(context.typeSystem.intTy, 0, true);
    }

    //函数返回值：在对assign_stmt的分析中遇到形如"funcName:="的赋值语句，特判
    if(!funcRec->funcRetValue){   //在对函数体的分析中 未成功获取返回值
        //报错：函数返回值未找到
        return LogErrorV("[_FunctionDefinition::codeGen]    Function ReturnValue not found: " + funcRec->id);
    }
    else{
        context.builder.CreateRet(funcRec->funcRetValue);
    }

    return function;    //返回Function::Create的返回值
}

//函数/过程的形参
//重用variant的codeGen
llvm::Value* _FormalParameter::codeGen() {
    cout << "_FormalParameter::codeGen" << endl;

    //形参AST
    // int flag;                 // flag=0表示传值调用，flag=1表示引用调用
    //❓如何实现传值/传引用？

    _Variant* var = new _Variant;
    var->variantId = this->paraId;
    var->type->type = make_pair(this->type, this->paraId.second);
    var->type->flag = 0;

    return var->codeGen();  //返回形参的地址
}

//函数调用
llvm::Value* _FunctionCall::codeGen(){
    cout << "_FunctionCall::codeGen" << endl;

    _SymbolRecord* funcRec = findSymbolRecord(mainSymbolTable, this->functionId.first);
    if(!funcRec){
        //报错：函数定义未找到
        return LogErrorV("[_FunctionCall::codeGen] Function definition not found: " + this->functionId.first + ", line " + this->functionId.second);
    }

    vector<llvm::Value*> args;
    //对每一个参数进行codeGen
    for(auto it = actualParaList.begin(); it!= actualParaList.end(); it++){
        args.push_back((*it)->codeGen());
        if(!args.back())   //若某个参数codeGen失败，立即返回
        {
            //报错：参数解析失败
            return LogErrorV("[_FunctionCall::codeGen]  Para codeGen failed: " + (*it)->variantReference->variantId.first);
        }
    }

    llvm::Function* callee = funcRec->llValue;  //函数指针（Function::Create）
    return context.builder.CreateCall(callee, args, "Calltmp");    //返回：函数返回值
}

//过程调用
void _ProcedureCall::codeGen(){
    cout << "_ProcedureCall::codeGen" << endl;

    //重用functionCall的codeGen
    _FunctionCall* procedure = new _FunctionCall;
    procedure->functionId = this->procedureId;
    procedure->actualParaList = this->actualParaList;
    procedure->returnType = "void";

    procedure->codeGen();    
}

llvm::Value* _Expression::codeGen(){
    cout << "_Expression::codeGen" << endl;

      llvm::Value* ret;
      //常数（字面量）：返回对应的LLVM常量
      if(this->type == "integer"){
          ret = llvm::ConstantInt::get(context.typeSystem.intTy, this->intValue, true);
      }
      else if(this->type == "real"){
          ret = llvm::ConstantFP::get(context.typeSystem.realTy, this->realValue, true);
      }
      else if(this->type == "char"){
          ret = llvm::ConstantInt::get(context.typeSystem.charTy, this->charValue, true);
      }
      else if(this->type == "boolean"){
          ret = llvm::ConstantInt::get(context.typeSystem.boolTy, this->boolValue, true);
      }
      //变量：返回变量的值；const部分声明的常量：返回常量的值
      else if(this->type=="var"){
          ret = this->variantReference->codeGen();
      }
      //函数调用：返回函数的返回值
      else if(this->type=="function"){
          ret = this->functionCall->codeGen();
      }
      else if(this->type=="compound" && this->operation="bracket"){
          ret = this->operand1->codeGen();
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

//赋值语句codeGen
llvm::Value* _AssignStatement::codeGen(string leftType, string rightType){
    cout << "_AssignStatement::codeGen" << endl;

    //获取左值（LLVM Value*）
    //左值类型：普通变量，数组元素，record成员，函数名
    _SymbolRecord* leftVar = findSymbolRecord(mainSymbolTable, variantReference->variantId.first);
    //若左值为数组元素/record成员，leftVar对应的只是数组/record变量，并不具体到成员本身
    llvm::Value* lValue = leftVar->llValue; //左值变量地址（在变量声明时即获得）
    
    //获取右值（LLVM Value*）
    //右值类型：常量，普通变量，数组元素，record成员，函数调用（返回值）
    llvm::Value* rValue = this->expression->codeGen();
    llvm::Value* rightValue = rValue;
    
    //若右值到左值存在合法转换，则将右值转换为左值的类型
    if(leftType == "real" && rightType == "integer")
    {
        auto realValue = context.builder.CreateSIToFP(rightValue, context.typeSystem.realTy);
        rightValue = realValue;
    }

    if(variantReference->IdvpartList.size()!=0){
        if(variantReference->kind == "array"){ //数组元素
            codeGenArrayAssign(leftVar, rightValue);
        }
        else{    //record成员
            string memberId = variantReference->IdvpartList[0]->IdvpartId.first;
            codeGenRecordAssign(leftVar->type, memberId, lValue, rightValue);
        }
    }
    else if(leftVar->flag == "function"){   //左值为函数名（函数返回语句）
        leftVar->funcRetValue = rightValue;     //设置函数的返回值
    }
    else{    //普通变量
        context.builder.CreateStore(rightValue, lValue);
    }

    return lValue;
}

llvm::Value* _IfStatement::codeGen(){
    cout << "_IfStatement::codeGen" << endl;

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
    cout << "_ForStatement::codeGen" << endl;

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
    cout << "_WhileStatement::codeGen" << endl;

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
    cout << "_RepeatStatement::codeGen" << endl;

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

//变量引用codeGen
//处理对象：传值参数、引用参数、普通变量、数组元素、结构体.属性、函数名、在const部分声明的常量
//功能：取出变量的值，并返回寄存器的地址(CreateLoad的返回值)
llvm::Value* _VariantReference::codeGen(){
    cout << "_VariantReference::codeGen" << endl;
    //[refer]   等价于NIdentifier

    _SymbolRecord* varRef = findSymbolRecord(mainSymbolTable, this->variantId.first);
    llvm::Value* addr = varRef->llValue;    //获取普通变量的地址/数组、record的首地址
    if(!addr){
        //报错：未知的变量名
        return LogErrorV("[_VariantReference::codeGen]  Unknown variant name: " + this->variantId.first + ", line " + this->variantId.second);
    }
    if(varRef->flag == "constant"){ //在const部分声明的常量，llValue即为其值
        return addr;
    }
    
    if(this->IdvpartList.size() == 0){
        //普通变量
        if (varRef->flag == "normal variant")
        {
            return context.builder.CreateLoad(addr, false, "");
        }
        //❓传值参数：执行函数/过程后值不变
        else if (varRef->flag == "value parameter")
        {
            // return context.builder.CreateLoad(addr, false, "");
        }
        //❓引用参数：执行函数/过程后值改变
        else if (varRef->flag == "var parameter")
        {
            // return context.builder.CreateLoad(addr, false, "");
        }
    }
    else{   //数组元素和record成员
        //0520：只支持一维数组
        return this->IdvpartList[0]->codeGen(varRef);
    }
}

//数组元素/record成员引用codeGen
llvm::Value* _Idvpart::codeGen(_SymbolRecord* variant){
    cout << "_Idvpart::codeGen" << endl;

    if(variant->flag == "array"){        //如果是数组元素
        //获取数组元素指针
        vector<_Expression*> indices = this->expressionList;
        llvm::Value* ptr = getArrayItemPtr(variant, indices);
        //LOAD
        if(ptr) //如果是多维数组，会返回空指针
            return context.builder.CreateLoad(ptr);
    }

    else if(variant->flag == "normal variant"){   //如果是record成员
        //获取record成员指针
        llvm::Value* ptr = getRecordItemPtr(variant, IdvpartId.first);
        //LOAD
        if(ptr)
            return context.builder.CreateLoad(ptr);
    }
}

//数组元素赋值
void _AssignStatement::codeGenArrayAssign(_SymbolRecord* leftVar, llvm::Value* rValue)
{
    cout << "codeGenArrayAssign" << endl;
    //获取数组元素指针
    vector<_Expression*> indices = this->variantReference->IdvpartList[0]->expressionList;
    llvm::Value* ptr = getArrayItemPtr(leftVar, indices);
    //赋值
    if(ptr) //如果是多维数组，会返回空指针
        context.builder.CreateStore(rValue, ptr);
}

//record成员赋值
void _AssignStatement::codeGenRecordAssign(_SymbolRecord* leftVar, string memberId, llvm::Value* rValue)
{
    cout << "codeGenRecordAssign" << endl;
    //获取record成员指针
    llvm::Value* ptr = getRecordItemPtr(leftVar, memberId);
    //赋值
    if(ptr)
        context.builder.CreateStore(rValue, ptr);
}

//创建数组类型对应的LLVM类型
llvm::Type* _Type::InitArrayType(string arrTypeName, string type)
{
    cout << "InitArrayType" << endl;

    int arraySize = 1;
    int div = this->arrayRangeList.size();
    int range;
    for (int i = 0; i < div; i++) //计算数组总大小
    {
        //第i维大小：上界-下界+1
        range = this->arrayRangeList[i].second - this->arrayRangeList[i].first + 1;
        arraySize *= range;
    }
    llvm::Type* llType = context.typeSystem.getllType(type);
    llvm::Type* arrayType = llvm::ArrayType::get(llType, arraySize);
    context.typeSystem.addArrayType(arrTypeName, arrayType);

    return arrayType;
}

//创建record类型对应的LLVM类型
llvm::Type* _Type::InitRecordType(string recTypeName)
{
    cout << "InitRecordType" << endl;

    llvm::Type* recordType = llvm::StructType::create(context.llvmContext, recTypeName);
    context.typeSystem.addRecordType(recTypeName, recordType);

    vector<llvm::Type *> memberTypes;
    for (auto it = this->recordList.begin(); it != this->recordList.end(); it++)
    {
        context.typeSystem.addRecordMember(recTypeName, (*it)->variantId.first, (*it)->type->type.first);
        memberTypes.push_back(context.typeSystem.getllType((*it)->type->type.first));
    }
    recordType->setBody(memberTypes);

    return recordType;
}

//获取数组元素的指针
llvm::Value* getArrayItemPtr(_SymbolRecord* array, vector<_Expression*> indices)
{
    //获取数组对应的LLVM Type
    string arrayTypeName = array->type;
    auto arrayType = context.typeSystem.getllType(arrayTypeName);
    //获取数组元素地址
    if(indices.size() > 1){   //暂不支持多维数组赋值
        //报错：暂不支持多维数组
        return LogErrorV("[getArrayItemPtr]   multidimensional array is not available now: " + 
            array->id + ", line " + array->lineNumber);
    }
    int loc = indices[0]->intNum;  //数组元素下标
    auto index_0 = llvm::ConstantInt::get(context.typeSystem.intTy, 0);      //基地址
    auto index_1 = llvm::ConstantInt::get(context.typeSystem.intTy, loc);    //偏移量
    auto ptr = context.builder.CreateGEP(arrayType, array->llValue, {index_0, index_1});

    return ptr;
}

//获取record成员的指针
llvm::Value* getRecordItemPtr(_SymbolRecord* record, string memberId)
{
    // CreateLoad
    auto recPtr = context.builder.CreateLoad(record->llValue, "recPtr");
    recPtr->setAlignment(4); //按4字节对齐

    //获取该成员在record中的位置
    long index = context.typeSystem.getRecordMemberIndex(record->type, memberId);
    if(index == -1)
        return nullptr;
    vector<llvm::Value*> indices;
    indices.push_back(llvm::ConstantInt::get(context.typeSystem.intTy, 0));               //基地址
    indices.push_back(llvm::ConstantInt::get(context.typeSystem.intTy, (uint64_t)index)); //偏移量
    auto ptr = context.builder.CreateInBoundsGEP(record->llValue, indices);

    return ptr;
}
