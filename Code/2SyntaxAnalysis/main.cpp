#include "main.h"
#include <fstream>
#include <sstream>

extern FILE *yyin;
extern vector<string> lexicalErrorInformation; //存放词法分析错误
extern vector<string> syntaxErrorInformation;  //存放语法错误信息
extern Token *ParseTreeHead;
void dfs(Token *root);
string itos(int num)
{
    stringstream sin;
    sin << num;
    return sin.str();
}
void outputErrorInformation(vector<string> &error);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "Missing parameter!\n";
        return 0;
    }
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        cout << "Wrong open file!\n";
    }
    else
    {
        yyin = fp;
        cout << "begin parsing\n";
        yyparse();
        cout << "end parsing\n";
        if (ParseTreeHead)
        {
            cout << "print ParseTree\n";
            dfs(ParseTreeHead);
        }
    }

    fclose(fp);

    cout << "print lexicalerrror\n";
    outputErrorInformation(lexicalErrorInformation);
    cout << "print parseerror\n";
    outputErrorInformation(syntaxErrorInformation);

    return 0;
}

void outputErrorInformation(vector<string> &error)
{
    cout << "begin error printing\n";
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
            cout << " \"" << root->children[i]->type < "\"";
        }
    }
    cout << "\n";
    for (int i = 0; i < root->children.size(); i++)
    {
        dfs(root->children[i]);
    }
}