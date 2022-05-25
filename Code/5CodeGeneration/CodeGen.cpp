#include "CodeGen.h"
#define ISTYPE(value, id) (value->getType()->getTypeID() == id)




//static Value* CastToBoolean()...  //用于if-else语句

//初始化中间代码生成相关参数
void CodeGenContext::InitCodeGen(){
    cout << "InitCodeGen" << endl;
    vector<llvm::Type*> SysArgs;
	llvm::FunctionType* PrgmType = llvm::FunctionType:get(llvm::Type::getVoidTy(context.llvmContext),SysArgs,false);
    llvm::Function* Program = llvm::Function::Create(PrgmType, GlobalValue::ExternalLinkage, "Program");
    llvm::BasicBlock* block = llvm::BasicBlock::Create(context.llvmContext, "entry");
}
//逻辑表达式判断值
static Value* CastToBoolean(CodeGenContext& context, Value* condValue){

    if( ISTYPE(condValue, Type::IntegerTyID) ){
        condValue = context.builder.CreateIntCast(condValue, Type::getInt1Ty(context.llvmContext), true);
        return context.builder.CreateICmpNE(condValue, ConstantInt::get(Type::getInt1Ty(context.llvmContext), 0, true));
    }else if( ISTYPE(condValue, Type::DoubleTyID) ){
        return context.builder.CreateFCmpONE(condValue, ConstantFP::get(context.llvmContext, APFloat(0.0)));
    }else{
        return condValue;
    }
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

    _SymbolRecord* record = findSymbolRecord(this->variantId.first);
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
            _SymbolRecord* tmp = findSymbolRecord(record->type);
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
    if(!retType){
        return LogErrorV("[_FunctionDefinition::codeGen]    Undefined function returnType: " + type_str);
    }

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

//❓函数/过程的形参
//重用variant的codeGen
llvm::Value* _FormalParameter::codeGen(int index) {
    cout << "_FormalParameter::codeGen" << endl;

    //形参AST
    // int flag;                 // flag=0表示传值调用，flag=1表示引用调用
    
    _Variant *var = new _Variant;
    var->variantId = this->paraId;
    var->type->type = make_pair(this->type, this->paraId.second);
    var->type->flag = 0;

    return var->codeGen(); //返回形参的地址
}

//函数调用
llvm::Value* _FunctionCall::codeGen(){
    cout << "_FunctionCall::codeGen" << endl;

    _SymbolRecord* funcRec = findSymbolRecord(this->functionId.first);
    if(!funcRec){
        //报错：函数定义未找到
        return LogErrorV("[_FunctionCall::codeGen] Function definition not found: " + this->functionId.first + ", line " + this->functionId.second);
    }

    vector<llvm::Value*> args;
    //对每一个参数进行codeGen
    for(auto it = actualParaList.begin(); it!= actualParaList.end(); it++){
        args.push_back((*it)->codeGen());   //获取实参的值
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
          ret = this->operand1->llvalue;
      }
      else if(this->type=="compound" && this->operation="not"){
		  if(this->operand1->boolValue == "true")
          	ret = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context.llvmContext), 1, true);  
		  else
			  ret = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context.llvmContext), 0, true); 
      }
      else if(this->type=="compound"&& this->operation="minus"){
		  Value* temp;
		  if( this->operand1->llvalue->getType()->getTypeID() == llvm::Type::DoubleTyID) {
			  temp = llvm::ConstantFP::get(context.typeSystem.realTy, 0.0, true);
			  ret = context.builder.CreateFSub(temp, this->operand1->llvalue, "subtmp");
		  }
		  else{
			  temp = llvm::ConstantInt::get(context.typeSystem.intTy, 0, true);
			  ret = context.builder.CreateSub(temp, this->operand1->llvalue, "subtmp");
		  }
      }
      else if(this->type="compound"){
          if(this->expressionType != "error"){
              Value* L = this->operand1->llvalue;
              Value* R = this->operand2->llvalue;
              bool fp = false;

              if( (L->getType()->getTypeID() == llvm::Type::DoubleTyID) || (R->getType()->getTypeID() == llvm::Type::DoubleTyID) ){  // type upgrade
                  fp = true;
                  if( (R->getType()->getTypeID() != llvm::Type::DoubleTyID) ){
                      R = context.builder.CreateUIToFP(R, Type::getDoubleTy(context.llvmContext), "ftmp");
                  }
                  if( (L->getType()->getTypeID() != llvm::Type::DoubleTyID) ){
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
				  ret = fp ? LogErrorV("Double type has no DIV operation") : llvm::ConstantInt::get(llvm::Type::getInt32Ty(context.llvmContext), int(this->operand1->totalIntValue/this->operand2->totalIntValue), true);
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
	  else
		  ret =  LogErrorV("Unknown expression type");
      this->llValue == ret;
      return ret;
}

//赋值语句codeGen
llvm::Value* _AssignStatement::codeGen(string leftType, string rightType){
    cout << "_AssignStatement::codeGen" << endl;

    //获取左值（LLVM Value*）
    //左值类型：普通变量，数组元素，record成员，函数名
    _SymbolRecord* leftVar = findSymbolRecord(variantReference->variantId.first);
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
        //获取数组元素/record成员指针
        llvm::Value* ptr = getItemPtr(this->variantReference);
        //赋值
        if(!ptr){
            return LogErrorV("[_AssignStatement::codeGen]   Cannot get array item ptr, VarName: " + this->variantReference->variantId.first);
        }
        context.builder.CreateStore(rValue, ptr);
    }
    //else if(leftVar->flag == "function"){   //左值为函数名（函数返回语句）
    else if(this->isReturnStatement){
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
      this->then->codeGen();
      thenBB = context.builder.GetInsertBlock();
      if( thenBB->getTerminator() == nullptr ){
          context.builder.CreateBr(mergeBB);
      }
      if( this->els ){
          theFunction->getBasicBlockList().push_back(elsBB);
          context.builder.SetInsertPoint(elsBB);
          this->els->codeGen();
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


      this->_do->codeGen();


      // do increment
      if( this->increment ){
          this->increment->codeGen();
      }

      // execute the again or stop
      condValue = this->condition->codeGen();
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
      llvm::Value* condValue = this->condition->codeGen();
      if( !condValue )
          return nullptr;

      condValue = CastToBoolean(context, condValue);
      // fall to the block
      context.builder.CreateCondBr(condValue, block, after);
      context.builder.SetInsertPoint(block);
      this->_do->codeGen();
      // execute the again or stop
      condValue = this->condition->codeGen();
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
      this->_do->codeGen();
      // execute the again or stop
      condValue = this->condition->codeGen();
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

    _SymbolRecord* varRef = findSymbolRecord(this->variantId.first);
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
        //函数参数只支持基本类型
        //传值参数
        else if (varRef->flag == "value parameter")
        {
            //return context.builder.CreateLoad(addr, false, "");
        }
        //引用参数
        else if (varRef->flag == "var parameter")
        {
            // return context.builder.CreateLoad(addr, false, "");
        }
    }
    else{   //数组元素和record成员
        if(this->IdvpartList.size() == 1){
            return this->IdvpartList[0]->codeGen(this);
        }
    }
}

//数组元素/record成员引用codeGen
llvm::Value* _Idvpart::codeGen(_VariantReference* varRef){
    cout << "_Idvpart::codeGen" << endl;

    llvm::Value* ptr = getItemPtr(varRef);
    if(!ptr){
        return LogErrorV("[_Idvpart::codeGen]   Cannot get item ptr, varName: " + varRef->variantId.first);
    }
    return context.builder.CreateLoad(ptr);
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
    if(!llType){
        return LogErrorV("[InitArrayType]   Unknown array type: " + type);
    }
    llvm::Type* arrayType = llvm::ArrayType::get(llType, arraySize);
    context.typeSystem.addArrayType(arrTypeName, arrayType, type, this->arrayRangeList);

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
        llvm::Type* memType = context.typeSystem.getllType((*it)->type->type.first);
        if(!memType){
            return LogErrorV("[InitRecordType]  Unknown record member type: " + (*it)->type->type.first);
        }
        memberTypes.push_back(memType);
    }
    recordType->setBody(memberTypes);

    return recordType;
}

//获取数组元素的指针
llvm::Value* getArrayItemPtr(string varType, llvm::Value* addr, int loc)
{
    //获取数组对应的LLVM Type
    auto arrayType = context.typeSystem.getllType(varType);
    if(!arrayType){
        return LogErrorV("[getArrayItemPtr]    Unknown array type: " + varType);
    }
    //获取数组元素地址
    auto index_0 = llvm::ConstantInt::get(context.typeSystem.intTy, 0);      //基地址
    auto index_1 = llvm::ConstantInt::get(context.typeSystem.intTy, loc);    //偏移量
    auto ptr = context.builder.CreateGEP(arrayType, addr, {index_0, index_1});

    return ptr;
}

//获取record成员的指针
llvm::Value* getRecordItemPtr(string varType, llvm::Value* addr, string memberId)
{
    // CreateLoad
    auto recPtr = context.builder.CreateLoad(addr, "recPtr");
    recPtr->setAlignment(4); //按4字节对齐

    //获取该成员在record中的位置
    long index = context.typeSystem.getRecordMemberIndex(varType, memberId);
    if(index == -1){
        return LogErrorV("[getRecordItemPtr]    Unknown record member: " + varType + "." + memberId);
    }
    vector<llvm::Value*> indices;
    indices.push_back(llvm::ConstantInt::get(context.typeSystem.intTy, 0));               //基地址
    indices.push_back(llvm::ConstantInt::get(context.typeSystem.intTy, (uint64_t)index)); //偏移量
    auto ptr = context.builder.CreateInBoundsGEP(addr, indices);

    return ptr;
}

//计算N维数组下标（N>=1）
int calcArrayIndex(_SymbolRecord* record, vector<_Expression*> indices){
    vector<pair<int,int>> rangeList(record->arrayRangeList);
    int loc = 0;
    int lower_size = 1;
    for(int i=indices.size()-1; i>=0; i--){
        loc += lower_size * (indices[i]->intNum - rangeList[i].first);
        lower_size *= rangeList[i].second - rangeList[i].first + 1;
    }
    return loc;
}

//获取数组元素/record成员的指针
llvm::Value* getItemPtr(_VariantReference* varRef)
{
    llvm::Value* ptr = nullptr;     //待获取的指针
    //获取符号表记录
    _SymbolRecord* record = findSymbolRecord(varRef->variantId.first);
    //数组各维上下界：record->arrayRangeList(vector<pair<int, int>>)
    //record成员列表：record->records(vector<_SymbolRecord *>)

    //获取嵌套层数
    int layer = varRef->IdvpartList.size();
    
    if(layer == 1){ //直接以当前变量varRef的地址作为基地址
        int flag = varRef->IdvpartList[0]->flag;
        if(flag == 1){  //record成员（a.b）
            ptr = getRecordItemPtr(record->type, record->llValue, varRef->IdvpartList[0]->IdvpartId.first);
            return ptr;
        }
        else{       //纯数组：a[1] 或 a[1][2][...][N]
            //多维纯数组，转化为一维数组处理
            //计算数组元素下标
            int loc = calcArrayIndex(record, varRef->IdvpartList[0]->expressionList;);
            ptr = getArrayItemPtr(record->type, record->llValue, loc);
            return ptr;
        }
    }

    //嵌套的情况
    llvm::Value* cur_base = record->llValue;    //当前层基地址
    string cur_varType = record->type;  //当前层对应的变量类型名
    int flag = 0;   //当前层的类型（flag=0为数组，=1为record）

    for(int i=0; i < layer; i++){
        flag = varRef->IdvpartList[i]->flag;
        if(flag == 0){   //本层为数组下标
            //获取当前层下标表示
            int lower = context.typeSystem.getArrayRange(cur_varType).first;    //当前层数组下界
            int loc = varRef->IdvpartList[i]->expressionList[0]->intNum - lower;
            //计算下一层的基地址
            cur_base = getArrayItemPtr(cur_varType, cur_base, loc);
            //下一层的类型即为当前层数组元素的类型
            cur_varType = context.typeSystem.getArrayMemberType(cur_varType);
        }
        else{       //本层为成员名
            //获取当前层成员名
            string memberId = varRef->IdvpartList[i]->IdvpartId.first;
            //计算下一层的基地址
            cur_base = getRecordItemPtr(cur_varType, cur_base, memberId);
            //下一层的类型即为当前层record成员的类型
            cur_varType = context.typeSystem.getRecordMemberType(cur_varType, memberId);
        }
    }
    
    ptr = cur_base;
    return ptr;
}
