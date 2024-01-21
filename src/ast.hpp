#ifndef AST_HPP_INCLUDED
#define AST_HPP_INCLUDED

#include <fstream>

#include "lexer.hpp"
#include "driver.hpp"
#include "inode.hpp"


namespace Tomboy
{
namespace AST
{
class AST
{
    pINode root_;
    static void delete_inode(pINode node)
    {
        if (node != nullptr)
        {
            delete_inode(node->get_left());
            delete_inode(node->get_right());
            delete_inode(node->get_third());
            delete node;
        }
    }

public:
    AST(std::ifstream &source)
    {
        auto lexer = std::make_unique<TomboyLexer>(&source);
        yy::Driver driver(lexer.get());

        if (driver.parse() == 0)
        {
            throw ParsingError(__LINE__);
        }

        root_ = driver.getAST();
    }

    void run()
    {
        SymTab vars{};
        vars.push_scope();
        root_->eval(vars);
    }

    void dump(std::ostream &os)
    {
        std::cout << "graph {\n";
        std::cout << "root--node" << root_ << ";\n";
        root_->dump(os);
        std::cout << "}\n";
    }

    ~AST()
    {
        delete_inode(root_);
    }
};
} // namespace AST
} // namespace Tomboy

#endif // AST_HPP_INCLUDED
