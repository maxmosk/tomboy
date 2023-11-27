#include <cstdlib>
#include <memory>

#include "FlexLexer.h"

#include "driver.hpp"

int main()
{
    std::unique_ptr<yyFlexLexer> lexer{new yyFlexLexer};
    yy::Driver driver(lexer.get());
    if (driver.parse() == 0)
    {
        std::cerr << "Execution failed. Your tomboy is sad." << std::endl;
        return EXIT_FAILURE;
    }
    
    AST::pINode program = driver.getAST();
    program->eval();

    return EXIT_SUCCESS;
}
