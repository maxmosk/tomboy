/* ----------------------------------------
 *
 * Syntax description:
 * program    -> statement; program | empty
 * statement  -> if | while | assign | print
 * if         -> IF LEFT_PARENTHESS expression RIGHT_PARENTHESS
 *                  LEFT_BRACE program RIGHT_BRACE
 * while      -> WHILE LEFT_PARENTHESS expression RIGHT_PARENTHESS
 *                  LEFT_BRACE program RIGHT_BRACE
 * print      -> PRINT expression
 * assign     -> ID EQUAL (expression | INPUT)
 * expression -> expression (PLUS | MINUS) term | term
 * term       -> term (MULT | DIV) factor | factor
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

#include "node.hpp"

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

%nterm<AST::pINode> statements
%nterm<AST::pINode> statement
%nterm<AST::pINode> if
%nterm<AST::pINode> while
%nterm<AST::pINode> assign
%nterm<AST::pINode> print
%nterm<AST::pINode> expression
%nterm<AST::pINode> expression_eval
%nterm<AST::pINode> term
%nterm<AST::pINode> factor
%nterm<AST::pINode> value

%start program

%left PLUS MINUS MULT DIV

%%
program:    statements          { driver->setAST($1); }
        |   %empty              { driver->setAST(AST::make_compound(nullptr, nullptr)); }
;

statements: statement statements { $$ = AST::make_compound($1, $2); }
        |   statement
;

statement:  print SCOLON
        |   if
        |   while
        |   expression SCOLON
;

if:         IF LEFT_PARENTHESS expression RIGHT_PARENTHESS
                LEFT_BRACE statements RIGHT_BRACE { $$ = AST::make_if($3, $6); }
        |   IF LEFT_PARENTHESS expression RIGHT_PARENTHESS
                LEFT_BRACE RIGHT_BRACE { $$ = AST::make_if($3, AST::make_compound(nullptr, nullptr)); }
;

while:      WHILE LEFT_PARENTHESS expression RIGHT_PARENTHESS
                LEFT_BRACE statements RIGHT_BRACE { $$ = AST::make_while($3, $6); }
        |   WHILE LEFT_PARENTHESS expression RIGHT_PARENTHESS
                LEFT_BRACE RIGHT_BRACE { $$ = AST::make_while($3, AST::make_compound(nullptr, nullptr)); }
;

print:      PRINT expression    { $$ = AST::make_print($2); }
;

assign:     ID EQUAL expression {
                                    $$ = AST::make_assign($3, $1);
                                    delete $1;
                                }
        |   ID EQUAL INPUT      {
                                    $$ = AST::make_assign(AST::make_input(), $1);
                                    delete $1;
                                }
;

expression: expression_eval
        |   assign
;

expression_eval:    expression_eval PLUS  term { $$ = AST::make_operation($1, $3, AST::Operations::ADD); }
                |   expression_eval MINUS term { $$ = AST::make_operation($1, $3, AST::Operations::SUB); }
                |   term
;

term:       term MULT factor    { $$ = AST::make_operation($1, $3, AST::Operations::MUL); }
        |   term DIV  factor    { $$ = AST::make_operation($1, $3, AST::Operations::DIV); }
        |   factor
;

factor:     LEFT_PARENTHESS expression RIGHT_PARENTHESS { $$ = $2; }
        |   value
;

value:      ID                  {
                                    $$ = AST::make_variable($1);
                                    delete $1;
                                }
        |   INT_LITERAL         { $$ = AST::make_integer($1); }
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
