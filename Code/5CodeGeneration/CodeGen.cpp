#include "CodeGen.h"
#include "ObjGen.h"
#include "semanticAnalyse.h"
#define ISTYPE(value, id) (value->getType()->getTypeID() == id)

int expcogen = 1;
extern int expflag;
CodeGenContext context;
vector<llvm::Function*> DLLpt; //库函数指针 1、2、3、4分别指代int real char boolean

bool codeGen_error = false;

//初始化库函数
void CodeGenContext::InitCodeGen()
{
    cout << "InitCodeGen" << endl;

    vector<llvm::Type *> argTypes;
    argTypes.push_back(context.typeSystem.getllType("integer"));
    llvm::Type *retType = context.typeSystem.getllType("void");
    llvm::FunctionType *funcType = llvm::FunctionType::get(retType, argTypes, false);
    llvm::Function *function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, "writeInt", context.Module.get());
    DLLpt.push_back(function);

    argTypes.clear();
    argTypes.push_back(context.typeSystem.getllType("real"));
    retType = context.typeSystem.getllType("void");
    funcType = llvm::FunctionType::get(retType, argTypes, false);
    function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, "writeReal", context.Module.get());
    DLLpt.push_back(function);

    argTypes.clear();
    argTypes.push_back(context.typeSystem.getllType("char"));
    retType = context.typeSystem.getllType("void");
    funcType = llvm::FunctionType::get(retType, argTypes, false);
    function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, "writeChar", context.Module.get());
    DLLpt.push_back(function);

    argTypes.clear();
    argTypes.push_back(context.typeSystem.getllType("boolean"));
    retType = context.typeSystem.getllType("void");
    funcType = llvm::FunctionType::get(retType, argTypes, false);
    function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, "writeBoolean", context.Module.get());
    DLLpt.push_back(function);

    argTypes.clear();
    argTypes.push_back(context.typeSystem.getllType("void"));
    retType = context.typeSystem.getllType("integer");
    funcType = llvm::FunctionType::get(retType, argTypes, false);
    function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, "readInt", context.Module.get());
    DLLpt.push_back(function);

    argTypes.clear();
    argTypes.push_back(context.typeSystem.getllType("void"));
    retType = context.typeSystem.getllType("real");
    funcType = llvm::FunctionType::get(retType, argTypes, false);
    function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, "readReal", context.Module.get());
    DLLpt.push_back(function);

    argTypes.clear();
    argTypes.push_back(context.typeSystem.getllType("void"));
    retType = context.typeSystem.getllType("char");
    funcType = llvm::FunctionType::get(retType, argTypes, false);
    function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, "readChar", context.Module.get());
    DLLpt.push_back(function);

    argTypes.clear();
    argTypes.push_back(context.typeSystem.getllType("void"));
    retType = context.typeSystem.getllType("boolean");
    funcType = llvm::FunctionType::get(retType, argTypes, false);
    function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, "readBoolean", context.Module.get());
    DLLpt.push_back(function);

    llvm::sys::DynamicLibrary::AddSymbol("writeInt", DLLpt[0]);
    llvm::sys::DynamicLibrary::AddSymbol("writeReal", DLLpt[1]);
    llvm::sys::DynamicLibrary::AddSymbol("writeChar", DLLpt[2]);
    llvm::sys::DynamicLibrary::AddSymbol("writeBoolean", DLLpt[3]);
    llvm::sys::DynamicLibrary::AddSymbol("readInt", DLLpt[4]);
    llvm::sys::DynamicLibrary::AddSymbol("readReal", DLLpt[5]);
    llvm::sys::DynamicLibrary::AddSymbol("readChar", DLLpt[6]);
    llvm::sys::DynamicLibrary::AddSymbol("readBoolean", DLLpt[7]);
}

//逻辑表达式判断值
llvm::Value *CastToBoolean(CodeGenContext &context, llvm::Value *condValue)
{
    cout << "CastToBoolean" << endl;

    if (ISTYPE(condValue, llvm::Type::IntegerTyID))
    {
        condValue = context.builder->CreateIntCast(condValue, llvm::Type::getInt1Ty(context.llvmContext), true);
        return context.builder->CreateICmpNE(condValue, llvm::ConstantInt::get(llvm::Type::getInt1Ty(context.llvmContext), 0, true));
    }
    else if (ISTYPE(condValue, llvm::Type::DoubleTyID))
    {
        return context.builder->CreateFCmpONE(condValue, llvm::ConstantFP::get(context.llvmContext, llvm::APFloat(0.0)));
    }
    else
    {
        return condValue;
    }
}

//各种AST节点的CodeGen()----------

llvm::Value *_SubProgram::codeGen()
{
    cout << "_SubProgram::codeGen" << endl;
    //创建program
    llvm::FunctionType *PrgmType = llvm::FunctionType::get(llvm::Type::getVoidTy(context.llvmContext), false);
    llvm::Function *Program = llvm::Function::Create(PrgmType, llvm::GlobalValue::ExternalLinkage, "main", context.Module.get());
    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(context.llvmContext, "", Program);

    if (!context.builder)
    {
        context.builder = make_unique<llvm::IRBuilder<>>(entryBlock);
    }
    else
    {
        context.builder->SetInsertPoint(entryBlock);
    }
    return nullptr;
}

//常量codeGen
llvm::Value *_Constant::codeGen()
{
    cout << "_Constant::codeGen" << endl;

    llvm::Value *ret;
    bool is_minus = this->isMinusShow;

    if (this->type == "integer")
    {
        int value = is_minus ? (-this->intValue) : this->intValue;
        ret = llvm::ConstantInt::get(context.typeSystem.intTy, value, true); // IsSigned=true
    }
    else if (this->type == "real")
    {
        float value = is_minus ? (-this->realValue) : this->realValue;
        ret = llvm::ConstantFP::get(context.typeSystem.realTy, (double)this->realValue);
    }
    else if (this->type == "char")
    {
        ret = llvm::ConstantInt::get(context.typeSystem.charTy, this->charValue, true);
    }
    else if (this->type == "boolean")
    {
        ret = llvm::ConstantInt::get(context.typeSystem.boolTy, this->boolvalue, true);
    }
    else
    {
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
        if(arrayType)
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
        if(!varType){
            return LogErrorV("[_Variant::codeGen]   getllType failed: " + record->type);
        }
        addr = context.builder->CreateAlloca(varType,nullptr);  //在栈上分配空间
    }

    if(!addr){
        return LogErrorV("[_Variant::codeGen]  alloca addr failed: " + this->variantId.first);
    }
    return addr;
}

// type部分codeGen
void _TypeDef::codeGen()
{
    cout << "_TypeDef::codeGen" << endl;

    //数组类型定义
    if (this->type->flag)
    {
        auto tmp = this->type->InitArrayType(this->typedefId.first, this->type->type.first);
    }
    // record类型定义
    else if (this->type->type.first == "record")
    {
        llvm::Type *tmp = this->type->InitRecordType(this->typedefId.first);
    }
}

//函数/过程定义
llvm::Function *_FunctionDefinition::codeGen()
{
    cout << "_FunctionDefinition::codeGen: "<< this->functionID.first << endl;

    vector<llvm::Type *> argTypes;
    string type_str;
    //遍历形参列表，获取每个形参对应的LLVM类型（只考虑基本类型）
    for (int i = 0; i != formalParaList.size(); i++)
    {
        type_str = formalParaList[i]->type;
        argTypes.push_back(context.typeSystem.getllType(type_str));
    }

    //获取函数返回值的LLVM类型
    llvm::Type *retType = nullptr;
    if (this->type.first != "")
    { //若为函数
        type_str = this->type.first;
    }
    //若为过程，看作返回值类型为void的函数
    else
    {
        type_str = "void";
    }
    retType = context.typeSystem.getllType(type_str);
    if (!retType)
    {
        LogErrorV("[_FunctionDefinition::codeGen]    Undefined function returnType: " + type_str);
        return nullptr;
    }
    //创建函数
    llvm::FunctionType *funcType = llvm::FunctionType::get(retType, argTypes, false);
    llvm::Function *function = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, functionID.first, context.Module.get());
    //函数基本块相关
    llvm::BasicBlock *funcBlock = llvm::BasicBlock::Create(context.llvmContext, "", function);
    if (!context.builder)
    {
        context.builder = make_unique<llvm::IRBuilder<>>(funcBlock);
    }
    else
    {
        context.builder->SetInsertPoint(funcBlock);
    }

    //函数形参相关
    _SymbolRecord *argRec = nullptr;
    auto formalArg = formalParaList.begin(); //函数定义中的形参列表
    for (auto &it : function->args())
    {
        it.setName((*formalArg)->paraId.first);          //将形参的名字一一对应上
        llvm::Value *argAlloc = (*formalArg)->codeGen(); //获取形参的地址
        //将参数store到形参的地址中
        context.builder->CreateStore(&it, argAlloc, false);
        //将参数记录到符号表中
        argRec = findSymbolRecord((*formalArg)->paraId.first);
        if (!argRec)
        {
            LogErrorV("[_FunctionDefinition::codeGen]   argRec: Cannot find symbol record: " + (*formalArg)->paraId.first);
            return nullptr;
        }
        argRec->llValue = argAlloc; //记录形参的地址
        formalArg++;
    }

    return function; //返回Function::Create的返回值
}

//构造函数返回指令
void _FunctionDefinition::CreateFuncRet(llvm::Value *funcRetValue)
{
    if (this->type.first == "") //若为过程，返回值类型为void
    {
        context.builder->CreateRet(nullptr);
        return;
    }

    //函数返回值：在对assign_stmt的分析中遇到形如"funcName:="的赋值语句，特判
    if (!funcRetValue)
    { //在对函数体的分析中 未成功获取返回值
        //报错：函数返回值未找到
        LogErrorV("[_FunctionDefinition::CreateFuncRet]    Function ReturnValue not found: " + this->functionID.first);
    }
    else
    {
        context.builder->CreateRet(funcRetValue);
    }
}

//函数/过程的形参(传值)
llvm::Value *_FormalParameter::codeGen()
{
    cout << "_FormalParameter::codeGen" << endl;

    return context.builder->CreateAlloca(context.typeSystem.getllType(this->type), nullptr, this->paraId.first);
}


//函数调用
llvm::Value *_FunctionCall::codeGen()
{
    cout << "_FunctionCall::codeGen: "<< this->functionId.first << endl;

    _SymbolRecord *funcRec = findSymbolRecord(this->functionId.first);
    if (!funcRec)
    {
        //报错：函数定义未找到
        return LogErrorV("[_FunctionCall::codeGen] Cannot find symbol record: " + this->functionId.first + ", line " + itos(this->functionId.second));
    }

    vector<llvm::Value *> args;
    //对每一个参数进行codeGen
    expcogen = 0;
    for (auto it = actualParaList.begin(); it != actualParaList.end(); it++)
    {
        llvm::Value *value = (*it)->codeGen();  //获取实参的值
        if(!value){            //若某个参数codeGen失败，立即返回
            //报错：参数解析失败
            return LogErrorV("[_FunctionCall::codeGen]  Para codeGen failed: " + (*it)->variantReference->variantId.first);
        }
        //实参类型转换
        string formalType = funcRec->findXthFormalParaType(it-actualParaList.begin());
        if(formalType == "real"){
            value = context.builder->CreateSIToFP(value, context.typeSystem.realTy);
        }
        args.push_back(value);
    }
    expcogen = 1;

    llvm::Function *callee = funcRec->functionPtr;               //函数指针（Function::Create）
    return context.builder->CreateCall(callee, args, "Calltmp"); //返回：函数返回值
}

void _ProcedureCall::writecodeGen(int type_arr[])
{
    cout << "_ProcedureCall::writecodeGen" << endl;
    //对每一个参数进行codeGen
    vector<llvm::Value *> args;
    int i = 0;
    
    for (auto it = actualParaList.begin(); it != actualParaList.end(); it++, ++i)
    {
        args.clear();
        if(expcogen == 0)
            args.push_back((*it)->codeGen()); //获取实参的值
        else
            args.push_back((*it)->llvalue);
        if (!args.back())                 //若某个参数codeGen失败，立即返回
        {
            //报错：参数解析失败
            LogErrorV("[_FunctionCall::codeGen]  Para codeGen failed: " + (*it)->variantReference->variantId.first);
            return;
        }
        context.builder->CreateCall(DLLpt[type_arr[i]], args, "Calltmp");
    }
    
    return;
}

void _ProcedureCall::readcodeGen(int type_arr[])
{
    cout << "_ProcedureCall::readcodeGen" << endl;
    llvm::Value *arg;
    vector<llvm::Value *> args;
    int i = 0;
    for (auto it = actualParaList.begin(); it != actualParaList.end(); it++, ++i)
    {
        llvm::Value *ret = context.builder->CreateCall(DLLpt[type_arr[i]], args, "Calltmp");
        _SymbolRecord *leftVar = findSymbolRecord(this->actualParaList[i]->variantReference->variantId.first);
        llvm::Value *addr = nullptr;
        //数组元素/record成员：获取指针
        if(this->actualParaList[i]->variantReference->IdvpartList.size()!=0)
            addr = getItemPtr(this->actualParaList[i]->variantReference);
        else    //普通变量，直接查符号表 
            addr = leftVar->llValue;

        context.builder->CreateStore(ret, addr);
    }
    return;
}

//过程调用
void _ProcedureCall::codeGen()
{
    cout << "_ProcedureCall::codeGen" << endl;

    //复用functionCall的codeGen
    _FunctionCall *procedure = new _FunctionCall;
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
          ret = llvm::ConstantInt::get(context.typeSystem.intTy, this->intNum, true);
      }
      else if(this->type == "real"){
          ret = llvm::ConstantFP::get(context.typeSystem.realTy, (double)this->realNum);
      }
      else if(this->type == "char"){
          ret = llvm::ConstantInt::get(context.typeSystem.charTy, this->charVal, true);
      }
      else if(this->type == "boolean"){
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

          if( this->operand1->llvalue->getType()->getTypeID() == llvm::Type::FloatTyID) {
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
              if( !L || !R ){
			      if(!L)
			      	LogErrorV("Expression L value failed");
			      else
			      	LogErrorV("Expression R value failed");
			  	  return nullptr;
			  }
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
                  //类型转换：将两个operator都转换成浮点型
                  if(ISTYPE(L, llvm::Type::IntegerTyID)){
                      auto realValue = context.builder->CreateSIToFP(L, context.typeSystem.realTy);
                      L = realValue;
                  }
                  if(ISTYPE(R, llvm::Type::IntegerTyID)){
                      auto realValue = context.builder->CreateSIToFP(R, context.typeSystem.realTy);
                      R = realValue;
                  }
                  ret = context.builder->CreateFDiv(L, R, "divftmp");
              }
              else if(this->operation == "and")
                  ret = fp ? LogErrorV("Double type has no AND operation") : context.builder->CreateAnd(L, R, "andtmp");
              else if(this->operation == "or")
                  ret = fp ? LogErrorV("Double type has no OR operation") : context.builder->CreateOr(L, R, "ortmp");
              else if(this->operation == "div")
                  ret = fp ? LogErrorV("Double type has no DIV operation") : context.builder->CreateSDiv(L, R, "divtmp");
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
    cout<<"_Statement::codeGen"<<endl;
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
    else if (this->type == "case"){
        _CaseStatement *casestatement = reinterpret_cast<_CaseStatement *>(this);
        casestatement->codeGen();
    }
	else if (this->type == "assign"){
		_AssignStatement *assignStatement = reinterpret_cast<_AssignStatement *>(this);
		string leftType = SemanticAnalyseVariantReference(assignStatement->variantReference);
        
        if(!expcogen)
            expflag = 0;
		string rightType = SemanticAnalyseExpression(assignStatement->expression);
		expflag = 1;

        if (assignStatement->variantReference->kind == "function return reference"){
            if(leftType != "error" && rightType != "error" && assignStatement->statementType != "error")
            {
                assignStatement->codeGen(leftType, rightType);
                _SymbolRecord* func = findSymbolRecord(assignStatement->variantReference->variantId.first);
                context.builder->CreateRet(func->funcRetValue);
            }
		}
        else if(leftType != "error" && rightType != "error" && assignStatement->statementType != "error")
        {
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
	        if(procedureCall->procedureId.first == "write"){
	            int f = 0;
				int type_arr[procedureCall->actualParaList.size()];
				for (int i = 0; i < procedureCall->actualParaList.size(); i++)
				{
                    if(!expcogen)
                        expflag = 0;
					string actualType = SemanticAnalyseExpression(procedureCall->actualParaList[i]);
					expflag = 1;
					// checked
					if (actualType == "error")
					{
						procedureCall->statementType = "error";
						f = 1;
					}
					if (actualType == "integer")
						type_arr[i] = 0;
					else if (actualType == "real")
						type_arr[i] = 1;
					else if (actualType == "char")
						type_arr[i] = 2;
					else if (actualType == "boolean")
						type_arr[i] = 3;
				}
				if (!f)
					procedureCall->writecodeGen(type_arr);
	        }
	        else if(procedureCall->procedureId.first == "read"){
	            int f = 0;
	            int type_arr[procedureCall->actualParaList.size()];
				for (int i = 0; i < procedureCall->actualParaList.size(); i++)
				{
					if(!expcogen)
                        expflag = 0;
					string actualType = SemanticAnalyseExpression(procedureCall->actualParaList[i]);
					expflag = 1;
					// checked
					if (!(procedureCall->actualParaList[i]->type == "var" && (procedureCall->actualParaList[i]->variantReference->kind == "var" || procedureCall->actualParaList[i]->variantReference->kind == "array")))
						addactualParameterOfReadErrorInformation(procedureCall->actualParaList[i]->lineNo, procedureCall->procedureId.first, i + 1, procedureCall->actualParaList[i]);
					if (actualType == "error")
					{
						procedureCall->statementType = "error";
						f = 1;
					}
					if (actualType == "integer")
						type_arr[i] = 4;
					else if (actualType == "real")
						type_arr[i] = 5;
					else if (actualType == "char")
						type_arr[i] = 6;
					else if (actualType == "boolean")
						type_arr[i] = 7;
				}
	            procedureCall->readcodeGen(type_arr);}
	        else
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

    llvm::Value* rValue;
	if(!expcogen)
	    rValue = this->expression->codeGen();
	else
		rValue = this->expression->llvalue;

    if(!rValue) {
        return LogErrorV("[_AssignStatement::codeGen]   rightValue codeGen failed");
    }
    llvm::Value* rightValue = rValue;

    //获取左值（LLVM Value*）
    //左值类型：普通变量，数组元素，record成员，函数名，形参
    _SymbolRecord* leftVar = findSymbolRecord(variantReference->variantId.first);
    if(!leftVar)
    {
        return LogErrorV("[_AssignStatement::codeGen]   Cannot find symbol record: " + 
            variantReference->variantId.first + ", line " + itos(variantReference->variantId.second));
    }
    //若左值为数组元素/record成员，leftVar对应的只是数组/record变量，并不具体到成员本身
    
    if(this->isReturnStatement){    //函数返回语句
        leftVar->funcRetValue = rightValue;     //设置函数的返回值
        return nullptr;
    }

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

      expcogen = 0;
      //llvm::Value* condValue = this->condition->codeGen();
      llvm::Value* condValue = this->condition->codeGen();

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
      expcogen = 0;
      thenBB = context.builder->GetInsertBlock();
      //在thenBB插入的最后一条指令是ret，则不再跳转回mergeBB
      if( thenBB->getTerminator() == nullptr ){
          context.builder->CreateBr(mergeBB);
      }

      if( this->els ){
          theFunction->getBasicBlockList().push_back(elsBB);
          context.builder->SetInsertPoint(elsBB);
          this->els->codeGen();
          expcogen = 0;
          //在elsBB插入的最后一条指令是ret，则不再跳转回mergeBB
          if(elsBB->getTerminator() == nullptr)
            context.builder->CreateBr(mergeBB);
      }

      theFunction->getBasicBlockList().push_back(mergeBB);
      context.builder->SetInsertPoint(mergeBB);
      expcogen = 1;
      return nullptr;
}

llvm::Value* _CaseStatement::codeGen(){
    expcogen = 0;
    cout << "_CaseStatement::codeGen" << endl;
    llvm::Function* theFunction = context.builder->GetInsertBlock()->getParent();
    vector<llvm::BasicBlock *> cond_vec;
    vector<llvm::BasicBlock *> body_vec;
    int len = 0;
    for(int i = 0;i < this->branch.size();i++)
        for(int j = 0;j < this->branch[i]->condition.size();j++)
            len++;
    for(int i = 0;i < len;i++){
        cond_vec.push_back(llvm::BasicBlock::Create(context.llvmContext, "case_branch", theFunction));
        body_vec.push_back(llvm::BasicBlock::Create(context.llvmContext, "case_body", theFunction));
    }
    llvm::BasicBlock *after = llvm::BasicBlock::Create(context.llvmContext, "casecont");
    context.builder->CreateBr(len == 0 ? after : cond_vec[0]);
    int size = 0;

    for (int i = 0; i < this->branch.size(); i++) {
        for(int j = 0;j < this->branch[i]->condition.size();j++){
             context.builder->SetInsertPoint(cond_vec[size]);
             llvm::Value* condValue = this->branch[i]->condition[j]->codeGen();
             if( !condValue )
                return nullptr;
            condValue = CastToBoolean(context, condValue);
            context.builder->CreateCondBr(condValue, body_vec[size], size == len - 1 ? after : cond_vec[size+1]);
            context.builder->SetInsertPoint(body_vec[size]);
            this->branch[i]->_do->codeGen();
            expcogen = 0;
            context.builder->CreateBr(after);
            size++;
        }
    }
    theFunction->getBasicBlockList().push_back(after);
    context.builder->SetInsertPoint(after);
    expcogen = 1;
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
      expcogen = 0;
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
    expcogen = 0;
    cout << "_WhileStatement::codeGen" << endl;

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
      expcogen = 0;
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
	expcogen = 0;
	    cout << "_RepeatStatement::codeGen" << endl;
    
	    llvm::Function* theFunction = context.builder->GetInsertBlock()->getParent();
	  	llvm::BasicBlock *block = llvm::BasicBlock::Create(context.llvmContext, "repeat_body", theFunction);
	  	llvm::BasicBlock *after = llvm::BasicBlock::Create(context.llvmContext, "repeat_end");
	    context.builder->CreateBr(block);
	    context.builder->SetInsertPoint(block);
	    for(int i = 0; i < this->_do.size(); i++){
	        this->_do[i]->codeGen();
	        expcogen = 0;
        }
	    llvm::Value* condValue = this->condition->codeGen();
	    if(!condValue)
	        return nullptr;
    
	      condValue = CastToBoolean(context, condValue);
	      // fall to the block
	      context.builder->CreateCondBr(condValue, after, block);
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
        if(varRef->flag == "value parameter")
        {
            cout<<"value parameter: "<<varRef->id<<endl;
            return context.builder->CreateLoad(addr, false, "");
        }
        else if(varRef->flag == "var parameter"){
            cout<<"var parameter: "<<varRef->id<<endl;
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
        if(findSymbolRecord(type))
            LogErrorV("[InitArrayType]  Unable to use variant as array_item_type");
        else
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

    //获取该成员在record中的位置
    long index = context.typeSystem.getRecordMemberIndex(varType, memberId);
    if(index == -1){
        return LogErrorV("[getRecordItemPtr]    Unknown record member: " + varType + "." + memberId);
    }

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

    int loc = 0;    //待获取的偏移量
    int lower_size = 1;

    for(int i=IdvpartList.size()-1; i >= 0; i--)
    {
        loc += lower_size* (IdvpartList[i]->expressionList[0]->intNum - rangeList[i].first);
        lower_size *= rangeList[i].second - rangeList[i].first + 1;
    }

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

    llvm::Value* cur_base = record->llValue;    //当前层基地址
    string cur_varType = record->type;  //当前层对应的变量类型名
    int flag = 0;   //当前层的类型（flag=0为数组，=1为record）

    for(int i=0; i < layer;){
        flag = varRef->IdvpartList[i]->flag;

        if(flag == 0){   //本层为数组下标
            //提取出当前数组的所有下标
            _SymbolRecord* arrRec = findSymbolRecord(cur_varType);  //当前数组定义
            int div = arrRec->amount;   //当前数组维数
            vector<_Idvpart*> indices(varRef->IdvpartList.begin()+i, varRef->IdvpartList.begin()+i+div);
            
            int loc = calcArrayIndex(cur_varType, indices);
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

llvm::Value* LogError(const char *str)
{
    fprintf(stderr, "LogError: %s\n", str);
    return nullptr;
}

llvm::Value * LogErrorV(string str)
{
    codeGen_error = true;
    return LogError(str.c_str());
}
