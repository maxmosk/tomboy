/* ----------------------------------------
 *
 * Syntax description:
 * program    -> operators
 * oparators  -> statement; operators | empty
 * statement  -> if | while | assign | print
 * if         -> IF LEFT_PARENTHESS expression RIGHT_PARENTHESS
 *                  LEFT_BRACE oparators RIGHT_BRACE
 * while      -> WHILE LEFT_PARENTHESS expression RIGHT_PARENTHESS
 *                  LEFT_BRACE oparators RIGHT_BRACE
 * print      -> PRINT expression
 * assign     -> ID EQUAL (expression | INPUT)
 * expression -> term (PLUS | MINUS) expression | term
 * term       -> factor (MULT | DIV) term | factor
 * factor     -> LEFT_PARENTHESS expression RIGHT_PARENTHESS | value
 * value      -> ID | INT_LITERAL
 *
 * ---------------------------------------- */


%require  "3.2"
%language "c++"
%skeleton "lalr1.cc"

%defines
%define api.value.type variant

%param {yy::Driver* driver}

%code requires
{
#include <string>

namespace yy
{
class Driver;
}
}

%code
{
#include "driver.hpp"

namespace yy {
parser::token_type yylex(parser::semantic_type *yylval, Driver *driver);
}
}


%token
    PLUS
    MINUS
    MULT
    DIV
    EQUAL
    SCOLON
    INPUT
    IF
    WHILE
    PRINT
    LEFT_PARENTHESS
    RIGHT_PARENTHESS
    LEFT_BRACE
    RIGHT_BRACE
    ERROR
;

%token <int> INT_LITERAL
%token <std::string *> ID

%nterm oparators
%nterm statement
%nterm if
%nterm while
%nterm assign
%nterm print
%nterm expression

%start program

%%
program:    oparators
;

oparators:  statement SCOLON oparators
        |   statement SCOLON
;

statement:  if
        |   while
        |   assign
        |   print
;

if:         IF LEFT_PARENTHESS expression RIGHT_PARENTHESS
                    LEFT_BRACE oparators RIGHT_BRACE
;

while:      WHILE LEFT_PARENTHESS expression RIGHT_PARENTHESS
                    LEFT_BRACE oparators RIGHT_BRACE
;

print:      PRINT expression
;

assign:     ID EQUAL expression
        |   ID EQUAL INPUT
;

expression: term PLUS  expression
        |   term MINUS expression
        |   term
;

term:       factor MULT term
        |   factor DIV  term
        |   factor
;

factor:     LEFT_PARENTHESS expression RIGHT_PARENTHESS
        |   value
;

value:      ID
        |   INT_LITERAL
;
%%


namespace yy {

parser::token_type yylex(parser::semantic_type *yylval, Driver *driver)
{
    return driver->yylex(yylval);
}

void parser::error(const std::string&)
{

}
}
