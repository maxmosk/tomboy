#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

#include <iostream>

#include "inode.hpp"


namespace Tomboy
{
namespace AST
{
namespace Node
{
class Operation final : public INode
{
    Operations op_;
public:
    Operation(pINode left, pINode right, Operations op)
        : INode{left, right}, op_{op} {}

    std::optional<Int> eval(SymTab &table) const override try
    {
        Int res = 0;
        std::optional<Int> left_val = eval_left(table);
        std::optional<Int> right_val = eval_right(table);

        switch (op_)
        {
        case Operations::ADD:
            res = left_val.value() + right_val.value();
            break;

        case Operations::SUB:
            res = left_val.value() - right_val.value();
            break;

        case Operations::MUL:
            res = left_val.value() * right_val.value();
            break;

        case Operations::DIV:
            if (right_val.value() == 0)
            {
                throw GenericError{"division by zero", __LINE__};
            }
            res = left_val.value() / right_val.value();
            break;

        case Operations::MOD:
            if (right_val.value() == 0)
            {
                throw GenericError{"division by zero", __LINE__};
            }
            res = left_val.value() % right_val.value();
            break;

        case Operations::EQ:
            res = left_val.value() == right_val.value();
            break;
        
        case Operations::NEQ:
            res = left_val.value() != right_val.value();
            break;
        
        case Operations::GT:
            res = left_val.value() > right_val.value();
            break;

        case Operations::LT:
            res = left_val.value() < right_val.value();
            break;

        case Operations::GE:
            res = left_val.value() >= right_val.value();
            break;

        case Operations::LE:
            res = left_val.value() <= right_val.value();
            break;

        default:
            throw GenericError{"unknown operation", __LINE__};
            break;
        }

        return res;
    }
    catch (const std::bad_optional_access &except)
    {
        throw GenericError{"operand hasn't value", __LINE__};
    }
};

class Integer final : public INode
{
    Int value_;
public:
    Integer(Int value)
        : INode{nullptr, nullptr}, value_{value} {}

    std::optional<Int> eval(SymTab &table) const override
    {
        return value_;
    }
};

class Print final : public INode
{
    Int value_;
public:
    Print(pINode left)
        : INode{left, nullptr} {}

    std::optional<Int> eval(SymTab &table) const override try
    {
        std::optional<Int> value = eval_left(table);

        std::cout << value.value() << std::endl;

        return std::nullopt;
    }
    catch (const std::bad_optional_access &except)
    {
        throw GenericError{"operand hasn't value", __LINE__};
    }
};

class Compound final : public INode
{
public:
    Compound(pINode left, pINode right)
        : INode{left, right} {}

    std::optional<Int> eval(SymTab &table) const override
    {
        if (has_left())
        {
            eval_left(table);
        }

        if (has_right())
        {
            eval_right(table);
        }

        return std::nullopt;
    }
};

class If final : public INode
{
    pINode cond_;
public:
    If(pINode cond, pINode left, pINode right)
        : INode{left, right}, cond_{cond} {}

    std::optional<Int> eval(SymTab &table) const override try
    {
        auto cond = cond_->eval(table);

        if (cond != 0)
        {
            table.push_scope();
            eval_left(table);
            table.pop_scope();
        }
        else if (has_right())
        {
            table.push_scope();
            eval_right(table);
            table.pop_scope();
        }

        return std::nullopt;
    }
    catch (const std::bad_optional_access &except)
    {
        throw GenericError{"condition hasn't value", __LINE__};
    }

    ~If()
    {
        delete cond_;
    }
};

class While final : public INode
{
public:
    While(pINode cond, pINode right)
        : INode{cond, right} {}

    std::optional<Int> eval(SymTab &table) const override
    {
        std::optional<Int> cond = std::nullopt;

        while (((cond = eval_left(table)) != std::nullopt) && (cond != 0))
        {
            table.push_scope();
            eval_right(table);
            table.pop_scope();
        }

        if (cond == std::nullopt)
        {
            throw GenericError{"condition hasn't value", __LINE__};
        }

        return std::nullopt;
    }
};

class Variable final : public INode
{
    std::string identifier_;
public:
    Variable(std::string &identifier)
        : INode{nullptr, nullptr}, identifier_{identifier} {}

    std::optional<Int> eval(SymTab &table) const override try
    {
        return table.value_of(identifier_);
    }
    catch (const UndefinedVariable &except)
    {
        std::fprintf(stdout, "Undefined variable \"%s\" on %zu:%zu (Tomboy source line %zu)\n",
                identifier_.c_str(), static_cast<std::size_t>(0), static_cast<std::size_t>(0), except.source_line());
        throw UndefinedVariable(except.source_line(), 0, 0);
    }
};

class Assign final : public INode
{
    std::string identifier_;
public:
    Assign(pINode left, std::string &identifier)
        : INode{left, nullptr}, identifier_{identifier} {}

    std::optional<Int> eval(SymTab &table) const override try
    {
        auto value = eval_left(table);

        table.assign(identifier_, value.value());

        return value;
    }
    catch (const std::bad_optional_access &except)
    {
        throw GenericError{"operand hasn't value", __LINE__};
    }
};

class Input final : public INode
{
    std::string identifier_;
public:
    Input() : INode{nullptr, nullptr} {}

    std::optional<Int> eval(SymTab &table) const override
    {
        Int value;
        std::cin >> value;
        if (std::cin.fail())
        {
            throw GenericError{"std::cin failed to input", __LINE__};
        }
        return value;
    }
};

class Unary final : public INode
{
    Operations op_;
public:
    Unary(pINode expr, Operations op)
        : INode{expr, nullptr}, op_{op} {}

    std::optional<Int> eval(SymTab &table) const override try
    {
        Int res = 0;
        std::optional<Int> left_val = eval_left(table);

        switch (op_)
        {
        case Operations::NEG:
            res = -left_val.value();
            break;

        case Operations::NOT:
            res = !left_val.value();
            break;

        default:
            throw GenericError{"unknown operation", __LINE__};
            break;
        }

        return res;
    }
    catch (const std::bad_optional_access &except)
    {
        throw GenericError{"operand hasn't value", __LINE__};
    }
};

class Logical final : public INode
{
    Operations op_;
public:
    Logical(pINode left, pINode right, Operations op)
        : INode{left, right}, op_{op} {}

    std::optional<Int> eval(SymTab &table) const override try
    {
        Int res = 0;

        switch (op_)
        {
        case Operations::AND:
            res = eval_left(table).value() && eval_right(table).value();
            break;

        case Operations::OR:
            res = eval_left(table).value() || eval_right(table).value();
            break;

        default:
            throw GenericError{"unknown operation", __LINE__};
            break;
        }

        return res;
    }
    catch (const std::bad_optional_access &except)
    {
        throw GenericError{"operand hasn't value", __LINE__};
    }
};
} // namespace Node
} // namespace AST
} // namespace Tomboy

#endif // NODE_HPP_INCLUDED
