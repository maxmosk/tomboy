#include <cstdlib>
#include <fstream>
#include <memory>

#include <lexer.hpp>

#include "driver.hpp"


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: tomboy SOURCE" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream source(argv[1]);
    auto lexer = std::make_unique<TomboyLexer>(&source);
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
