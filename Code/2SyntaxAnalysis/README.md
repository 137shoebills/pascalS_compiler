## 语法分析

对原lex.l做出以下修改：
* 将符号"\_BEGIN"改为了"BEGIN";
* 将原识别出字符常量的地方返回的"CHAR"修改为返回"LETTER";
同时新增了对关键字"char"的识别，返回"CHAR";
* 增加了宏定义YY_USER_ACTION，用于确定语法错误的位置;
* 将原lex.l中的main函数删除，在yacc.y中新增了main函数；

对main.h做出以下修改：
* 在class Token中新增构造函数`Token(string val) : value(val) {}`,便于构建语法树；

同时makefile中增加了对yacc.y的编译命令以及编译链接命令;
