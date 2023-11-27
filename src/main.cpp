#include <cstdlib>
#include <memory>

#include "FlexLexer.h"

#include "driver.hpp"

int main()
{
    FlexLexer *lexer = new yyFlexLexer;
    yy::Driver driver(lexer);
    std::cout << driver.parse() << std::endl;
    delete lexer;
    return EXIT_SUCCESS;
}
