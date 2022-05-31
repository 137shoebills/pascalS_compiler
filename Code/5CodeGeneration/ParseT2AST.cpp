

/*
普通语法分析树到抽象语法树的转换
*/
#include "ParseT2AST.h"

void printstatement(_Statement *statement){
    if(statement != NULL){
        cout<<"~~type: "<<statement->type<<endl;
        cout<<"statementType: "<<statement->statementType<<endl;
        cout<<"lineNo: "<<statement->lineNo<<endl;
        cout<<"isReturn: "<<statement->isReturnStatement<<endl;
        if(statement->type == "compound"){
            _Compound *_compound = new _Compound;
            _compound = (_Compound*)statement;
            if(_compound->statementList.size()){
            cout<<"compound statement:"<<endl;
            for(int i =0 ;i < _compound->statementList.size();i++){
                printstatement(_compound->statementList[i]);
            }}
        }
        else if(statement->type == "assign"){
            _AssignStatement *_assignStatement = new _AssignStatement;
            _assignStatement =(_AssignStatement * )statement;
            cout<<"~~variantId:"<<_assignStatement->variantReference->variantId.first<<" "<<_assignStatement->variantReference->variantId.second<<endl;
            if(_assignStatement->variantReference->IdvpartList.size())
            printidvpart(_assignStatement->variantReference->IdvpartList);
            printExp(_assignStatement->expression);
        }
        else if(statement->type == "procedure"){
            _ProcedureCall *_procedureCall = new _ProcedureCall;
            _procedureCall = (_ProcedureCall*)statement;
            cout<<"procedureIdL: "<<_procedureCall->procedureId.first<<" "<<_procedureCall->procedureId.second<<endl;
            if(_procedureCall->actualParaList.size()){
                cout<<"actualParalist"<<endl;
                for(int i = 0;i < _procedureCall->actualParaList.size();i++)
                    printExp(_procedureCall->actualParaList[i]);
            }
        }
        else if(statement->type == "if"){
            _IfStatement* _ifStatement = new _IfStatement;
            _ifStatement = (_IfStatement*)statement;
            cout<<"condition: "<<endl;
            printExp(_ifStatement->condition) ;
            cout<<"then: "<<endl;
            printstatement(_ifStatement->then);
            cout<<"else: "<<endl;
            printstatement(_ifStatement->els) ;
        }
        else if(statement->type == "for"){
            _ForStatement *_forStatement = new _ForStatement;
            _forStatement = (_ForStatement *)statement;
            cout<<"relopId: ";
            cout<<_forStatement->id.first<<" "<<_forStatement->id.second<<endl;
            cout<<"START: "<<endl;
            printExp(_forStatement->start);
            cout<<"END: "<<endl;
            printExp(_forStatement->end);
            cout<<"Do: "<<endl;
            printstatement(_forStatement->_do);
        }
        else if (statement->type == "while"){
            _WhileStatement *_whileStatement = new _WhileStatement;
            _whileStatement = (_WhileStatement *)statement;
            cout<<"condition: "<<endl;
            printExp(_whileStatement->condition);
            cout<<"Do: "<<endl;
            printstatement(_whileStatement->_do);
        }
        else if(statement->type == "repeat"){
            _RepeatStatement *_repeatStatement = new _RepeatStatement;
            _repeatStatement = (_RepeatStatement*)statement;
            cout<<"condition: "<<endl;
            printExp(_repeatStatement->condition);
            if(_repeatStatement->_do.size()){
                cout<<"Do: "<<endl;
                for(int i = 0;i < _repeatStatement->_do.size();i++)
                printstatement(_repeatStatement->_do[i]);}
        }
		else if(statement->type == "case")
	           {
                   cout<<"!"<<endl;
                   cout<<"caseAST"<<endl;
	               _CaseStatement *caseStatement = new _CaseStatement;
	               caseStatement = (_CaseStatement *)statement;
	               cout << "CaseID:" << endl;
	               printExp(caseStatement->caseid);
	               cout << "caseBranchSize:" << caseStatement->branch.size() << endl;
	               for (int i = 0; i < caseStatement->branch.size(); ++i)
	               {
	                   for (int j = 0; j < caseStatement->branch[i]->condition.size();++j)
	                   {
	                       cout << "   condition " << i << ": ";
	                       cout << caseStatement->branch[i]->condition[j]->boolValue << endl;
	                   }
	                   cout << "       Do:" << endl;
	                   printstatement(caseStatement->branch[i]->_do);
	               }
	           }
			   
    }
}
void printfuncall(_FunctionCall* functionCall){
    cout<<"~~functionId: "<<functionCall->functionId.first<<" "<<functionCall->functionId.second<<endl;
    if(functionCall->actualParaList.size()){
        cout<<"actualParaList: "<<endl;
        for(int i = 0;i < functionCall->actualParaList.size();i++){
            printExp(functionCall->actualParaList[i]);
        }
    }
}
void printExp(_Expression* expression){
    
    cout<<"~~type: "<<expression->type<<endl;
    cout<<"isMInux: "<<expression->isMinusShow<<endl;
    cout<<"lineNo: "<<expression->lineNo<<endl;
    if(expression->type == "compound"){
        cout<<"operation: "<<expression->operation<<endl;
        cout<<"operationtype: "<<expression->operationType<<endl;
        cout<<"operand1: "<<endl;
        printExp(expression->operand1);
        cout<<"operand2: "<<endl;
        printExp(expression->operand2);
    }
    else if(expression->type == "integer"){
        cout<<"strofNum: "<<expression->strOfNum<<endl;
        cout<<"intnum: "<<expression->intNum<<endl;
    }
    else if(expression->type == "real"){
        cout<<"strofNum: "<<expression->strOfNum<<endl;
        cout<<"intnum: "<<expression->realNum<<endl;
    }
    else if (expression->type == "var"){
        if(expression->variantReference != NULL){
            cout<<"variantReference: "<<endl;
            cout<<"~~variantId:"<<expression->variantReference->variantId.first<<" "<<expression->variantReference->variantId.second<<endl;
            if(expression->variantReference->IdvpartList.size())
            printidvpart(expression->variantReference->IdvpartList);
        }
        
    }
    else if(expression->type == "function"){
        printfuncall(expression->functionCall);
    }
    else if(expression->type == "letter"){
        cout<<"charval: "<<expression->charVal<<endl;
    }
    
}
//void printvariantReference(vector<_VariantReference*> variantReference){
//    for(int i = 0;i < variantReference.size();i++){
//        cout<<"~~variantId: "<<variantReference[i]->variantId.first<<" "<<variantReference[i]->variantId.second<<endl;
//        cout<<"falg: "<<variantReference[i]->flag<<endl;
//        if(variantReference[i]->IdvpartList.size()){
//            cout<<"IdvpartList: "<<endl;
//            for(int j = 0;j < variantReference[i]->IdvpartList.size();j++){
//                if(variantReference[i]->IdvpartList[j]->flag == 0 && variantReference[i]->IdvpartList[j]->expressionList.size())
//                    for(int k = 0;k < variantReference[i]->IdvpartList[j]->expressionList.size();k++)
//                        printExp(variantReference[i]->IdvpartList[j]->expressionList[k]);
//                else if(variantReference[i]->IdvpartList[j]->flag == 1){
//                    cout<<variantReference[i]->IdvpartList[j]->IdvpartId.first<<" "<<variantReference[i]->IdvpartList[j]->IdvpartId.second<<endl;
//                }
//            }
//
//
//        }
//        cout<<"str: "<<variantReference[i]->str<<endl;
//    }
//}
void printidvpart(vector<_Idvpart*> idvpartlist){
    for(int i = 0;i < idvpartlist.size();i++){
        if(idvpartlist[i]->flag == 0 && idvpartlist[i]->expressionList.size())
            for(int j = 0;j <idvpartlist[i]->expressionList.size();j++)
                printExp(idvpartlist[i]->expressionList[j]);
        else if(idvpartlist[i]->flag == 1)
            cout<<"~~IdvpartId: "<<idvpartlist[i]->IdvpartId.first<<" "<<idvpartlist[i]->IdvpartId.second<<endl;
    }
}
void dfscompound(_Compound *compound){
    cout<<"~~compound lineno: "<<compound->lineNo<<endl;
    if(compound->statementList.size()){
    cout<<"compound statement:"<<endl;
    for(int i =0 ;i < compound->statementList.size();i++){
        printstatement(compound->statementList[i]);
    }}
}
void dfssubprogramDefinitionList(vector<_FunctionDefinition*> subprogramDefinitionList){
    for(int i= 0;i <subprogramDefinitionList.size();i++){
        cout<<"~~programId:"<<subprogramDefinitionList[i]->functionID.first<<" "<<subprogramDefinitionList[i]->functionID.second<<endl;
        
        cout<<"formalparaList: ";
        for(int j = 0;j < subprogramDefinitionList[i]->formalParaList.size();j++){
            cout<<"~~paraId:"<<subprogramDefinitionList[i]->formalParaList[j]->paraId.first<<" "<<subprogramDefinitionList[i]->formalParaList[j]->paraId.second<<endl;
            cout<<"type: "<<subprogramDefinitionList[i]->formalParaList[j]->type<<endl;
            cout<<"flag: "<<subprogramDefinitionList[i]->formalParaList[j]->flag<<endl;
        }
        
        cout<<"type: ";
        cout<<subprogramDefinitionList[i]->type.first<<" "<<subprogramDefinitionList[i]->type.second<<endl;
        if(subprogramDefinitionList[i]->constList.size()){
             cout<<"SubP Constlist:"<<endl;
            dfsconstlist(subprogramDefinitionList[i]->constList);}
        if(subprogramDefinitionList[i]->typedefList.size()){
            cout<<"SubP Typedeflist:"<<endl;
            dfstypedefList(subprogramDefinitionList[i]->typedefList);}
        if(subprogramDefinitionList[i]->variantList.size()){
            cout<<"SubP VariantList:"<<endl;
            dfsvariantList(subprogramDefinitionList[i]->variantList);}
        if(subprogramDefinitionList[i]->subprogramDefinitionList.size()){
            cout<<"SubP Subdef:"<<endl;
            dfssubprogramDefinitionList(subprogramDefinitionList[i]->subprogramDefinitionList);}
        if(subprogramDefinitionList[i]->compound){
            cout<<"SubP Compound:"<<endl;
            dfscompound(subprogramDefinitionList[i]->compound);
        }
    }
   
}
void dfsvariantList(vector<_Variant*> variantList){
    for(int i = 0;i < variantList.size();i++){
        cout<<"~~variantId: "<<variantList[i]->variantId.first<<" "<<variantList[i]->variantId.second<<endl;
        printType(variantList[i]->type);
    }
}
void printType(_Type *type){
    cout<<"type: "<<type->type.first<<" "<<type->type.second<<endl;
    cout<<"flag: "<<type->flag<<endl;
    if(type->flag == 1){
        cout<<"array limit:"<<endl;
        for(int i = 0;i < type->arrayRangeList.size();i++)
            cout<<type->arrayRangeList[i].first<<" "<<type->arrayRangeList[i].second<<endl;
    }
    if(type->type.first == "record")
        dfsvariantList(type->recordList);
}
void dfstypedefList(vector<_TypeDef*> typedefList){
    for(int i = 0;i < typedefList.size();i++){
        cout<<"~~typedefId: "<<typedefList[i]->typedefId.first<<" "<<typedefList[i]->typedefId.second<<endl;
        printType(typedefList[i]->type);
    }
}
void dfsconstlist(vector<_Constant*> constList){
    for(int i = 0;i < constList.size();i++){
        cout<<"~~constId: "<<constList[i]->constId.first<<" "<<constList[i]->constId.second<<endl;
        cout<<"type:"<<constList[i]->type<<endl;
        cout<<"value: ";
        switch (constList[i]->type[1]) {
            case 'd':
                cout<<constList[i]->valueId.second<<" "<<constList[i]->valueId.first;
                break;
            case 'n':
                cout<<constList[i]->intValue;
                break;
            case 'e':
                cout<<constList[i]->realValue;
                break;
            case 'h':
                cout<<constList[i]->charValue;
            default:
                break;
        }
        cout<<endl;
        cout<<"string: "<<constList[i]->strOfVal<<endl;
        cout<<"isminus: "<<constList[i]->isMinusShow<<endl;
    }
}
void dfssubP(_SubProgram* subProgram){
    if(subProgram->constList.size()){
        cout<<"~~subProgram->constList: ";
        dfsconstlist(subProgram->constList);
    }
    
    if(subProgram->typedefList.size()){
        cout<<"~~subProgram->typedefList: ";
        dfstypedefList(subProgram->typedefList);

    }
    if(subProgram->variantList.size()){
        cout<<"~~subProgram->variantList: ";

        dfsvariantList(subProgram->variantList);
    }
    if(subProgram->subprogramDefinitionList.size()){
        cout<<"~~subProgram->subprogramDefinitionList: ";
        dfssubprogramDefinitionList(subProgram->subprogramDefinitionList);
    }
    if(subProgram->compound){
        dfscompound(subProgram->compound);
    }
}
void dfsAST(_Program* ASTRoot){
    // cout<<"~~_Program->programId:"<<ASTRoot->programId.first<<" "<<ASTRoot->programId.second<<endl;
    // cout<<"~~_Program->paraList: ";
    // for(int i = 0;i < ASTRoot->paraList.size();i++)
    //     cout<<ASTRoot->paraList[i].first<<" "<<ASTRoot->paraList[i].second<<" ";
    // cout<<endl;
    // cout<<"~~_Program->subProgram: ";
    // dfssubP(ASTRoot->subProgram);
}
int str2int(string str){
    int res=0;
    int len=int(str.length());
    for(int i=0;i<len;i++){
        res=res*10;
        res+=str[i]-'0';
    }
    return res;
}

float str2float(string str){
    float res=0;
    int len=int(str.length());
    int loc=int(str.find('.'));
    for(int i=0;i<loc;i++){
        res*=10;
        res+=str[i]-'0';
    }
    float base=1;
    for(int i=loc+1;i<len;i++){
        base=base*0.1;
        res+=base*(str[i]-'0');
    }
    return res;
}
_Program* getProgram(Token *now){
    if(now->type!="programstruct"){
        cout << "getProgram error" << endl;
        return NULL;
    }
    _Program* ASTRoot=new _Program;
    getProgramHead(now->children[0],ASTRoot->programId,ASTRoot->paraList);
    ASTRoot->subProgram=getProgramBody(now->children[1]);
    dfsAST(ASTRoot);
    return ASTRoot;
}

void getProgramHead(Token *now,pair<string,int>& _programId,vector< pair<string,int> >& _paraList){
    if(now->type!="program_head"){
        cout << "getProgramHead error" << endl;
        return;
    }
    _programId=make_pair(now->children[1]->value,now->children[1]->lineNo);
    getIdList(now->children[3],_paraList,true);
}

_SubProgram* getProgramBody(Token *now){
    if(now->type!="program_body"){
        cout << "getProgramBody error" << endl;
        return NULL;
    }
    _SubProgram *_subProgram=new _SubProgram;
    getConstList(now->children[0],_subProgram->constList);
    getTypeDefList(now->children[1],_subProgram->typedefList);
    getVariantList(now->children[2],_subProgram->variantList);
    getSubprogramDefinitionList(now->children[3],_subProgram->subprogramDefinitionList);
    _subProgram->compound = getCompoundStatement(now->children[4]);
    return _subProgram;
}

void getIdList(Token *now,vector< pair<string,int> >& res,bool reverseFlag){
    if(now->type!="idlist"){
        cout << "getIdList error" << endl;
        return;
    }
    if(now->children.size() > 1){
        res.push_back(make_pair(now->children[2]->value,now->children[2]->lineNo));
        getIdList(now->children[0],res,reverseFlag);
    }
    else{
        res.push_back(make_pair(now->children[0]->value,now->children[0]->lineNo));
        if(reverseFlag)
            reverse(res.begin(),res.end());
    }
}

void getConstList(Token *now,vector<_Constant*>& _constantList){
    if(now->type!="const_declarations"){
        cout << "getConstList error" << endl;
        return;
    }
    if(now->children.size())
        getConst(now->children[1],_constantList);
}

void getConst(Token *now,vector<_Constant*>& _constantList){
    if(now->type!="const_declaration"){
        cout << "getConst error" << endl;
        return;
    }
    int loc=int(now->children.size()-3);//ID位置
    _Constant* _constant=new _Constant;
    _constant->constId=make_pair(now->children[loc]->value,now->children[loc]->lineNo);
    setConst(now->children[loc+2],_constant);
    _constantList.push_back(_constant);
    if(loc==2)
        getConst(now->children[0],_constantList);
    else
        reverse(_constantList.begin(),_constantList.end());
}

void setConst(Token *now,_Constant* &_constant){//pascal在定义常量时，并没有指定常量的类型，所以需要自行判断
    if(now->type!="const_variable"){
        cout << "setConst error" << endl;
        return;
    }
    int loc=1;
    if(now->children.size()==1)
        loc=0;
    if(now->children[loc]->type=="IDENTIFIER"){//如果右值是标识符
        _constant->type="id";
        _constant->valueId = make_pair(now->children[loc]->value,now->children[loc]->lineNo);
        _constant->strOfVal = now->children[loc]->value;
        _constant->isMinusShow = (loc == 1 && now->children[0]->type == "MINUS");
    }
    else if(now->children[loc]->type=="UINUM"){
        _constant->type="integer";
        _constant->intValue=str2int(now->children[loc]->value);
        _constant->strOfVal = now->children[loc]->value;
        _constant->isMinusShow = (loc == 1 && now->children[0]->type == "MINUS");
    }
    else if(now->children[loc]->type=="UFNUM"){
        _constant->type="real";
        _constant->realValue=str2float(now->children[loc]->value);
        _constant->strOfVal = now->children[loc]->value;
        _constant->isMinusShow = (loc == 1 && now->children[0]->type == "MINUS");
    }
    else if(now->children[loc]->type=="BOOL_CONSTANT"){
        _constant->type="boolean";
        if(now->children[loc]->value == "true")
            _constant->boolvalue=true;
        else
            _constant->boolvalue=false;
        _constant->strOfVal = now->children[loc]->value;
    }
    else if(now->children[loc]->type=="CHAR"){
        _constant->type="char";
        _constant->charValue=now->children[loc]->value[0];
        _constant->strOfVal = now->children[loc]->value;
    }
    else{
        cout << "setConst error" << endl;
    }
}
void getTypeDefList(Token *now,vector<_TypeDef*>& _typedefList){
    if(now->type!="type_declarations"){
        cout << "getTypeDefList error" << endl;
        return;
    }
    if(now->children.size())
        getTypeDef(now->children[1],_typedefList);
}
void getTypeDef(Token *now,vector<_TypeDef*>& _typedefList){
    if(now->type!="type_declaration"){
        cout << "getTypeDef error" << endl;
        return;
    }
    int loc = int(now->children.size()-3);
    _Type *_type=getType(now->children[loc+2]);
    _typedefList.push_back(new _TypeDef(make_pair(now->children[loc]->value,now->children[loc]->lineNo),_type));
    if(loc==2)
        getTypeDef(now->children[0],_typedefList);
    else
        reverse(_typedefList.begin(),_typedefList.end());
}
void getVariantList(Token *now,vector<_Variant*>& _variantList){
    if(now->type!="var_declarations"){
        cout << "getVariantList error" << endl;
        return;
    }
    if(now->children.size())
        getVariant(now->children[1],_variantList);
}
void getVariant(Token *now,vector<_Variant*>& _variantList){
    if(now->type!="var_declaration"){
        cout << "getVariant error" << endl;
        return;
    }
    vector< pair<string,int> > _idList;
    int loc=int(now->children.size()-3);//idlist位置
    getIdList(now->children[loc],_idList,false);
    _Type *_type=getType(now->children[loc+2]);//获取变量类型
    for(int i=0;i<_idList.size();i++)
        _variantList.push_back(new _Variant(_idList[i],_type));
    if(loc==2)
        getVariant(now->children[0],_variantList);
    else
        reverse(_variantList.begin(),_variantList.end());
}

_Type* getType(Token *now){
    if(now->type!="type"){
        cout << "getType error" << endl;
        return NULL;
    }
    _Type* _type = new _Type;
    int loc=int(now->children.size()-1);
    if(loc==0&&now->children[0]->type!="standard_type"){ //type->IDENTIFIER
        _type->flag=0;
        _type->type = make_pair(now->children[0]->value, now->children[0]->lineNo);
        return _type;
    }
    if(loc != 2)
    _type->type=make_pair(now->children[loc]->children[0]->value,now->children[loc]->children[0]->lineNo);
    if(loc==5){
        _type->flag=1;
        _Type* Type=(_Type*)(getType(now->children[5]));
        _type->type = make_pair(Type->type.first, now->children[0]->lineNo);
        getArrayRangeList(now->children[2],_type->arrayRangeList);
    }
    else
        _type->flag=0;
    if(loc == 2){
        _type->type = make_pair("record", now->children[0]->lineNo);
        getrecordBody(now->children[1],_type->recordList);
    }
    return _type;
}
void getrecordBody(Token *now,vector<_Variant*> &recordList){
    if(now->type!="record_body"){
        cout << "getType error" << endl;
        return ;
    }
    getVariant(now->children[0],recordList);
}
void getArrayRangeList(Token *now,vector< pair<int,int> >& _arrayRangeList){
    if(now->type!="periods"){
        cout << "getArrayRangeList error" << endl;
        return;
    }
    int loc = now->children.size()-1;
    if(loc == 0){
        getArrayRange(now->children[loc],_arrayRangeList);
        reverse(_arrayRangeList.begin(),_arrayRangeList.end());
    }
    else if(loc > 0){
        getArrayRange(now->children[loc],_arrayRangeList);
        getArrayRangeList(now->children[0],_arrayRangeList);
    }
}

void getArrayRange(Token *now,vector< pair<int,int> >& _arrayRangeList){
    if(now->type!="period"){
        cout << "getArrayRange error" << endl;
        return;
    }
    _arrayRangeList.push_back(make_pair(str2int(now->children[0]->children[0]->value),str2int(now->children[3]->children[0]->value)));
}


void getSubprogramDefinitionList(Token *now,vector<_FunctionDefinition*>& _subprogramDefinitionList){
    if(now->type!="subprogram_declarations"){
        cout << "getSubprogramDefinitionList error" << endl;
        return;
    }
    if (now->children.size()) {
        _subprogramDefinitionList.push_back(getSubprogramDefinition(now->children[1]));
        getSubprogramDefinitionList(now->children[0], _subprogramDefinitionList);
    }
    else
        reverse(_subprogramDefinitionList.begin(),_subprogramDefinitionList.end());
}

_FunctionDefinition* getSubprogramDefinition(Token *now){
    if(now->type!="subprogram_declaration"){
        cout << "getSubprogramDefinition error" << endl;
        return NULL;
    }
    _FunctionDefinition *_functionDefinition=new _FunctionDefinition;
    getSubprogramHead(now->children[0],_functionDefinition->functionID,_functionDefinition->formalParaList,_functionDefinition->type);
    getSubprogramBody(now->children[1],_functionDefinition->constList,_functionDefinition->typedefList,_functionDefinition->variantList,_functionDefinition->subprogramDefinitionList,_functionDefinition->compound);
    return _functionDefinition;
}
void getSubprogramHead(Token *now,pair<string,int>& functionID,vector<_FormalParameter*>& _formalParaList,pair<string,int> &_type){
    if(now->type!="subprogram_head"){
        cout << "getSubprogramHead error" << endl;
        return;
    }
    functionID=make_pair(now->children[1]->value,now->children[1]->lineNo);
    getFormalParaList(now->children[2],_formalParaList);
    _type=make_pair("",-1);//pocedure
    if (now->children.size() == 6)
        _type = make_pair(now->children[4]->children[0]->value, now->children[4]->children[0]->lineNo);//function
}
void getFormalParaList(Token *now,vector<_FormalParameter*>& _formalParaList){
    if(now->type!="formal_parameter"){
        cout << "getFormalParaList error" << endl;
        return;
    }
    if(now->children.size())
        getFormalParameters(now->children[1],_formalParaList);
}
void getFormalParameters(Token *now,vector<_FormalParameter*>& _formalParaList){
    if(now->type!="parameter_lists"){
        cout << "getFormalParameters error" << endl;
        return;
    }
    int loc=int(now->children.size()-1);
    getParameter(now->children[loc],_formalParaList);
    if(loc==2)
        getFormalParameters(now->children[0],_formalParaList);
    else
        reverse(_formalParaList.begin(),_formalParaList.end());
    
}
void getParameter(Token *now,vector<_FormalParameter*>& _formalParaList){
    if(now->type!="parameter_list"){
        cout << "getParameter error" << endl;
        return;
    }
    if(now->children[0]->type=="var_parameter")
        getValueParameter(now->children[0]->children[1],_formalParaList,1);
    else if(now->children[0]->type=="value_parameter")
        getValueParameter(now->children[0],_formalParaList,0);
    else
        cout << "getParameter error" << endl;
}
void getValueParameter(Token *now,vector<_FormalParameter*>& _formalParaList,int flag){
    if(now->type!="value_parameter"){
        cout << "getValueParameter error" << endl;
        return;
    }
    vector< pair<string,int> > _idList;
    getIdList(now->children[0],_idList,false);
    string _type=now->children[2]->children[0]->value;
    for(int i=0;i<_idList.size();i++)
        _formalParaList.push_back(new _FormalParameter(_idList[i],_type,flag));
}

void getSubprogramBody(Token *now,vector<_Constant*>& _constList,vector<_TypeDef*> _typedefList,vector<_Variant*>& _variantList,vector<_FunctionDefinition*> &_subprogramDefinitionList,_Compound* &_compound){
    if(now->type!="program_body"){
        cout << "getSubprogramBody error" <<endl;
        return;
    }
    getConstList(now->children[0],_constList);
    getTypeDefList(now->children[1],_typedefList);
    getVariantList(now->children[2],_variantList);
    getSubprogramDefinitionList(now->children[3],_subprogramDefinitionList);
    _compound = getCompoundStatement(now->children[4]);
}

_Compound* getCompoundStatement(Token *now){
    if(now->type!="compound_statement"){
        cout << "getCompoundStatement error" << endl;
        return NULL;
    }
    _Compound *_compound = new _Compound;
    _compound->lineNo = now->children[0]->lineNo;
    _compound->type = "compound";
    getStatementList(now->children[1],_compound->statementList);
    return _compound;
}
 

void getStatementList(Token *now,vector<_Statement*>& _statementList){
    if(now->type!="statement_list"){
        cout << "getStatementList error" << endl;
        return;
    }
    int loc=int(now->children.size()-1);
    _Statement* statement = getStatement(now->children[loc]);
    if(statement != NULL)
        _statementList.push_back(statement);
    if(loc==2)
        getStatementList(now->children[0],_statementList);
    else
        reverse(_statementList.begin(),_statementList.end());
}
_Statement* getStatement(Token *now){
    if(now->type!="statement"){
        cout << "getStatement error" << endl;
        return NULL;
    }
    if (now->children.size() == 0)
        return NULL;
    if(now->children[0]->type=="IDENTIFIER"){
        _AssignStatement *_assignStatement = new _AssignStatement;
        _assignStatement->lineNo = now->children[2]->lineNo;
        _assignStatement->type="assign";//！！！！！！！！！
        _assignStatement->variantReference = new _VariantReference;
        _assignStatement->variantReference->locFlag= -1;
        _assignStatement->variantReference->variantId = make_pair(now->children[0]->value, now->children[0]->lineNo);
        getidVpartList(now->children[1],_assignStatement->variantReference->IdvpartList);
        _assignStatement->expression=getExpression(now->children[3]);
        if(_assignStatement->expression->type=="function"){
            _assignStatement->expression->variantReference = new _VariantReference;
            _assignStatement->expression->variantReference->locFlag=1;}
        return _assignStatement;
    }
    else if(now->children[0]->type=="procedure_call")
        return getProcedureCall(now->children[0]);
    else if(now->children[0]->type=="compound_statement")
        return getCompoundStatement(now->children[0]);
    else if(now->children[0]->type=="IF"){
        _IfStatement* _ifStatement = new _IfStatement;
        _ifStatement->lineNo = now->children[0]->lineNo;
        _ifStatement->type="if";
        _ifStatement->condition=getExpression(now->children[1]);
        _ifStatement->then=getStatement(now->children[3]);
        _ifStatement->els=getElseStatement(now->children[4]);
        return _ifStatement;
    }
    else if(now->children[0]->type=="CASE"){
        _CaseStatement* _caseStatement = new _CaseStatement;
        _caseStatement->type = "case";
        _caseStatement->lineNo = now->children[0]->lineNo;
        _caseStatement->caseid = getExpression(now->children[1]);
        getCaseBody(now->children[3],_caseStatement);
        return _caseStatement;
    }
    else if(now->children[0]->type=="FOR"){
        _ForStatement* _forStatement = new _ForStatement;
        _forStatement->lineNo = now->children[0]->lineNo;
        _forStatement->type="for";
        _forStatement->id=make_pair(now->children[1]->value,now->children[1]->lineNo);
        _forStatement->start=getExpression(now->children[3]);
        if(_forStatement->start->type=="function" || _forStatement->start->type=="var")
            _forStatement->start->variantReference->locFlag=1;
        _forStatement->end=getExpression(now->children[5]);
        if(_forStatement->end->type=="function" || _forStatement->end->type=="var")
            _forStatement->end->variantReference->locFlag=1;
        _forStatement->_do=getStatement(now->children[7]);
        getforStatementOperation(now,_forStatement);
        return _forStatement;
    }
    else if(now->children[0]->type=="WHILE"){
        _WhileStatement* _whileStatement = new _WhileStatement;
        _whileStatement->lineNo = now->children[0]->lineNo;
        _whileStatement->type="while";
        _whileStatement->condition=getExpression(now->children[1]);
        _whileStatement->_do=getStatement(now->children[3]);
        return _whileStatement;
    }
    else if(now->children[0]->type=="REPEAT"){
        _RepeatStatement* _repeatStatement = new _RepeatStatement;
        _repeatStatement->lineNo = now->children[0]->lineNo;
        _repeatStatement->type="repeat";
        _repeatStatement->condition=getExpression(now->children[3]);
        getStatementList(now->children[1],_repeatStatement->_do);
        return _repeatStatement;
    }
    else{
        cout << "[getStatement] statement token error" << endl;
        return NULL;
    }
}
void getCaseBody(Token *now,_CaseStatement* &_caseStatement){
    if(now->type!="case_body"){
        cout << "getCaseBody error" << endl;
        return;
    }
    if(now->children.size() > 0){
        getBranchlist(now->children[0],_caseStatement);
    }
}
void getBranchlist(Token *now,_CaseStatement* &_caseStatement){
    if(now->type!="branch_list"){
        cout << "getBranchlist error" << endl;
        return;
    }
    int loc = now->children.size()-1;
    getBranch(now->children[loc],_caseStatement);
    if(loc > 0){
        getBranchlist(now->children[0],_caseStatement);
    }
    else{
        reverse(_caseStatement->branch.begin(), _caseStatement->branch.end());
    }
}
void getBranch(Token *now,_CaseStatement* &_caseStatement){
    if(now->type!="branch"){
        cout << "getBranch error" << endl;
        return;
    }
    vector<_Constant *> constlist;
    _Branch* branch = new _Branch;
    getConstlist(now->children[0],constlist);
    for(int i = 0;i < constlist.size();i++){
        _Expression * condition = new _Expression;
        //构建for循环条件condition
        condition->operationType="relop";
        condition->type="compound";
        condition->operation="=";
        condition->operand1 = new _Expression;
        condition->operand1=_caseStatement->caseid;
        condition->operand2 = new _Expression;
        const2exp(constlist[i], condition->operand2);
        branch->condition.push_back(condition);
    }
    branch->_do = getStatement(now->children[2]);
    _caseStatement->branch.push_back(branch);
    
}
void getConstlist(Token *now,vector<_Constant *> &constlist){
    if(now->type!="const_list"){
        cout << "getConstlist error" << endl;
        return;
    }
    int loc = now->children.size() - 1;
    _Constant * _constant = new _Constant;
    setConst(now->children[loc], _constant);
    constlist.push_back(_constant);
    if(loc > 0){
        getConstlist(now->children[0], constlist);
    }
    else{
        reverse(constlist.begin(), constlist.end());
    }
}
//构建for循环增量
void getforStatementOperation(Token *now,_ForStatement* &_forStatement){
    //构建for循环条件condition
    _forStatement->condition->operationType="relop";
     _forStatement->condition->type="compound";
     if(now->children[4]->children[0]->value=="downto"){
       _forStatement->condition->operation=">=";
     }
     else{
       _forStatement->condition->operation="<=";
     }
    _forStatement->condition->operand1 = new _Expression;
      _forStatement->condition->operand1->type="var";
     _forStatement->condition->operand1->variantReference = new _VariantReference;
     _forStatement->condition->operand1->variantReference->variantId = _forStatement->id;
    _forStatement->condition->lineNo = _forStatement->lineNo;
    _forStatement->condition->operand2 = new _Expression;
      _forStatement->condition->operand2 = _forStatement->end;

      //构建for循环初始化initial
      _forStatement->initial->lineNo =  _forStatement->lineNo;
      _forStatement->initial->type="assign";
      _forStatement->initial->variantReference = new _VariantReference;
      _forStatement->initial->variantReference->variantId = _forStatement->id;
    _forStatement->initial->expression = new _Expression;
      _forStatement->initial->expression = _forStatement->start;

    _forStatement->increment->lineNo = _forStatement->lineNo;
    _forStatement->increment->type="assign";
    _forStatement->increment->variantReference = new _VariantReference;
    _forStatement->increment->variantReference->variantId = _forStatement->id;
    _forStatement->increment->expression = new _Expression;
    _forStatement->increment->expression->type="compound";
    _forStatement->increment->expression->operationType="addop";
    _forStatement->increment->expression->lineNo =  _forStatement->lineNo;
    //判断循环量递增还是递减
    if(_forStatement->condition->operation==">=")
        _forStatement->increment->expression->operation = "-";
    else
        _forStatement->increment->expression->operation = "+";
    _forStatement->increment->expression->operand1 = new _Expression;
    _forStatement->increment->expression->operand1 =   _forStatement->condition->operand1;
    _forStatement->increment->expression->operand2 = new _Expression;
    _forStatement->increment->expression->operand2->type="integer";
    _forStatement->increment->expression->operand2->strOfNum = "1";
    _forStatement->increment->expression->operand2->intNum=1;
    _forStatement->increment->expression->operand2->lineNo=_forStatement->lineNo;
}
//获取结构体.属性或数组维度上下限
void getidVpartList(Token *now,vector<_Idvpart*> &idvpartlist){
    if(now->type!="variable"){
        cout << "getVariantList error" << endl;
        return;
    }
    if(now->children.size()!=0){
        getVpart(now->children[1],idvpartlist);
        getidVpartList(now->children[0],idvpartlist);
    }
    else{
        reverse(idvpartlist.begin(),idvpartlist.end());
    }
}
void getVpart(Token *now,vector<_Idvpart*> &idvpartlist){
    if(now->type!="id_varpart"){
        cout << "getVpart error" << endl;
        return;
    }
    _Idvpart* idvpart = new _Idvpart;
    if(now->children.size() == 3){
        idvpart->flag = 0;
        getExpressionList(now->children[1],idvpart->expressionList);
    }
    else if(now->children.size() == 2){
        idvpart->flag = 1;
        idvpart->IdvpartId = make_pair(now->children[1]->value, now->children[1]->lineNo);
    }
    idvpartlist.push_back(idvpart);
}

_Statement* getProcedureCall(Token *now) {
    if (now->type != "procedure_call") {
        cout << "getProcedureCall error" << endl;
        return NULL;
    }
    _ProcedureCall *_procedureCall = new _ProcedureCall;
    _procedureCall->lineNo = now->children[0]->lineNo;
    _procedureCall->type = "procedure";
    _procedureCall->procedureId = make_pair(now->children[0]->value, now->children[0]->lineNo);
    if (now->children.size() == 4)
        getExpressionList(now->children[2], _procedureCall->actualParaList);
    return _procedureCall;
}

void getExpressionList(Token *now,vector<_Expression*>& _expressionList){
    if(now->type!="expression_list"){
        cout << "getExpressionList error" << endl;
        return;
    }
    int loc=int(now->children.size()-1);
    _expressionList.push_back(getExpression(now->children[loc]));
    if(loc==2)
        getExpressionList(now->children[0],_expressionList);
    else
        reverse(_expressionList.begin(),_expressionList.end());
}

_Expression* getExpression(Token *now){
    if(now->type!="expression"){
        cout << "getExpression error" << endl;
        return NULL;
    }
    _Expression* _expression=NULL;
    if(now->children.size() == 3){
        _expression = new _Expression;
        _expression->type="compound";
        _expression->operation=now->children[1]->value;
        _expression->operationType="relop";
        _expression->operand1=getSimpleExpression(now->children[0]);
        _expression->operand2=getSimpleExpression(now->children[2]);
        _expression->lineNo = _expression->operand1->lineNo;
    }
    else
        _expression=getSimpleExpression(now->children[0]);
    return _expression;
}
_Expression* getSimpleExpression(Token *now){
    if(now->type!="simple_expression"){
        cout << "getSimpleExpression error" << endl;
        return NULL;
    }
    _Expression* _expression=NULL;
    if(now->children.size()==3){
        _expression = new _Expression();
        _expression->type="compound";
        _expression->operation=now->children[1]->value;
        _expression->operationType="addop";
        _expression->operand1=getSimpleExpression(now->children[0]);
        _expression->operand2=getTerm(now->children[2]);
        _expression->lineNo = _expression->operand1->lineNo;
    }
    else if(now->children.size()==2){
          _expression = new _Expression();
          _expression->operand1=getTerm(now->children[1]);
          _expression->type="compound";
          _expression->isMinusShow=0;
          _expression->operation="minus";
          _expression->operationType="single";
         if(now->children[0]->type=="MINUS")
            _expression->isMinusShow=1;
    }
    else
        _expression=getTerm(now->children[0]);
    return _expression;
}

_Expression* getTerm(Token *now){
    if(now->type!="term"){
        cout << "term" << endl;
        return NULL;
    }
    _Expression* _expression=NULL;
    if(now->children.size()==3){
        _expression = new _Expression;
        _expression->type="compound";
        _expression->operation=now->children[1]->value;
        _expression->operationType="mulop";
        _expression->operand1=getTerm(now->children[0]);
        _expression->operand2=getFactor(now->children[2]);
        _expression->lineNo = _expression->operand1->lineNo;
    }
    else
        _expression=getFactor(now->children[0]);
    return _expression;
}
_Expression* getFactor(Token *now){
    if(now->type!="factor"){
        cout << "getFactor error" << endl;
        return NULL;
    }
    _Expression* _expression = new _Expression;
    _expression->operand1=_expression->operand2=NULL;
    if(now->children[0]->type=="UINUM"){
        _expression->type="integer";
        _expression->strOfNum = now->children[0]->value;
        _expression->intNum=str2int(now->children[0]->value);
        _expression->lineNo=now->children[0]->lineNo;
    }
    else if(now->children[0]->type=="UFNUM"){
        _expression->type="real";
        _expression->strOfNum = now->children[0]->value;
        _expression->realNum=str2float(now->children[0]->value);
        _expression->lineNo=now->children[0]->lineNo;
    }
    else if(now->children[0]->type=="IDENTIFIER" && now->children.size()==2){
        //！！！！！！！！！
        _expression->type="var";
        _expression->variantReference = new _VariantReference;
        _expression->variantReference->variantId = make_pair(now->children[0]->value, now->children[0]->lineNo);
        getidVpartList(now->children[1],_expression->variantReference->IdvpartList);
        _expression->lineNo = now->children[0]->lineNo;
    }
    else if(now->children[0]->type=="IDENTIFIER" && now->children.size()==4){
        _expression->type="function";
        _expression->functionCall = new _FunctionCall;
        _expression->functionCall->functionId=make_pair(now->children[0]->value,now->children[0]->lineNo);
        getExpressionList(now->children[2],_expression->functionCall->actualParaList);
        _expression->lineNo = _expression->functionCall->functionId.second;
    }
    else if(now->children[0]->type=="("){
        _expression->type="compound";
        _expression->operationType = "single";
        _expression->operation="bracket";
        _expression->operand1=getExpression(now->children[1]);
        _expression->lineNo = _expression->operand1->lineNo;
    }
    else if(now->children[0]->type=="NOT"){
        _expression->type="compound";
        _expression->operationType = "single";
        _expression->operation="not";
        _expression->operand1=getFactor(now->children[1]);
        _expression->lineNo = _expression->operand1->lineNo;
    }
    else if (now->children[0]->type == "LETTER") {
        _expression->type = "char";
        _expression->charVal = now->children[0]->value[0];
        _expression->lineNo = now->children[0]->lineNo;
    }
    else if (now->children[0]->type == "BOOL_CONSTANT"){
        _expression->type = "boolean";
        _expression->boolValue= now->children[0]->value[0];
        _expression->lineNo = now->children[0]->lineNo;
    }
    else{
        cout << "getFactor error" << endl;
        return NULL;
    }
    return _expression;
}

_Statement* getElseStatement(Token *now){
    if(now->type!="else_part"){
        cout << "getElseStatement error" << endl;
        return NULL;
    }
    if(now->children.size()==0)
        return NULL;
    return getStatement(now->children[1]);
}

void const2exp(_Constant * constant,_Expression *& _expression){
    if(constant->isMinusShow == 1){
        _expression->type="compound";
        _expression->isMinusShow=1;
        _expression->operation="minus";
        _expression->operationType="single";
        _expression->operand1 = new _Expression;
        if(constant->type == "integer"){
            _expression->operand1->type = "integer";
            _expression->operand1->intNum = constant->intValue;
        }
        else if(constant->type == "real"){
            _expression->operand1->type = "real";
            _expression->operand1->realNum=constant->realValue;
        }
        else if(constant->type == "char"){
            _expression->operand1->type = "char";
            _expression->operand1->charVal = constant->charValue;
        }
        else if(constant->type == "boolean"){
            _expression->operand1->type = "boolean";
            if(constant->boolvalue==true)
                _expression->boolValue="true";
            else
                 _expression->boolValue="false";
        }
        else{
            //id
            _expression->operand1->type = "var";
            _expression->operand1->variantReference = new _VariantReference;
            _expression->operand1->variantReference->variantId =constant->constId;
        }
        _expression->operand1->strOfNum = constant->strOfVal;
        
    }
    else{
        if(constant->type == "integer"){
            _expression->type = "integer";
            _expression->intNum = constant->intValue;
        }
        else if(constant->type == "real"){
            _expression->type = "real";
            _expression->realNum=constant->realValue;
        }
        else if(constant->type == "char"){
            _expression->type = "char";
            _expression->charVal = constant->charValue;
        }
        else if(constant->type == "boolean"){
            _expression->type = "boolean";
            _expression->boolValue = constant->boolvalue;
        }
        else{
            //id
            _expression->type = "var";
            _expression->variantReference = new _VariantReference;
            _expression->variantReference->variantId =constant->constId;
        }
        _expression->strOfNum = constant->strOfVal;
    }
    
}
