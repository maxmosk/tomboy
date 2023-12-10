#include <cstdlib>
#include <fstream>
#include <memory>

#include <lexer.hpp>

#include "driver.hpp"


int main(int argc, char **argv) try
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
        return EXIT_FAILURE;
    }

    auto program = std::unique_ptr<Tomboy::AST::INode>{driver.getAST()};
    Tomboy::SymTab vars{};
    vars.push_scope();
    program->eval(vars);

    return EXIT_SUCCESS;
}
catch (const Tomboy::TomboyError &except)
{
    std::printf("TomboyError: \"%s\" on %zu:%zu (Tomboy source line %zu)\n",
            except.what(), except.line(), except.column(), except.source_line());
    return EXIT_FAILURE;
}
catch (...)
{
    std::printf("Something went wrong...\n");
    return EXIT_FAILURE;
}
