#ifndef EXCEPT_HPP_INCLUDED
#define EXCEPT_HPP_INCLUDED

#include <stdexcept>

namespace Tomboy
{
class GenericError : public std::runtime_error
{
    std::size_t source_line_;
    std::size_t line_;
    std::size_t column_;
public:
    GenericError(const char *info, std::size_t source_line, std::size_t line = 0, std::size_t column = 0)
            : runtime_error{info},
              source_line_{source_line},
              line_{line},
              column_{column}
    {}

    std::size_t source_line() const
    {
        return source_line_;
    }

    std::size_t line() const
    {
        return line_;
    }

    std::size_t column() const
    {
        return column_;
    }
};

class UndefinedVariable : public GenericError
{
public:
    UndefinedVariable(std::size_t source_line, std::size_t line = 0, std::size_t column = 0)
            : GenericError{"Variable doesn't exist", source_line, line, column}
    {}
};

class ParsingError : public GenericError
{
public:
    ParsingError(std::size_t source_line, std::size_t line = 0, std::size_t column = 0)
            : GenericError{"Failed source code parsing", source_line, line, column}
    {}
};
} // namespace Tomboy

#endif // EXCEPT_HPP_INCLUDED
