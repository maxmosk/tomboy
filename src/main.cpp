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
    if (!source.is_open())
    {
        std::cerr << "Cannot open file: " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    auto lexer = std::make_unique<TomboyLexer>(&source);
    yy::Driver driver(lexer.get());
    if (!driver.parse())
    {
        return EXIT_FAILURE;
    }

    auto program = std::unique_ptr<Tomboy::AST::INode>{driver.getAST()};
    Tomboy::SymTab vars{};
    vars.push_scope();
    program->eval(vars);

    return EXIT_SUCCESS;
}
catch (const Tomboy::GenericError &except)
{
    if ((except.column() != 0) && (except.line() != 0))
    {
        std::fprintf(stderr, "GenericError: \"%s\" on %zu:%zu (Tomboy source line %zu)\n",
            except.what(), except.line(), except.column(), except.source_line());
    }
    else
    {
        std::fprintf(stderr, "GenericError: \"%s\" (Tomboy source line %zu)\n",
            except.what(), except.source_line());
    }
    
    return EXIT_FAILURE;
}
catch (const std::runtime_error &except)
{
    std::fprintf(stderr, "std::runtime_error: \"%s\"", except.what());
    return EXIT_FAILURE;
}
catch (...)
{
    std::fprintf(stderr, "Something went wrong...\n"
                         "Implement better error reporting or have sex with debug!\n");
    return EXIT_FAILURE;
}
