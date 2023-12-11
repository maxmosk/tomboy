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

%nterm<Tomboy::AST::pINode> statements
%nterm<Tomboy::AST::pINode> statement
%nterm<Tomboy::AST::pINode> if
%nterm<Tomboy::AST::pINode> while
%nterm<Tomboy::AST::pINode> assign
%nterm<Tomboy::AST::pINode> print
%nterm<Tomboy::AST::pINode> expression
%nterm<Tomboy::AST::pINode> expression_eval
%nterm<Tomboy::AST::pINode> term
%nterm<Tomboy::AST::pINode> factor
%nterm<Tomboy::AST::pINode> value
%nterm<Tomboy::AST::pINode> neg_value

%start program

%left PLUS MINUS MULT DIV

%%
program:    statements          { driver->setAST($1); }
        |   %empty              { driver->setAST(Tomboy::AST::make_compound(nullptr, nullptr)); }
;

statements: statement statements { $$ = Tomboy::AST::make_compound($1, $2); }
        |   statement
;

statement:  print SCOLON
        |   if
        |   while
        |   expression SCOLON
        |   SCOLON              { $$ = Tomboy::AST::make_compound(nullptr, nullptr); }
;

if:         IF LEFT_PARENTHESS expression RIGHT_PARENTHESS
                LEFT_BRACE statements RIGHT_BRACE { $$ = Tomboy::AST::make_if($3, $6); }
        |   IF LEFT_PARENTHESS expression RIGHT_PARENTHESS
                LEFT_BRACE RIGHT_BRACE { $$ = Tomboy::AST::make_if($3, Tomboy::AST::make_compound(nullptr, nullptr)); }
;

while:      WHILE LEFT_PARENTHESS expression RIGHT_PARENTHESS
                LEFT_BRACE statements RIGHT_BRACE { $$ = Tomboy::AST::make_while($3, $6); }
        |   WHILE LEFT_PARENTHESS expression RIGHT_PARENTHESS
                LEFT_BRACE RIGHT_BRACE { $$ = Tomboy::AST::make_while($3, Tomboy::AST::make_compound(nullptr, nullptr)); }
;

print:      PRINT expression    { $$ = Tomboy::AST::make_print($2); }
;

assign:     ID EQUAL expression {
                                    $$ = Tomboy::AST::make_assign($3, $1);
                                    delete $1;
                                }
;

expression: expression_eval
        |   assign
;

expression_eval:    expression_eval PLUS  term { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::ADD); }
                |   expression_eval MINUS term { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::SUB); }
                |   term
;

term:       term MULT factor    { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::MUL); }
        |   term DIV  factor    { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::DIV); }
        |   factor
;

factor:     LEFT_PARENTHESS expression RIGHT_PARENTHESS { $$ = $2; }
        |   value
;

value:      ID                  {
                                    $$ = Tomboy::AST::make_variable($1);
                                    delete $1;
                                }
        |   INT_LITERAL         { $$ = Tomboy::AST::make_integer($1); }
        |   INPUT               { $$ = Tomboy::AST::make_input(); }
        |   neg_value
;

neg_value:  MINUS value         { $$ = Tomboy::AST::make_operation(Tomboy::AST::make_integer(0), $2, Tomboy::Operations::SUB); }
;
%%


namespace yy {
parser::token_type yylex(parser::semantic_type *yylval, Driver *driver)
{
    return driver->yylex(yylval);
}

void parser::error(const std::string&)
{
    std::cerr << "Parsing error on " << driver->get_line() << ":" << driver->get_column() << std::endl;
}
}
