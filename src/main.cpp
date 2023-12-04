#include <cstdlib>
#include <memory>

#include <FlexLexer.h>

#include "driver.hpp"


int main()
{
    auto lexer = std::make_unique<yyFlexLexer>();
    yy::Driver driver(lexer.get());
    if (driver.parse() == 0)
    {
        std::cerr << "Execution failed. Your tomboy is sad." << std::endl;
        return EXIT_FAILURE;
    }

    auto program = std::unique_ptr<AST::INode>{driver.getAST()};
    AST::SymTab vars{};
    vars.push_scope();
    program->eval(vars);

    return EXIT_SUCCESS;
}
