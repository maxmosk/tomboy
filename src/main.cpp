#include <cstdlib>

#include "FlexLexer.h"

int main()
{
    FlexLexer *lexer = new yyFlexLexer;
    while(lexer->yylex() != 0)
        ;
    return EXIT_SUCCESS;
}
