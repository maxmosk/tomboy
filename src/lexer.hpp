#ifndef LEXER_HPP_INCLUDED
#define LEXER_HPP_INCLUDED

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include <iostream>

class TomboyLexer : public yyFlexLexer {
    std::size_t column_ = 1;
    std::size_t next_column_ = 1;
    std::size_t line_ = 1;
public:
    TomboyLexer(std::istream *inp) : yyFlexLexer{inp} {}

    void process_newline()
    {
        column_ = 0;
        next_column_ = 1;
        ++line_;
    }

    void token_action()
    {
        column_ = next_column_;
        next_column_ += yyleng;
    }

    std::size_t get_column()
    {
        return column_;
    }

    std::size_t get_line()
    {
        return line_;
    }

    int yylex() override;
};

#endif // LEXER_HPP_INCLUDED
