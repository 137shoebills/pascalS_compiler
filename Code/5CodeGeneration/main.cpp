#include "main.h"
#include "ParseT2AST.h"
#include "semanticAnalyse.h"
#include "CodeGen.h"
#include "ObjGen.h"
#include <fstream>
#include <sstream>

extern FILE *yyin;
extern Token *ParseTreeHead;
extern vector<string> lexicalErrorInformation; //存放词法分析错误
extern vector<string> syntaxErrorInformation;  //存放语法错误信息
extern vector<string> semanticErrorInformation; //存储错误信息的列表
extern vector<string> semanticWarningInformation;//存储警告信息的列表
extern bool codeGen_error;

string itos(int num)
{
    stringstream sin;
    sin << num;
    return sin.str();
}
void dfs(Token *root);
void outputErrorInformation(vector<string> &error);
void predeal(char *filename);

int main(int argc, char **argv)
{
    // yydebug = 1;

    string output_filename = "output.s";     //默认目标代码生成文件
    string compilerName = "gcc";             //默认C编译器
	string exeName = "PascalCompiler.exe";         //默认的编译C程序后获得的可执行文件名

    if(argc < 2)    //使用默认输出文件名"Complier.exe"
    {
        cout << "Missing parameter!\n";
        return 0;
    }
    else if(argc > 3)
    {
        cout << "Too much parameter more than three!\n";
        return 0;
    }
    else if(argc == 3)
    {
        exeName = argv[2];
    }

    if(!strcmp(argv[1],"--help") || !strcmp(argv[1],"-h")) //帮助选项
    { 
        cout << "Please put \"./compiler xxx.pas [xxx.exe]\" to run the pascalS-compiler!\n";
        return 0;
    }
    else if(!strcmp(argv[1],"--version") || !strcmp(argv[1],"-v")) //版本选项
    {
        cout << "version:2022.6\n";
        return 0;
    }
    else if(!strcmp(argv[1],"--developer") || !strcmp(argv[1],"-d")) //生成者选项
    {
        cout << "developers: Hu Minzhen ; Deng Shuyu ; Yang Lan ; Yuan Jie ; Pan Ling ; Zheng Siyang...\n";
        return 0;
    }

    char *filename = argv[1];
    predeal(filename);
    
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        cout << "Wrong open file!\n";
    }
    else
    {
        yyin = fp;
        yyparse();
    }

    fclose(fp);

    // if (ParseTreeHead) //打印语法分析树
    // {
    //     cout << "\nParseTree:\n";
    //     dfs(ParseTreeHead);
    // }

    if(lexicalErrorInformation.size() > 0)
    {
        cout << "\n***************Lexical Errrors:***************\n";
        outputErrorInformation(lexicalErrorInformation);
    }
    else{
        cout << "Lexical Succeed!\n";
    }
    if(syntaxErrorInformation.size() > 0) //若语法分析出错则退出编译
    {
        cout << "\n***************Parse Errors:***************\n";
        outputErrorInformation(syntaxErrorInformation);      
        return -1; 
    }
    else{
        cout << "Parse Succeed!\n";
    }

    _Program *ASTRoot = getProgram(ParseTreeHead);

    SemanticAnalyse(ASTRoot);
    if(semanticWarningInformation.size() > 0)
    {
        cout << "\n***************Semantic Warnings:***************\n";
        outputErrorInformation(semanticWarningInformation);        
    }
    if(semanticErrorInformation.size() > 0)
    {
        cout << "\n***************Semantic Errors:***************\n";
        outputErrorInformation(semanticErrorInformation);        
    }

    //若语义分析或中间代码生成出错，则不再进行目标代码生成
    if(semanticErrorInformation.size() > 0 || codeGen_error){
        cout<<"***************Semantic/codeGen errors, stop generating target file***************"<<endl;
        return -1;
    }
    
    cout << "SemanticAnalyse and CodeGen Succeed!\n";
    ObjCodeGen(context, output_filename);
    cout << "ObjGen Succeed!\n";

    string cmd= compilerName + " " + output_filename + " IOdll.c -o " + exeName;
    system(cmd.c_str());

    return 0;
}

void outputErrorInformation(vector<string> &error)
{
    for (int i = 0; i < error.size(); i++)
    {
        cout << error[i] << "\n";
    }
}

void dfs(Token *root)
{
    if (root->children.size() == 0)
    {
        if (root->value == "")
        {
            cout << root->type << " -> "
                 << "empty\n";
        }
        return;
    }
    cout << root->type << " ->";
    for (int i = 0; i < root->children.size(); i++)
    {
        if (root->children[i]->children.size() == 0 && root->children[i]->value != "")
        {
            cout << " \"" << root->children[i]->value << "\"";
        }
        else
        {
            cout << " \"" << root->children[i]->type << "\"";
        }
    }
    cout << "\n";
    for (int i = 0; i < root->children.size(); i++)
    {
        dfs(root->children[i]);
    }
}

void predeal(char *filename)
{
    FILE *fp;
    char *s, *sp;
    int file_size, read_size;
    if ((fp = fopen(filename, "rb+")) == NULL)
    {
        fprintf(stderr, "Can't open %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fseek(fp, 0L, SEEK_END); //文件指针移动到文件末尾
    file_size = ftell(fp);
    rewind(fp); //文件指针移动到文件开头
    //为字符数组分配内存，然后读入数据
    s = (char *)calloc(file_size, 1);
    read_size = fread(s, 1, file_size, fp);
    fclose(fp);
    if (read_size != file_size)
    {
        fprintf(stdout, "Can't read all massage\n");
        exit(EXIT_FAILURE);
    }

    //转换字母
    sp = s;
    int flag = 1;
    while (*sp)
    {
        if ((*sp) == '\'')
        {
            flag = !flag;
        }
        if (flag == 1 && isupper(*sp))
            *sp = tolower(*sp);
        sp++;
    }
    //先清除源文件数据，然后向文件写入字符数组数据
    if ((fp = fopen(filename, "wb+")) == NULL)
    {
        fprintf(stderr, "Can't open %sin", filename);
        exit(EXIT_FAILURE);
    }
    fwrite(s, 1, read_size, fp);

    fclose(fp);
}
