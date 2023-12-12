%option c++
%option yylineno
%option noyywrap
%option yyclass="TomboyLexer"


%{
#include "lexer.hpp"

#include "paracl.tab.hh"

#define YY_USER_ACTION token_action();
%}


WHITE_SPACE     [ \t\v]+
DIGIT0          [0-9]
ID              [A-Za-z_][0-9A-Za-z_]*
COMMENT         "//"[^"\n"]*"\n"


%%

{WHITE_SPACE}       /* skip all spaces/tabs/other */
{COMMENT}           /* ignore all comments */
"\n"                process_newline();
"if"                return yy::parser::token_type::IF;
"else"              return yy::parser::token_type::ELSE;
"while"             return yy::parser::token_type::WHILE;
"print"             return yy::parser::token_type::PRINT;
{DIGIT0}+           return yy::parser::token_type::INT_LITERAL;
{ID}                return yy::parser::token_type::ID;
";"                 return yy::parser::token_type::SCOLON;
"="                 return yy::parser::token_type::EQUAL;
"=="                return yy::parser::token_type::IS_EQUAL;
"!="                return yy::parser::token_type::NOT_EQUAL;
">"                 return yy::parser::token_type::GREATER;
"<"                 return yy::parser::token_type::LESS;
">="                return yy::parser::token_type::GREATER_OR_EQUAL;
"<="                return yy::parser::token_type::LESS_OR_EQUAL;
"?"                 return yy::parser::token_type::INPUT;
"+"                 return yy::parser::token_type::PLUS;
"-"                 return yy::parser::token_type::MINUS;
"*"                 return yy::parser::token_type::MULT;
"/"                 return yy::parser::token_type::DIV;
"!"                 return yy::parser::token_type::NOT;
"("                 return yy::parser::token_type::LEFT_PARENTHESS;
")"                 return yy::parser::token_type::RIGHT_PARENTHESS;
"{"                 return yy::parser::token_type::LEFT_BRACE;
"}"                 return yy::parser::token_type::RIGHT_BRACE;
.                   return yy::parser::token_type::ERROR;

%%
