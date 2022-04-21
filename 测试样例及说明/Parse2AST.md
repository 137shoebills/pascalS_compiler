[toc]

# 语法树转AST



## 数据结构设计

### 主程序[^1]

~~~c++
class _Program//程序
{
    public:
        pair<string,int> programId;//PASCAL程序名称标识符及行号
        vector< pair<string,int> > paraList;//PASCAL程序参数列表及行号
        _SubProgram* subProgram;//分程序
}
~~~
![image-20220420224657708](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220420224657708.png)

### 分程序

~~~c++
class _SubProgram//分程序
{
    public:
        vector<_Constant*> constList;//常数定义列表
        vector<_Variant*> variantList;//变量定义列表
        vector<_TypeDef*> typedefList;//重构定义列表
        vector<_FunctionDefinition*> subprogramDefinitionList;//子程序和子函数定义列表
        _Compound *compound;//主程序体
}
~~~

![image-20220421101240202](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421101240202.png)

### 常数定义

~~~c++
class _Constant//常量定义
{//常量定义的时候，右值可以是已经定义好的常量标识符
//在代码生成的时候，可以根据常量的范围对类型进行进一步的细化
    public:
        pair<string,int> constId;
        string type;//常数类型,分为"id","integer","real","char"
        pair<string,int> valueId;
        char charValue;
				int intValue;
        float realValue;
        string strOfVal;//所有常量取值的字符串表示
        bool isMinusShow;//是否为负数
}
~~~

![image-20220421122446515](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421122446515.png)

### 变量定义

~~~c++
class _Variant//变量定义
{
    public:
        pair<string,int> variantId;//变量标识符ID及行号
        _Type *type;//变量类型
};
~~~

![image-20220421124443539](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421124443539.png)

### 类型定义

~~~c++
class _Type//类型
{
    public:
        pair<string,int> type;//基本类型及行号 "integer"、"char"、"real"、"boolean" 
        int flag;//0表示非数组，1表示数组
        vector<pair<int,int> > arrayRangeList;//flag=1时，表示数组各维上下界
}
~~~

![image-20220421124807345](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421124807345.png)

### 重构定义

~~~c++
class _TypeDef//重构定义
{
    public:
        pair<string,int> typedefId;//重构类型ID及行号
        _Type *type;//变量类型
}
~~~

![image-20220421125504888](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421125504888.png)

### 子程序和子函数定义

~~~c++
class _FunctionDefinition
{//函数/过程定义
    public:
        pair<string,int> functionID;//函数/过程标识符及行号
        vector<_FormalParameter*> formalParaList;//形式参数列表
        pair<string,int> type;//如果type.first是空串，则为过程，否则为函数,取值为"integer","real","boolean","char"四种
        vector<_Constant*> constList;//常数定义列表
        vector<_TypeDef*> typedefList;//重构定义列表
        vector<_Variant*> variantList;//变量定义列表
        vector<_FunctionDefinition*> subprogramDefinitionList;//子程序和子函数定义列表
        _Compound *compound;//子程序体
}
~~~

![image-20220421130126825](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421130126825.png)

### 形式参数定义

~~~C++
class _FormalParameter//形式参数
{
    public:
        pair<string,int> paraId;//形式参数标识符和行号
        string type;//形式参数类型，形式参数一定是基本类型，所以改为string
        int flag;//flag=0表示传值调用，flag=1表示引用调用
}
~~~

![image-20220421131022530](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421131022530.png)

### 基本语句定义

~~~c++
class _Statement
{
    public:
      string type;//"compound","repeat","while","for","if","assign","procedure"
      string  ;//语句是否有效，区别于type，取值为"void"或"error"
      int lineNo;//行号
      bool isReturnStatement;//是否是返回值语句
}
~~~

![image-20220421132708171](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421132708171.png)

### 复合语句定义

~~~c++
class _Compound:public _Statement
{
    public:
        vector<_Statement*> statementList;//语句列表
				//行号由begin的位置决定
    public:
        _Compound();
        ~_Compound();
};
~~~

![image-20220421133548813](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421133548813.png)

### 赋值语句

~~~c++
class _AssignStatement:public _Statement
{
    public:
        _VariantReference* variantReference;//左值变量
        _Expression* expression;//右值表达式
		//行号由赋值符号的位置决定
}
~~~

![image-20220421134007919](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421134007919.png)

### 过程调用语句

~~~c++
class _ProcedureCall:public _Statement
{
    public:
        pair<string,int> procedureId;//过程标识符
        vector<_Expression*> actualParaList;//实际参数列表，由表达式组成
		//行号由procedure名的位置决定
}
~~~

![image-20220421134252795](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421134252795.png)

### if语句定义

~~~c++
class _IfStatement:public _Statement
{
    public:
        _Expression *condition;//条件表达式
        _Statement *then;//满足条件时执行的语句
        _Statement *els;//不满足条件时执行的语句，如果为NULL，则没有else部分
		//行号由if的位置决定
}
~~~

![image-20220421134645763](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421134645763.png)

### For语句定义

~~~c++
class _ForStatement:public _Statement
{
    public:
        pair<string,int> id;//循环变量
        _Expression *start;//起始值
        _Expression *end;//终止值
        _Statement *_do;//循环体语句
		//行号由for的位置决定
}
~~~

![image-20220421140314508](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421140314508.png)

### Repeat语句

~~~c++
class _RepeatStatement:public _Statement
{
    public:
        _Expression *condition;//条件表达式
        vector<_Statement*> _do;//循环体语句
		//行号由repeat的位置决定
}
~~~

![image-20220421140541859](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421140541859.png)



### While语句

~~~c++
class _WhileStatement:public _Statement
{
    public:
        _Expression *condition;//条件表达式
        _Statement *_do;//循环体语句
		//行号由while的位置决定
}
~~~

![image-20220421140924596](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421140924596.png)

### 函数调用语句

~~~c++
class _FunctionCall
{
    public:
        pair<string,int> functionId;//函数标识符
        vector<_Expression*> actualParaList;//实际参数列表，由表达式组成
};
~~~

![image-20220421141235502](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421141235502.png)

### 函数体中变量定义

~~~c++
class _VariantReference
{
    public:
  		pair<string,int> variantId;//变量标识符和行号
      vector<_Idvpart*> IdvpartList;//后缀
}
~~~

![image-20220421143024388](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421143024388.png)



### 变量后缀定义

~~~c++
class _Idvpart{
public:
    vector<_Expression*> expressionList;//数组下标表示，flag = 1;
    pair<string, int> IdvpartId;//变量属性表示，flag = 0
    int flag;
}
~~~

![image-20220421144238904](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421144238904.png)

### 表达式定义 

~~~c++
class _Expression
{
    public:
        string type;//表达式类型,"var"表示变量,"integer"表示整数,"real"表示浮点数,"char"表示常量字符
        //"function"表示函数调用,"compound"表示复合表达式,
        //compound有普通的二目运算符，还有"minus"、"not"、"bracket"等单目运算符
        _VariantReference* variantReference;//变量或常量或数组
        int intNum;//整数
        float realNum;//浮点数
        string strOfNum;//整数和浮点数的string表示（考虑从PASCAL-S源程序将字符串转为浮点数，再将浮点数转为字符串会带来精度问题，所以需要存下初始字符串值）
        char charVal;//常量字符
        _FunctionCall *functionCall;//函数调用
        int isMinusShow;//是否为负
        string operation;//具体操作符
        string operationType;//操作符类型,"relop","mulop","addop","single"
        _Expression *operand1,*operand2;
        int lineNo;//行号, 用表达式中最先出现的操作数的行号表示
};
~~~

![image-20220421145348441](https://cdn.jsdelivr.net/gh/Nov29/picgo@main/img/image-20220421145348441.png)



## 转化函数

### 字符串转整型

~~~c++
int str2int(string str);
~~~

| 参数       | 描述             |
| ---------- | ---------------- |
| string str | 需要转化的字符串 |

返回转化后的int值

### 字符串转浮点型

~~~c++
float str2float(string str);
~~~

| 参数       | 描述             |
| ---------- | ---------------- |
| string str | 需要转化的字符串 |

返回转化后的float值

### 获取整个程序

~~~c++
_Program* getProgram(Token *now);
~~~

| 参数       | 描述               |
| ---------- | ------------------ |
| Token *now | 当前语法分析树节点 |

返回AST根结点指针_Program类指针

### 获取主程序头

~~~c++
void getProgramHead(Token *now, pair<string, int>& _programId, vector< pair<string, int> >& _paraList);
~~~
例如

~~~pascal
program test(input, output);
~~~



| 参数                                   | 描述               |
| -------------------------------------- | ------------------ |
| Token *now                             | 当前语法分析树节点 |
| pair<string, int>& _programId          | 主程序名及行号     |
| vector< pair<string, int> >& _paraList | 参数名及行号列表   |

### 获取主程序体

~~~c++
_SubProgram* getProgramBody(Token *now);
~~~
例如
~~~pasacal
const n=a;
var x,y: integer;

function gtcomdiv(a,b: integer):integer;
    begin
        if b=0 then gtcomdiv:=a     {这里末尾没有分号？}
        else gtcomdiv:=gtcomdiv(b,a mod b)
    end;
begin
    read(x,y);
    write(gtcomdiv(x,y));
end.
~~~
| 参数       | 描述               |
| ---------- | ------------------ |
| Token *now | 当前语法分析树节点 |



返回子程序体，_SubProgram类指针

### 获取标识符列表

~~~c++
void getIdList(Token *now, vector< pair<string, int> >& res,bool reverseFlag);
~~~
例如
~~~pasacal
input, output
~~~

| 参数                                   | 描述               |
| -------------------------------------- | ------------------ |
| Token *now                             | 当前语法分析树节点 |
| vector< pair<string, int> >& res         | 标识符名及行号列表     |
| bool reverseFlag | 是否反转标识符列表   |

### 获取常数值

~~~c++
void setConst(Token *now, _Constant* &_constant);
~~~

根据当前token类型获取符合该类型的值id,int,float,char

| 参数                  | 描述               |
| --------------------- | ------------------ |
| Token *now            | 当前语法分析树节点 |
| _Constant* &_constant | 待修改的常量定义   |

### 获取常量定义

~~~c++
void getConst(Token *now,vector<_Constant*>& _constantList)
~~~
| 参数                              | 描述               |
| --------------------------------- | ------------------ |
| Token *now                        | 当前语法分析树节点 |
| vector<_Constant*>& _constantList | 常量定义列表       |

### 获取常量定义列表

~~~c++
void getConstList(Token *now,vector<_Constant*>& _constantList);
~~~
| 参数                              | 描述               |
| --------------------------------- | ------------------ |
| Token *now                        | 当前语法分析树节点 |
| vector<_Constant*>& _constantList | 常量定义列表       |

### 获取重构定义列表

~~~c++
void getTypeDefList(Token *now,vector<_TypeDef*>& _typedefList);
~~~
| 参数                              | 描述               |
| --------------------------------- | ------------------ |
| Token *now                        | 当前语法分析树节点 |
| vector<_TypeDef*>& _typedefList | 重构定义列表       |

### 获取重构定义

~~~c++
void getTypeDef(Token *now,vector<_TypeDef*>& _typedefList);
~~~
| 参数                              | 描述               |
| --------------------------------- | ------------------ |
| Token *now                        | 当前语法分析树节点 |
| vector<_TypeDef*>& _typedefList | 重构定义列表       |

### 获取变量定义列表

~~~c++
void getVariantList(Token *now,vector<_Variant*>& _variantList);
~~~
| 参数                              | 描述               |
| --------------------------------- | ------------------ |
| Token *now                        | 当前语法分析树节点 |
| vector<_TypeDef*>& _typedefList | 变量定义列表       |

### 获取变量定义

~~~c++
void getVariant(Token *now,vector<_Variant*>& _variantList);
~~~
| 参数                              | 描述               |
| --------------------------------- | ------------------ |
| Token *now                        | 当前语法分析树节点 |
| vector<_TypeDef*>& _typedefList | 变量定义列表       |

### 获取类型

~~~c++
_Type* getType(Token *now);
~~~
| 参数                              | 描述               |
| --------------------------------- | ------------------ |
| Token *now                        | 当前语法分析树节点 |

返回值，类型具体定义，_Type类型指针

### 获取数组上下界列表

~~~c++
void getArrayRangeList(Token *now,vector< pair<int,int> >& _arrayRangeList);
~~~
| 参数                                     | 描述               |
| ---------------------------------------- | ------------------ |
| Token *now                               | 当前语法分析树节点 |
| vector< pair<int,int> >& _arrayRangeList | 数组各维上下界列表 |

### 获取数组上下界

~~~c++
void getArrayRange(Token *now,vector< pair<int,int> >& _arrayRangeList);
~~~
| 参数                                     | 描述               |
| ---------------------------------------- | ------------------ |
| Token *now                               | 当前语法分析树节点 |
| vector< pair<int,int> >& _arrayRangeList | 数组各维上下界列表 |

### 获取子函数/过程定义列表

~~~c++
void getSubprogramDefinitionList(Token *now,vector<_FunctionDefinition*>& _subprogramDefinitionList)
~~~

| 参数                                     | 描述               |
| ---------------------------------------- | ------------------ |
| Token *now                               | 当前语法分析树节点 |
| vector<_FunctionDefinition*>& _subprogramDefinitionList | 子函数定义列表 |

### 获取子函数/过程定义

~~~c++
_FunctionDefinition* getSubprogramDefinition(Token *now);
~~~

| 参数                                     | 描述               |
| ---------------------------------------- | ------------------ |
| Token *now                               | 当前语法分析树节点 |

返回子程序定义类指针

### 获取子程序头

~~~c++
void getSubprogramHead(Token *now,pair<string,int>& functionID,vector<_FormalParameter*>& _formalParaList,pair<string,int> &_type);
~~~

| 参数                                       | 描述                                                         |
| ------------------------------------------ | ------------------------------------------------------------ |
| Token *now                                 | 当前语法分析树节点                                           |
| pair<string,int>& functionID               | 子程序名及行号                                               |
| vector<_FormalParameter*>& _formalParaList | 参数列表                                                     |
| pair<string,int> &_type                    | 返回值类型及行号，可能为空串，表示无返回值，即当前子程序是过程 |

### 获取形式参数列表

~~~c++
void getFormalParaList(Token *now,vector<_FormalParameter*>& _formalParaList);
~~~

| 参数                                       | 描述               |
| ------------------------------------------ | ------------------ |
| Token *now                                 | 当前语法分析树节点 |
| vector<_FormalParameter*>& _formalParaList | 形式参数列表       |

### 获取形式参数

~~~c++
void getFormalParameters(Token *now,vector<_FormalParameter*>& _formalParaList);
~~~
| 参数                                       | 描述               |
| ------------------------------------------ | ------------------ |
| Token *now                                 | 当前语法分析树节点 |
| vector<_FormalParameter*>& _formalParaList | 形式参数列表       |

### 区别形式参数

~~~c++
void getParameter(Token *now,vector<_FormalParameter*>& _formalParaList);
~~~
| 参数                                       | 描述               |
| ------------------------------------------ | ------------------ |
| Token *now                                 | 当前语法分析树节点 |
| vector<_FormalParameter*>& _formalParaList | 形式参数列表       |

### 获取形式参数具体信息

~~~c++
void getValueParameter(Token *now,vector<_FormalParameter*>& _formalParaList,int flag);
~~~
| 参数                                       | 描述                                      |
| ------------------------------------------ | ----------------------------------------- |
| Token *now                                 | 当前语法分析树节点                        |
| vector<_FormalParameter*>& _formalParaList | 形式参数列表                              |
| int flag                                   | flag=0表示传值参数，flag!=0表示传引用参数 |

### 获取子程序体

~~~c++
void getSubprogramBody(Token *now,vector<_Constant*>& _constList,vector<_TypeDef*> _typedefList,vector<_Variant*>& _variantList,vector<_FunctionDefinition*> &_subprogramDefinitionList,_Compound* &_compound)
~~~

| 参数                                                    | 描述               |
| ------------------------------------------------------- | ------------------ |
| Token *now                                              | 当前语法分析树节点 |
| vector<_Constant*>& _constList                          | 子程序常量定义     |
| vector<_TypeDef*> _typedefList                          | 子程序重构定义     |
| vector<_Variant*>& _variantList                         | 子程序变量定义     |
| vector<_FunctionDefinition*> &_subprogramDefinitionList | 子程序子程序定义   |
| _Compound* &_compound                                   | 子程序函数体       |

### 获取函数体复合语句

~~~c++
_Compound* getCompoundStatement(Token *now);
~~~
| 参数                                                    | 描述               |
| ------------------------------------------------------- | ------------------ |
| Token *now                                              | 当前语法分析树节点 |

返回复合语句类指针

### 获取语句列表

~~~c++
void getStatementList(Token *now,vector<_Statement*>& _statementList);
~~~
| 参数                                | 描述               |
| ----------------------------------- | ------------------ |
| Token *now                          | 当前语法分析树节点 |
| vector<_Statement*>& _statementList | 语句列表           |

### 获取语句

~~~c++
_Statement* getStatement(Token *now);
~~~
| 参数                                | 描述               |
| ----------------------------------- | ------------------ |
| Token *now                          | 当前语法分析树节点 |

返回语句类指针

### 获取结构体.属性或数组维度上下限列表

~~~c++
void getidVpartList(Token *now,vector<_Idvpart*> &idvpartlist);
~~~
| 参数                           | 描述               |
| ------------------------------ | ------------------ |
| Token *now                     | 当前语法分析树节点 |
| vector<_Idvpart*> &idvpartlist | 引用变量后缀列表   |

### 获取结构体.属性或数组维度上下限

~~~c++
void getVpart(Token *now,vector<_Idvpart*> &idvpartlist)
~~~
| 参数                           | 描述               |
| ------------------------------ | ------------------ |
| Token *now                     | 当前语法分析树节点 |
| vector<_Idvpart*> &idvpartlist | 引用变量后缀列表   |

### 获取过程调用语句

~~~c++
_Statement* getProcedureCall(Token *now);
~~~
| 参数                           | 描述               |
| ------------------------------ | ------------------ |
| Token *now                     | 当前语法分析树节点 |

返回语句类型指针。

### 获取表达式列表

~~~c++
void getExpressionList(Token *now,vector<_Expression*>& _expressionList);
~~~
| 参数                           | 描述               |
| ------------------------------ | ------------------ |
| Token *now                     | 当前语法分析树节点 |
| vector<_Expression*>& _expressionList | 表达式列表   |

### 获取表达式

~~~c++
_Expression* getExpression(Token *now);
~~~
| 参数                           | 描述               |
| ------------------------------ | ------------------ |
| Token *now                     | 当前语法分析树节点 |

返回表达式类指针

### 获取简单表达式

~~~c++
_Expression* getSimpleExpression(Token *now);
~~~
| 参数                           | 描述               |
| ------------------------------ | ------------------ |
| Token *now                     | 当前语法分析树节点 |

返回表达式类指针

### 获取Term

~~~c++
_Expression* getTerm(Token *now);
~~~

| 参数                           | 描述               |
| ------------------------------ | ------------------ |
| Token *now                     | 当前语法分析树节点 |

返回表达式类指针

### 获取Factor
~~~c++
_Expression* getFactor(Token *now);
~~~

| 参数                           | 描述               |
| ------------------------------ | ------------------ |
| Token *now                     | 当前语法分析树节点 |

返回表达式类指针

### 获取else语句

~~~c++
_Statement* getElseStatement(Token *now);
~~~
| 参数                           | 描述               |
| ------------------------------ | ------------------ |
| Token *now                     | 当前语法分析树节点 |

返回语句类指针




[^1]: 以下类定义只给出成员变量定义，忽略构造函数及析构函数

