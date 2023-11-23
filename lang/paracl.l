%option c++
%option yylineno
%option noyywrap


%{

%}


WHITE_SPACE     [ \t\n\v]+
DIGIT0          [0-9]
OP_ARITH        [+-/*]
ID              [A-Za-z_][0-9A-Za-z_]*
SCOLON          ;
EQUAL           =
IF_OPERATOR     "if"
WHILE_OPERATOR  "while"
PRINT_OPERATOR  "print"
INPUT_OPERATOR  "?"


%%

{WHITE_SPACE}       /* skip all spaces/tabs/other */
{IF_OPERATOR}       // TODO
{WHILE_OPERATOR}    // TODO
{PRINT_OPERATOR}    // TODO
[+-]?{DIGIT0}+      // TODO
{ID}                // TODO
{SCOLON}            // TODO
{EQUAL}             // TODO
{INPUT_OPERATOR}    // TODO
{OP_ARITH}          // TODO
.                   // TODO

%%
