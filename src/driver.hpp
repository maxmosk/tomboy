#ifndef DRIVER_HPP_INCLUDED
#define DRIVER_HPP_INCLUDED

#include <string>
#include <cstring>

#include <FlexLexer.h>

#include "paracl.tab.hh"

namespace yy
{

class Driver
{
    FlexLexer *plex_;

public:
    Driver(FlexLexer *plex) : plex_(plex) {}

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

        return tt;
    }

    bool parse() {
        return !parser(this).parse();
    }
};

} // namespace yy

#endif // DRIVER_HPP_INCLUDED
