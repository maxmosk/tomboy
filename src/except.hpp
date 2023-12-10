#ifndef EXCEPT_HPP_INCLUDED
#define EXCEPT_HPP_INCLUDED

#include <stdexcept>

namespace Tomboy
{
class TomboyError : public std::runtime_error
{
    std::size_t source_line_;
    std::size_t line_;
    std::size_t column_;
public:
    TomboyError(const char *info, std::size_t source_line, std::size_t line = 0, std::size_t column = 0)
            : runtime_error{info},
              source_line_{source_line},
              line_{line},
              column_{column_}
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
} // namespace Tomboy

#endif // EXCEPT_HPP_INCLUDED
