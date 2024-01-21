%require  "3.2"
%language "c++"
%skeleton "lalr1.cc"

%defines
%define api.value.type variant

%param {yy::Driver* driver}

%code requires
{
#include <string>

#include "nodes_factory.hpp"

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
    MOD
    NOT

    EQUAL
    IS_EQUAL
    NOT_EQUAL
    GREATER
    LESS
    GREATER_OR_EQUAL
    LESS_OR_EQUAL
    AND
    OR

    SCOLON
    INPUT
    IF
    THEN
    ELSE
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
%nterm<Tomboy::AST::pINode> print
%nterm<Tomboy::AST::pINode> expression
%nterm<Tomboy::AST::pINode> expression_compare
%nterm<Tomboy::AST::pINode> expression_equal
%nterm<Tomboy::AST::pINode> expression_eval
%nterm<Tomboy::AST::pINode> expression_and
%nterm<Tomboy::AST::pINode> expression_or
%nterm<Tomboy::AST::pINode> term
%nterm<Tomboy::AST::pINode> factor
%nterm<Tomboy::AST::pINode> value
%nterm<Tomboy::AST::pINode> unary
%nterm<Tomboy::AST::pINode> block

%start program

%left PLUS MINUS MULT DIV
%nonassoc THEN
%nonassoc ELSE

%%
program:    statements          { driver->setAST($1); }
        |   %empty              { driver->setAST(Tomboy::AST::make_scope(nullptr)); }
;

statements: statement statements { $$ = Tomboy::AST::make_scope(Tomboy::AST::make_compound($1, $2)); }
        |   statement
;

statement:  print SCOLON
        |   if
        |   while
        |   expression SCOLON
        |   SCOLON              { $$ = Tomboy::AST::make_compound(nullptr, nullptr); }
        |   block
;

block:      LEFT_BRACE statements RIGHT_BRACE { $$ = Tomboy::AST::make_scope($2); }
        |   LEFT_BRACE RIGHT_BRACE { $$ = Tomboy::AST::make_scope(nullptr); }

if:         IF LEFT_PARENTHESS expression RIGHT_PARENTHESS
                statement %prec THEN    { $$ = Tomboy::AST::make_if($3, Tomboy::AST::make_scope($5)); }
        |   IF LEFT_PARENTHESS expression RIGHT_PARENTHESS
                statement
            ELSE
                statement               { $$ = Tomboy::AST::make_if($3, Tomboy::AST::make_scope($5), Tomboy::AST::make_scope($7)); }

;

while:      WHILE LEFT_PARENTHESS expression RIGHT_PARENTHESS
                statement { $$ = Tomboy::AST::make_while($3, Tomboy::AST::make_scope($5)); }
;

print:      PRINT expression    { $$ = Tomboy::AST::make_print($2); }
;

expression: expression_or
        |   ID EQUAL expression {
                                    $$ = Tomboy::AST::make_assign($3, $1);
                                    delete $1;
                                }
;

expression_compare: expression_eval
        |   expression_compare GREATER expression_eval
                { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::GT); }
        |   expression_compare LESS expression_eval
                { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::LT); }
        |   expression_compare GREATER_OR_EQUAL expression_eval
                { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::GE); }
        |   expression_compare LESS_OR_EQUAL expression_eval
                { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::LE); }
;

expression_equal:   expression_compare
                |   expression_equal IS_EQUAL  expression_compare
                { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::EQ); }
                |   expression_equal NOT_EQUAL expression_compare
                { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::NEQ); }
;

expression_eval:    expression_eval PLUS  term { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::ADD); }
                |   expression_eval MINUS term { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::SUB); }
                |   term
;

expression_and:     expression_and AND expression_equal { $$ = Tomboy::AST::make_logical($1, $3, Tomboy::Operations::AND); }
                |   expression_equal
;

expression_or:      expression_or OR expression_and { $$ = Tomboy::AST::make_logical($1, $3, Tomboy::Operations::OR); }
                |   expression_and
;

term:       term MULT factor    { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::MUL); }
        |   term DIV  factor    { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::DIV); }
        |   term MOD  factor    { $$ = Tomboy::AST::make_operation($1, $3, Tomboy::Operations::MOD); }
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
        |   unary
;

unary:      MINUS value         { $$ = Tomboy::AST::make_unary($2, Tomboy::Operations::NEG); }
        |   NOT value           { $$ = Tomboy::AST::make_unary($2, Tomboy::Operations::NOT); }
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
