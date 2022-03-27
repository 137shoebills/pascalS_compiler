%{
#include "main.h"
#include "LexicalAnalysis.h"

%}

line \n.*
digit [0-9]
letter [a-zA-Z]
identifier {letter}({letter}|{digit})*
_integer {digit}+
floater {digit}+\.{digit}+
exponent ({_integer}|{floater})E((+{_integer})|(-{_integer})|({_integer}))
blank_chars [ \f\r\t\v]*
assginop (:=)
relop (>= | > | <= | < | <> | =)
addop (\+ | or)
mulop (\* | \/ | div | mod | and)
divop (\( | \) | \[ | \] | : | , | ; | \.)
noteop (\(\* | \*\) | \{ | \})



%%
[a-zA-Z]+ {words++; chars += strlen(yytext);}
\n        {chars++;lines++;}
.         {chars++;}
%%

main(int argc,char **argv)
{
    yylex();
    printf("lines:%8d words:%8d chars:%8d\n",lines,words,chars);
}