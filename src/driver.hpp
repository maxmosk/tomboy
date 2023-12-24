#ifndef DRIVER_HPP_INCLUDED
#define DRIVER_HPP_INCLUDED

#include <string>
#include <cstring>

#include "lexer.hpp"

#include "paracl.tab.hh"

namespace yy
{
class Driver
{
    Tomboy::AST::pINode root_;
    TomboyLexer *plex_;

public:
    Driver(TomboyLexer *plex) : plex_(plex) {}

    parser::token_type yylex(parser::semantic_type *yylval)
    {
        parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());

        if (tt == parser::token_type::INT_LITERAL)
        {
            yylval->as<int>() = std::stoi(plex_->YYText());
        }
        else if (tt == parser::token_type::ID)
        {
            yylval->as<std::string *>() = new std::string{plex_->YYText(),
                    static_cast<std::size_t>(plex_->YYLeng())};
        }
        else if (tt == parser::token_type::ERROR)
        {
            std::cerr << "Lexing error on line " << plex_->lineno() << ":" << std::endl;
            std::cerr << "    Unknown token ";
            std::cerr.write(plex_->YYText(), plex_->YYLeng());
            std::cerr << std::endl;
        }

        return tt;
    }

    bool parse() {
        return !parser(this).parse();
    }

    void setAST(Tomboy::AST::pINode root)
    {
        root_ = root;
    }

    std::size_t get_column()
    {
        return plex_->get_column();
    }

    std::size_t get_line()
    {
        return plex_->get_line();
    }

    Tomboy::AST::pINode getAST() const
    {
        return root_;
    }
};
} // namespace yy

#endif // DRIVER_HPP_INCLUDED
