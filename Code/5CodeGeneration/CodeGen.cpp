#include "CodeGen.h"
#include "ObjGen.h" //test
#include "semanticAnalyse.h"
#define ISTYPE(value, id) (value->getType()->getTypeID() == id)
int expcogen = 1;
extern int expflag;
CodeGenContext context;

bool have_error = false;

//初始化中间代码生成相关参数
void CodeGenContext::InitCodeGen(){

    cout << "InitCodeGen" << endl;

    //初始化llvm，设置目标机
    llvm::InitializeNativeTarget();
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    //获取目标三元组并设置
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    context.Module->setTargetTriple(targetTriple);

    //获取目标机数据类型并设置
    string TargetError;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, TargetError);
    if(!target){
        llvm::errs()<<TargetError;
        return;
    }
    auto CPU = "generic";
    auto features = "";

    llvm::TargetOptions options;
    auto relocationModel = llvm::Reloc::Model::PIC_;
    auto theTargetMachine = target->createTargetMachine(targetTriple, CPU, features, options, relocationModel);
    context.Module->setDataLayout(theTargetMachine->createDataLayout());
}

//逻辑表达式判断值
//static llvm::Value* CastToBoolean(CodeGenContext& context, llvm::Value* condValue){
llvm::Value* CastToBoolean(CodeGenContext& context, llvm::Value* condValue){
    cout<<"CastToBoolean"<<endl;

    if( ISTYPE(condValue, llvm::Type::IntegerTyID) ){
        condValue = context.builder->CreateIntCast(condValue, llvm::Type::getInt1Ty(context.llvmContext), true);
        return context.builder->CreateICmpNE(condValue, llvm::ConstantInt::get(llvm::Type::getInt1Ty(context.llvmContext), 0, true));
    }else if( ISTYPE(condValue, llvm::Type::DoubleTyID) ){
        return context.builder->CreateFCmpONE(condValue, llvm::ConstantFP::get(context.llvmContext, llvm::APFloat(0.0)));
    }else{
        return condValue;
    }
}

//各种AST节点的CodeGen()----------

llvm::Value* _SubProgram::codeGen()
{
    cout<<"_SubProgram::codeGen"<<endl;
    //创建program
	llvm::FunctionType* PrgmType = llvm::FunctionType::get(llvm::Type::getVoidTy(context.llvmContext),false);
    //llvm::Function* Program = llvm::Function::Create(PrgmType, llvm::GlobalValue::ExternalLinkage, "Program",context.Module.get());
    llvm::Function* Program = llvm::Function::Create(PrgmType, llvm::GlobalValue::ExternalLinkage, "main",context.Module.get());
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context.llvmContext, "", Program);
    //context.builder = make_unique<llvm::IRBuilder<>>(entryBlock);
    if(!context.builder){
        cout<<"_SubProgram::codeGen: new builder\n";
        context.builder = make_unique<llvm::IRBuilder<>>(entryBlock);
    }
    else{
        cout<<"_SubProgram::codeGen: SetInsertPoint\n";
        context.builder->SetInsertPoint(entryBlock);
    }
    return nullptr;
}

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
        ret = llvm::ConstantFP::get(context.typeSystem.realTy, (double)this->realValue);
    }
    else if(this->type == "char"){
        ret = llvm::ConstantInt::get(context.typeSystem.charTy, this->charValue, true);
    }
    else if(this->type == "boolean"){
        ret = llvm::ConstantInt::get(context.typeSystem.boolTy, this->boolvalue, true);
    }
    else{
        //报错：未知的常量类型
        ret = LogErrorV("[_Constant::codeGen] Unknown Constant_Type: " + this->type + ", line " + itos(this->valueId.second));
    }

    return ret;
}

//变量声明codeGen
//返回：局部变量地址（CreateAlloca的返回值）
llvm::Value* _Variant::codeGen() {
    cout << "_Variant::codeGen" << endl;
    
    _SymbolRecord* record = findSymbolRecord(this->variantId.first);
    if(!record){
        return LogErrorV("[_Variant::codeGen]   Cannot find symbol record: " + this->variantId.first + ", line " + itos(this->variantId.second));
    }
    llvm::Value* addr = nullptr;    //待获取：局部变量地址
    llvm::Type* varType = context.typeSystem.getllType(record->type);   //变量的llvm类型
    if(!varType){
        return LogErrorV("[_Variant::codeGen]   getllType failed: " + record->type);
    }
    else cout<<"varType str: "<<record->type<<", varName: "<<this->variantId.first<<endl;
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
                return LogErrorV("[_Variant::codeGen]  Cannot find symbol record: " + record->type + ", line " + itos(this->variantId.second));
            }
            string type = tmp->type;
            arrayType = this->type->InitArrayType(record->type, type);
        }
        //在栈上创建一个局部变量
        //addr = context.builder->CreateAlloca(arrayType, "arraytmp");
        addr = context.builder->CreateAlloca(arrayType);
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
        addr = context.builder->CreateAlloca(varType,nullptr);  //在栈上分配空间
    }

    if(!addr){
        return LogErrorV("[_Variant::codeGen]  alloca addr failed: " + this->variantId.first);
    }
    return addr;
}

//type部分codeGen
void _TypeDef::codeGen(){
    cout << "_TypeDef::codeGen" << endl;

    //数组类型定义
    if(this->type->flag)
    {
        auto tmp = this->type->InitArrayType(this->typedefId.first, this->type->type.first);
    }
    //record类型定义
    else if(this->type->type.first == "record")
    {
        llvm::Type* tmp = this->type->InitRecordType(this->typedefId.first);
    }
}

//write的codeGen
llvm::Value* GenSysWrite(vector<llvm::Value*> &args_list, bool new_line) {
    llvm::Function *llvm_printf = nullptr;
    if (llvm_printf == nullptr) {
        //register printf
        vector<llvm::Type *> arg_types = {llvm::Type::getInt8PtrTy(context.llvmContext)};
        llvm::FunctionType *func_type = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context.llvmContext),
            arg_types,
            true
        );
        llvm::Function *func = llvm::Function::Create(
            func_type,
            llvm::Function::ExternalLinkage,
            "printf",
            &*(context.Module)
        );
        func->setCallingConv(llvm::CallingConv::C);
        llvm_printf = func;
    }
    std::string format;
    std::vector<llvm::Value *> printf_args;
    printf_args.emplace_back(nullptr);
    for (auto arg: args_list) {
        auto *tp = arg->getType();
        if (tp->isIntegerTy()) {
            format += "%d";
            printf_args.emplace_back(arg);
        }
        else if (tp->isFloatingPointTy()) {
            format += "%f";
            printf_args.emplace_back(arg);
        }
        else if (tp->isStructTy()) {
            format += "%c";
            printf_args.emplace_back(arg);
        } else {
            cerr << "[write] Unsupported type" << std::endl;
            return nullptr;
        }
    }
    if (new_line) {
        format += "\n";
    }
    printf_args[0] = context.builder->CreateGlobalStringPtr(format, "printf_format");
    return context.builder->CreateCall(llvm_printf, printf_args, "call_printf");
}

//函数/过程定义
llvm::Function* _FunctionDefinition::codeGen() 
{
    cout << "_FunctionDefinition::codeGen" << endl;

    vector<llvm::Type*> argTypes;
    string type_str;
    //遍历形参列表，获取每个形参对应的LLVM类型（只考虑基本类型）
    for(int i=0; i!=formalParaList.size(); i++){
        type_str = formalParaList[i]->type;
        argTypes.push_back(context.typeSystem.getllType(type_str));
    }
    
    //获取函数返回值的LLVM类型
    llvm::Type* retType = nullptr;
    if(this->type.first != ""){   //若为函数
        type_str = this->type.first;
    }
    //else{   //若为过程，看作返回值类型为integer的函数（返回无意义的integer值0）
    else{
        type_str = "void";
        //type_str = "integer";
    }
    retType = context.typeSystem.getllType(type_str); 
    if(!retType){
        LogErrorV("[_FunctionDefinition::codeGen]    Undefined function returnType: " + type_str);
        return nullptr;
    }
    //创建函数
    llvm::FunctionType* funcType = llvm::FunctionType::get(retType, argTypes, false);
    llvm::Function* function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, functionID.first, context.Module.get());
    //函数基本块相关
    llvm::BasicBlock* funcBlock = llvm::BasicBlock::Create(context.llvmContext, "", function);
    if(!context.builder){
        cout<<"[_FunctionDefinition::codeGen]   new builder, block: "<<functionID.first<<endl;
        context.builder = make_unique<llvm::IRBuilder<>>(funcBlock);
    }
    else{
        cout<<"[_FunctionDefinition::codeGen]   SetInsertPoint, block: "<<functionID.first<<endl;
        context.builder->SetInsertPoint(funcBlock);
    }

    //函数形参相关
    _SymbolRecord* argRec = nullptr;
    auto formalArg = formalParaList.begin();  //函数定义中的形参列表
    for(auto &it: function->args()){
        it.setName((*formalArg)->paraId.first);    //将形参的名字一一对应上
        llvm::Value* argAlloc = (*formalArg)->codeGen();   //获取形参的地址
        //将参数store到形参的地址中
        context.builder->CreateStore(&it, argAlloc, false);   
        //将参数记录到符号表中
        argRec = findSymbolRecord((*formalArg)->paraId.first);
        if(!argRec){
            LogErrorV("[_FunctionDefinition::codeGen]   argRec: Cannot find symbol record: " + (*formalArg)->paraId.first);
            return nullptr;
        }
        argRec->llValue = argAlloc; //记录形参的地址
        formalArg++;
    }
    
    return function;    //返回Function::Create的返回值
}

//构造函数返回指令
void _FunctionDefinition::CreateFuncRet(llvm::Value* funcRetValue)
{
    // if(this->type.first == ""){   //若为过程，设置一个无意义的返回值（integer值0）
    //     funcRetValue = llvm::ConstantInt::get(context.typeSystem.intTy, 0, true);
    // }
    if(this->type.first == ""){
        context.builder->CreateRet(nullptr);
        return;
    }

    //函数返回值：在对assign_stmt的分析中遇到形如"funcName:="的赋值语句，特判
    if(!funcRetValue){   //在对函数体的分析中 未成功获取返回值
        //报错：函数返回值未找到
        LogErrorV("[_FunctionDefinition::CreateFuncRet]    Function ReturnValue not found: " + this->functionID.first);
    }
    else{
        context.builder->CreateRet(funcRetValue);
    }
}

//函数/过程的形参(传值)
llvm::Value* _FormalParameter::codeGen() {
    cout << "_FormalParameter::codeGen" << endl;
    
    return context.builder->CreateAlloca(context.typeSystem.getllType(this->type),nullptr, this->paraId.first);
}


//函数调用
llvm::Value* _FunctionCall::codeGen(){
    cout << "_FunctionCall::codeGen" << endl;

    _SymbolRecord* funcRec = findSymbolRecord(this->functionId.first);
    if(!funcRec){
        //报错：函数定义未找到
        return LogErrorV("[_FunctionCall::codeGen] Cannot find symbol record: " + this->functionId.first + ", line " + itos(this->functionId.second));
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

    if(this->functionId.first == "write")
    {
        return GenSysWrite(args,1);
    }
    
    llvm::Function* callee = funcRec->functionPtr;  //函数指针（Function::Create）
    return context.builder->CreateCall(callee, args, "Calltmp");    //返回：函数返回值
}

//过程调用
void _ProcedureCall::codeGen(){
    cout << "_ProcedureCall::codeGen" << endl;

    //复用functionCall的codeGen
    _FunctionCall* procedure = new _FunctionCall;
    procedure->functionId = this->procedureId;
    procedure->actualParaList = this->actualParaList;
    procedure->returnType = "void";

    procedure->codeGen();    
}

llvm::Value* _Expression::codeGen(){
    cout << "_Expression::codeGen" << endl;

    cout<<"expr type: "<<this->type<<endl;
      llvm::Value* ret;
      //常数（字面量）：返回对应的LLVM常量
      if(this->type == "integer"){
          ret = llvm::ConstantInt::get(context.typeSystem.intTy, this->intNum, true);
      }
      else if(this->type == "real"){
          ret = llvm::ConstantFP::get(context.typeSystem.realTy, (double)this->realNum);
      }
      else if(this->type == "char"){
          ret = llvm::ConstantInt::get(context.typeSystem.charTy, this->charVal, true);
      }
      else if(this->type == "boolean"){
          //ret = llvm::ConstantInt::get(context.typeSystem.boolTy, this->boolValue == "true"? 1 : 0, true);
          ret = llvm::ConstantInt::get(context.typeSystem.boolTy, this->boolValue == "t"? 1 : 0, true);
      }
      //变量：返回变量的值；const部分声明的常量：返回常量的值
      else if(this->type=="var"){
          ret = this->variantReference->codeGen();
      }
      //函数调用：返回函数的返回值
      else if(this->type=="function"){
          ret = this->functionCall->codeGen();
      }
      else if(this->type=="compound" && this->operation=="bracket"){
	  	if(!expcogen)
            this->operand1->llvalue = this->operand1->codeGen();
          
            ret = this->operand1->llvalue;

      }
      else if(this->type=="compound" && this->operation=="not"){
        if(!expcogen)
            this->operand1->llvalue = this->operand1->codeGen();
          ret = context.builder->CreateNot(this->operand1->llvalue,"not");
      }
        else if(this->type=="compound" && this->operation=="minus"){
          llvm::Value* temp;
		  if(expcogen == 0)
            this->operand1->llvalue = this->operand1->codeGen();

		  if( this->operand1->llvalue->getType()->getTypeID() == llvm::Type::DoubleTyID) {
			  temp = llvm::ConstantFP::get(context.typeSystem.realTy, (double)0.0);
			  ret = context.builder->CreateFSub(temp, this->operand1->llvalue, "subtmp");
		  }
		  else{
			  temp = llvm::ConstantInt::get(context.typeSystem.intTy, 0, true);
			  ret = context.builder->CreateSub(temp, this->operand1->llvalue, "subtmp");
		  }
      }
      else if(this->type=="compound"){
          if(this->expressionType != "error"){
              llvm::Value* L = expcogen ? this->operand1->llvalue : this->operand1->codeGen();
              llvm::Value* R = expcogen ? this->operand2->llvalue : this->operand2->codeGen();
              bool fp = false;

              if( (L->getType()->getTypeID() == llvm::Type::FloatTyID) || (R->getType()->getTypeID() == llvm::Type::FloatTyID) ){  // type upgrade
                  fp = true;
                  if( (R->getType()->getTypeID() != llvm::Type::FloatTyID) ){
                      R = context.builder->CreateUIToFP(R, llvm::Type::getFloatTy(context.llvmContext), "ftmp");
                  }
                  if( (L->getType()->getTypeID() != llvm::Type::FloatTyID) ){
                      L = context.builder->CreateUIToFP(L, llvm::Type::getFloatTy(context.llvmContext), "ftmp");
                  }
              }
              if( !L || !R ){
              return nullptr;
              }
              if(this->operation == "+")
                  ret = fp ? context.builder->CreateFAdd(L, R, "addftmp") : context.builder->CreateAdd(L, R, "addtmp");
              else if(this->operation == "-")
                  ret = fp ? context.builder->CreateFSub(L, R, "subftmp") : context.builder->CreateSub(L, R, "subtmp");
              else if(this->operation == "*")
                  ret = fp ? context.builder->CreateFMul(L, R, "mulftmp") : context.builder->CreateMul(L, R, "multmp");
              else if(this->operation == "/"){
                  ret = fp ? context.builder->CreateFDiv(L, R, "divftmp") : context.builder->CreateSDiv(L, R, "divtmp");
              }
              else if(this->operation == "and")
                  ret = fp ? LogErrorV("Double type has no AND operation") : context.builder->CreateAnd(L, R, "andtmp");
              else if(this->operation == "or")
                  ret = fp ? LogErrorV("Double type has no OR operation") : context.builder->CreateOr(L, R, "ortmp");
              else if(this->operation == "div"){
                  //test
                  //cout<<"op1="<<this->operand1->totalIntValue<<", op2="<<this->operand2->totalIntValue<<endl;
                  //op1和op2的totalIntValue都为0，如下，totalIntValueValid标志位为0，表示该值未启用
                  //cout<<"totalIntValue valid="<<this->operand1->totalIntValueValid<<endl;
                    //❓如何做整除？
                  ret = fp ? LogErrorV("Double type has no DIV operation") : llvm::ConstantInt::get(
                      llvm::Type::getInt32Ty(context.llvmContext), 
                      int(this->operand1->totalIntValue/this->operand2->totalIntValue), true);
              
              }
              else if(this->operation == "mod")
                  ret = fp ? LogErrorV("Double type has no Mod operation") : context.builder->CreateSRem(L, R, "modtmp");
              else if(this->operation == ">=")
                  ret = fp ? context.builder->CreateFCmpOGE(L, R, "cmpftmp") : context.builder->CreateICmpSGE(L, R, "cmptmp");
              else if(this->operation == ">")
                  ret = fp ? context.builder->CreateFCmpOGT(L, R, "cmpftmp") : context.builder->CreateICmpSGT(L, R, "cmptmp");
              else if(this->operation == "<=")
                  ret = fp ? context.builder->CreateFCmpOLE(L, R, "cmpftmp") : context.builder->CreateICmpSLE(L, R, "cmptmp");    
              else if(this->operation == "<")
                  ret = fp ? context.builder->CreateFCmpOLT(L, R, "cmpftmp") : context.builder->CreateICmpSLT(L, R, "cmptmp");
              else if(this->operation == "<>")
                  ret = fp ? context.builder->CreateFCmpONE(L, R, "cmpftmp") : context.builder->CreateICmpNE(L, R, "cmptmp");
              else if(this->operation == "=")
                  ret = fp ? context.builder->CreateFCmpOEQ(L, R, "cmpftmp") : context.builder->CreateICmpEQ(L, R, "cmptmp");
              else
                  ret = LogErrorV("Unknown binary operator");

          }
      }
      else
        ret = LogErrorV("Unknown expression type");
      this->llvalue == ret;
      return ret;
}
//语句codegen
llvm::Value* _Statement::codeGen(){
    if(this->type == "repeat"){
        _RepeatStatement *repeatStatement = reinterpret_cast<_RepeatStatement *>(this);
        repeatStatement->codeGen();
    }
    else if(this->type =="while"){
        _WhileStatement *whileStatement = reinterpret_cast<_WhileStatement *>(this);
        whileStatement->codeGen();
    }
    else if (this->type == "for"){
        _ForStatement *forStatement = reinterpret_cast<_ForStatement *>(this);
        forStatement->codeGen();
    }
    else if (this->type == "if"){
        _IfStatement *ifStatement = reinterpret_cast<_IfStatement *>(this);
        ifStatement->codeGen();
    }
	else if (this->type == "assign"){
		_AssignStatement *assignStatement = reinterpret_cast<_AssignStatement *>(this);
		string leftType = SemanticAnalyseVariantReference(assignStatement->variantReference);
		string rightType = SemanticAnalyseExpression(assignStatement->expression);
		if (assignStatement->variantReference->kind == "function return reference"){
			if(leftType != "error" && rightType != "error" && assignStatement->statementType != "error")
				assignStatement->codeGen(leftType, rightType);
		}
		if(leftType != "error" && rightType != "error" && assignStatement->statementType != "error"){
			_SymbolRecord* leftVar = findSymbolRecord(assignStatement->variantReference->variantId.first);
            if(!leftVar){
                return LogErrorV("[_Statement::codeGen] Cannot find symbol record: " + assignStatement->variantReference->variantId.first + 
                    ", line " + itos(assignStatement->variantReference->variantId.second));
            }
			leftVar->llValue = assignStatement->codeGen(leftType, rightType);	//返回左值的llValue
		}
    }
    else if (this->type == "procedure"){
		_ProcedureCall *procedureCall = reinterpret_cast<_ProcedureCall *>(this);
        procedureCall->codeGen();
    }
    else{
        return LogErrorV("[_Statement::codeGen] Unknown stmt type: " + this->type);
    }
    return nullptr;
}
//赋值语句codeGen
llvm::Value* _AssignStatement::codeGen(string leftType, string rightType){
    cout << "_AssignStatement::codeGen "<< this->lineNo << endl;

    //获取右值（LLVM Value*）
    //右值类型：常量，普通变量，数组元素，record成员，函数调用（返回值）
    //llvm::Value* rValue = this->expression->codeGen();
    llvm::Value* rValue = this->expression->llvalue;

    if(!rValue) {
        return LogErrorV("[_AssignStatement::codeGen]   rightValue codeGen failed");
    }
    llvm::Value* rightValue = rValue;

    //获取左值（LLVM Value*）
    //左值类型：普通变量，数组元素，record成员，函数名，形参
    _SymbolRecord* leftVar = findSymbolRecord(variantReference->variantId.first);
    if(leftVar){ cout<< "assign_leftVal: " << leftVar->id<<endl; }
    else{
        return LogErrorV("[_AssignStatement::codeGen]   Cannot find symbol record: " + 
            variantReference->variantId.first + ", line " + itos(variantReference->variantId.second));
    }
    //若左值为数组元素/record成员，leftVar对应的只是数组/record变量，并不具体到成员本身
    
    if(this->isReturnStatement){    //函数返回语句
        leftVar->funcRetValue = rightValue;     //设置函数的返回值
        return nullptr;
    }
    // if(leftVar->flag == "value parameter"){  //左值为函数参数（传值）
    //     leftVar->llValue = rightValue;  //暂不支持类型转换
    //     return nullptr;
    // }

    llvm::Value* lValue = leftVar->llValue; //左值变量地址（在变量声明时即获得）
    if(!lValue){
        return LogErrorV("[_AssignStatement::codeGen]   Cannot get addr of leftValue");
    }
    
    //若右值到左值存在合法转换，则将右值转换为左值的类型
    if(leftType == "real" && rightType == "integer")
    {
        auto realValue = context.builder->CreateSIToFP(rightValue, context.typeSystem.realTy);
        rightValue = realValue;
    }

    if(variantReference->IdvpartList.size()!=0){
        //获取数组元素/record成员指针
        llvm::Value* ptr = getItemPtr(this->variantReference);
        //赋值
        if(!ptr){
            return LogErrorV("[_AssignStatement::codeGen]   Cannot get array item ptr, VarName: " + this->variantReference->variantId.first);
        }
        context.builder->CreateStore(rightValue, ptr);
    }
    else{    //普通变量
        context.builder->CreateStore(rightValue, lValue);
    }

    return lValue;
}

llvm::Value* _IfStatement::codeGen(){
    cout << "_IfStatement::codeGen" << endl;

      //llvm::Value* condValue = this->condition->codeGen();
      llvm::Value* condValue = this->condition->llvalue;

      if( !condValue )
          return nullptr;
      condValue = CastToBoolean(context, condValue);
      llvm::Function* theFunction = context.builder->GetInsertBlock()->getParent();//得到if语句所属函数
      llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(context.llvmContext, "then", theFunction);
      llvm::BasicBlock *elsBB = llvm::BasicBlock::Create(context.llvmContext, "else");//else部分
      llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(context.llvmContext, "ifcont");
      if( this->els ){
        //condvalue为条件判断结果值。如果为true，就进入thenBB分支，如果为false，就进入elsBB分支。
          context.builder->CreateCondBr(condValue, thenBB, elsBB);
      } else{
          context.builder->CreateCondBr(condValue, thenBB, mergeBB);
      }
      //插入指令
      context.builder->SetInsertPoint(thenBB);
      this->then->codeGen();
      thenBB = context.builder->GetInsertBlock();
      if( thenBB->getTerminator() == nullptr ){
          context.builder->CreateBr(mergeBB);
      }
      if( this->els ){
          theFunction->getBasicBlockList().push_back(elsBB);
          context.builder->SetInsertPoint(elsBB);
          this->els->codeGen();
          context.builder->CreateBr(mergeBB);
      }
      theFunction->getBasicBlockList().push_back(mergeBB);
      context.builder->SetInsertPoint(mergeBB);
      return nullptr;
}

llvm::Value* _ForStatement::codeGen(){
	expcogen = 0;
    cout << "_ForStatement::codeGen" << endl;

      llvm::Function* theFunction = context.builder->GetInsertBlock()->getParent();
      llvm::BasicBlock *block = llvm::BasicBlock::Create(context.llvmContext, "forloop", theFunction);
      llvm::BasicBlock *after = llvm::BasicBlock::Create(context.llvmContext, "forcont");

      // execute the initial
      if( this->initial )
          this->initial->codeGen("integer", "integer");  //initial:_AssignStatement*

      llvm::Value* condValue = this->condition->codeGen();
      if( !condValue )
          return nullptr;

      condValue = CastToBoolean(context, condValue);

      // fall to the block
      context.builder->CreateCondBr(condValue, block, after);

      context.builder->SetInsertPoint(block);

      this->_do->codeGen();

      // do increment
      if( this->increment ){
          this->increment->codeGen("integer", "integer");
      }

      // execute the again or stop
      condValue = this->condition->codeGen();
      condValue = CastToBoolean(context, condValue);
      context.builder->CreateCondBr(condValue, block, after);

      // insert the after block
      theFunction->getBasicBlockList().push_back(after);
      context.builder->SetInsertPoint(after);
		expcogen = 1;
      return nullptr;
}

llvm::Value* _WhileStatement::codeGen(){
    cout << "_WhileStatement::codeGen" << endl;
	expcogen = 0;
      llvm::Function* theFunction = context.builder->GetInsertBlock()->getParent();
  	  llvm::BasicBlock *block = llvm::BasicBlock::Create(context.llvmContext, "while_body", theFunction);
  	  llvm::BasicBlock *after = llvm::BasicBlock::Create(context.llvmContext, "while_end");
      llvm::Value* condValue = this->condition->codeGen();
      if( !condValue )
          return nullptr;

      condValue = CastToBoolean(context, condValue);
      // fall to the block
      context.builder->CreateCondBr(condValue, block, after);
      context.builder->SetInsertPoint(block);
      this->_do->codeGen();
      // execute the again or stop
      condValue = this->condition->codeGen();
      condValue = CastToBoolean(context, condValue);
      context.builder->CreateCondBr(condValue, block, after);
      // insert the after block
      theFunction->getBasicBlockList().push_back(after);
      context.builder->SetInsertPoint(after);
	expcogen = 1;
      return nullptr;
}

llvm::Value* _RepeatStatement::codeGen(){
    cout << "_RepeatStatement::codeGen" << endl;
	expcogen = 0;
    for(int i = 0; i < this->_do.size(); i++)
        this->_do[i]->codeGen();
    llvm::Function* theFunction = context.builder->GetInsertBlock()->getParent();
  	llvm::BasicBlock *block = llvm::BasicBlock::Create(context.llvmContext, "repeat_body", theFunction);
  	llvm::BasicBlock *after = llvm::BasicBlock::Create(context.llvmContext, "repeat_end");

    llvm::Value* condValue = this->condition->codeGen();
    if(!condValue)
        return nullptr;
    
      condValue = CastToBoolean(context, condValue);
      // fall to the block
      context.builder->CreateCondBr(condValue, after, block);
      context.builder->SetInsertPoint(block);
      for(int i = 0; i < this->_do.size(); i++)
        this->_do[i]->codeGen();
      // execute the again or stop
      condValue = this->condition->codeGen();
      condValue = CastToBoolean(context, condValue);
      context.builder->CreateCondBr(condValue, after, block);
      // insert the after block
      theFunction->getBasicBlockList().push_back(after);
      context.builder->SetInsertPoint(after);
	expcogen = 1;
      return nullptr;

}

//变量引用codeGen
//处理对象：传值参数、引用参数、普通变量、数组元素、结构体.属性、函数名、在const部分声明的常量
//功能：取出变量的值，并返回寄存器的地址(CreateLoad的返回值)
llvm::Value* _VariantReference::codeGen(){
    cout << "_VariantReference::codeGen" << endl;

    _SymbolRecord* varRef = findSymbolRecord(this->variantId.first);
    if(!varRef){
        //报错：未知的变量名
        return LogErrorV("[_VariantReference::codeGen]  Cannot find symbol record: " + 
            this->variantId.first + ", line " + itos(this->variantId.second));
    }
    llvm::Value* addr = varRef->llValue;    //获取普通变量的地址/数组、record的首地址
    if(varRef->flag == "constant"){ //在const部分声明的常量，llValue即为其值
        return addr;
    }
    
    if(this->IdvpartList.size() == 0){
        //普通变量
        if (varRef->flag == "normal variant")
        {
            return context.builder->CreateLoad(addr, false, "");
        }
        if(varRef->flag == "value parameter"){
            return context.builder->CreateLoad(addr, false, "");
        }
    }
    else{   //数组元素和record成员
        if(this->IdvpartList.size() > 0){
            return this->IdvpartList[0]->codeGen(this);
        }
    }
    return nullptr;
}

//数组元素/record成员引用codeGen
llvm::Value* _Idvpart::codeGen(_VariantReference* varRef){
    cout << "_Idvpart::codeGen" << endl;

    llvm::Value* ptr = getItemPtr(varRef);
    if(!ptr){
        return LogErrorV("[_Idvpart::codeGen]   Cannot get item ptr, varName: " + varRef->variantId.first);
    }
    return context.builder->CreateLoad(ptr);
}

//创建数组类型对应的LLVM类型
llvm::Type* _Type::InitArrayType(string arrTypeName, string type)
//llvm::ArrayType* _Type::InitArrayType(string arrTypeName, string type)
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
        LogErrorV("[InitArrayType]   Unknown array type: " + type);
        return nullptr;
    }
    auto arrayType = llvm::ArrayType::get(llType, arraySize);
    context.typeSystem.addArrayType(arrTypeName, arrayType, type, this->arrayRangeList);

    return arrayType;
}

//创建record类型对应的LLVM类型
llvm::Type* _Type::InitRecordType(string recTypeName)
{
    cout << "InitRecordType" << endl;

    llvm::StructType* recordType = llvm::StructType::create(context.llvmContext, recTypeName);
    context.typeSystem.addRecordType(recTypeName, recordType);

    vector<llvm::Type *> memberTypes;
    for (auto it = this->recordList.begin(); it != this->recordList.end(); it++)
    {
        context.typeSystem.addRecordMember(recTypeName, (*it)->variantId.first, (*it)->type->type.first);
        llvm::Type* memType = context.typeSystem.getllType((*it)->type->type.first);
        if(!memType){
            LogErrorV("[InitRecordType]  Unknown record member type: " + (*it)->type->type.first);
            return nullptr;
        }
        memberTypes.push_back(memType);
    }
    recordType->setBody(memberTypes);
    

    return recordType;
}

//获取数组元素的指针
llvm::Value* getArrayItemPtr(string varType, llvm::Value* addr, int loc)
{
    cout<<"getArrayItemPtr"<<endl;
    //获取数组对应的LLVM Type
    auto arrayType = context.typeSystem.getllType(varType);
    if(!arrayType){
        return LogErrorV("[getArrayItemPtr]    Unknown array type: " + varType);
    }

    //获取数组元素地址
    auto index_0 = llvm::ConstantInt::get(context.typeSystem.intTy, 0);      //基地址
    auto index_1 = llvm::ConstantInt::get(context.typeSystem.intTy, loc);    //偏移量
    llvm::Value* ptr = nullptr;
    ptr = context.builder->CreateGEP(arrayType, addr, {index_0, index_1});
    if(!ptr){
        return LogErrorV("[getArrayItemPtr] getArrayItemPtr failed: " + varType);
    }
    return ptr;
}

//获取record成员的指针
llvm::Value* getRecordItemPtr(string varType, llvm::Value* addr, string memberId)
{
    cout<<"getRecordItemPtr"<<endl;
    // CreateLoad
    //auto recPtr = context.builder->CreateLoad(addr, "recPtr");
    //recPtr->setAlignment(4); //按4字节对齐
    //❓cannot convert 'int' to 'llvm::MaybeAlign'

    //获取该成员在record中的位置
    long index = context.typeSystem.getRecordMemberIndex(varType, memberId);
    if(index == -1){
        return LogErrorV("[getRecordItemPtr]    Unknown record member: " + varType + "." + memberId);
    }
    cout<<"record member " << varType + "." + memberId<< " 's index is: "<< index<<"\n\n";

    vector<llvm::Value*> indices;
    indices.push_back(llvm::ConstantInt::get(context.typeSystem.intTy, 0));               //基地址
    indices.push_back(llvm::ConstantInt::get(context.typeSystem.intTy, (uint64_t)index)); //偏移量
    auto ptr = context.builder->CreateInBoundsGEP(addr, indices);
    if(!ptr){
        return LogErrorV("[getRecordItemPtr]    getRecordItemPtr failed: " + varType);
    }
    return ptr;
}

//计算N维数组下标（N>=1，按C标准，从0开始）
//如果查符号表失败，则返回-1
int calcArrayIndex(string arrTypeName, vector<_Idvpart*> IdvpartList)
{
    cout<<"calcArrayIndex"<<endl;

    //vector<pair<int,int>> rangeList(record->arrayRangeList);
    //变量的record只记录了数组类型名，首先需要查表得到数组定义
    _SymbolRecord* arrRec = findSymbolRecord(arrTypeName);
    if(!arrRec){
        LogErrorV("[calcArrayIndex] Cannot find symbol record: " + arrTypeName);
        return -1;
    }
    vector<pair<int,int>> rangeList(arrRec->arrayRangeList);
    if(rangeList.size() == 0){
        LogErrorV("[calcArrayIndex] get arrayRangeList failed: " + arrTypeName);
        return -1;
    }

    // cout<<"rangeList:"<<endl;
    // for(int i=0; i<rangeList.size(); i++)
    //     cout<<rangeList[i].first<<" "<<rangeList[i].second<<endl;

    int loc = 0;    //待获取的偏移量
    int lower_size = 1;

    for(int i=IdvpartList.size()-1; i >= 0; i--)
    {
        loc += lower_size* (IdvpartList[i]->expressionList[0]->intNum - rangeList[i].first);
        lower_size *= rangeList[i].second - rangeList[i].first + 1;
    }

    cout<<"array item index(from 0): "<<loc<<"\n\n";
    return loc;
}

//获取数组元素/record成员的指针
llvm::Value* getItemPtr(_VariantReference* varRef)
{
    cout<<"getItemPtr"<<endl;
    llvm::Value* ptr = nullptr;     //待获取的指针
    //获取符号表记录
    _SymbolRecord* record = findSymbolRecord(varRef->variantId.first);
    if(!record){
        return LogErrorV("[getItemPtr]  Cannot find symbol record: " + varRef->variantId.first + 
            ", line " + itos(varRef->variantId.second));
    }
    //数组各维上下界：record->arrayRangeList(vector<pair<int, int>>)
    //record成员列表：record->records(vector<_SymbolRecord *>)

    //获取嵌套层数
    int layer = varRef->IdvpartList.size();
    cout<<"array/record layer = "<<layer<<endl;

    llvm::Value* cur_base = record->llValue;    //当前层基地址
    string cur_varType = record->type;  //当前层对应的变量类型名
    int flag = 0;   //当前层的类型（flag=0为数组，=1为record）

    for(int i=0; i < layer;){
        flag = varRef->IdvpartList[i]->flag;
        cout<<"cur_varType = "<<cur_varType<<", flag = " << flag << endl;

        if(flag == 0){   //本层为数组下标
            //提取出当前数组的所有下标
            _SymbolRecord* arrRec = findSymbolRecord(cur_varType);  //当前数组定义
            int div = arrRec->amount;   //当前数组维数
            vector<_Idvpart*> indices(varRef->IdvpartList.begin()+i, varRef->IdvpartList.begin()+i+div);
            // cout<<"indices of array "<<cur_varType<<" is:"<<endl;
            // for(int i=0; i<indices.size(); i++){
            //     cout<<indices[i]->expressionList[0]->intNum<<endl;
            // }
            int loc = calcArrayIndex(cur_varType, indices);
            cout<<"cur_layer_index(from 0) : "<<loc<<endl;
            //计算下一层的基地址
            cur_base = getArrayItemPtr(cur_varType, cur_base, loc);
            //下一层的类型即为当前层数组元素的类型
            cur_varType = context.typeSystem.getArrayMemberType(cur_varType);
            
            i += div;   //跳过当前已经处理完的数组层数
        }
        else{       //本层为成员名
            //获取当前层成员名
            string memberId = varRef->IdvpartList[i]->IdvpartId.first;
            //计算下一层的基地址
            cur_base = getRecordItemPtr(cur_varType, cur_base, memberId);
            //下一层的类型即为当前层record成员的类型
            cur_varType = context.typeSystem.getRecordMemberType(cur_varType, memberId);

            i++;
        }
    }
    
    ptr = cur_base;
    return ptr;
}

//unique_ptr<_Expression> LogError(const char *str)
llvm::Value* LogError(const char *str)
{
    fprintf(stderr, "LogError: %s\n", str);
    return nullptr;
}

llvm::Value * LogErrorV(string str)
{
    have_error = true;
    return LogError(str.c_str());
}
