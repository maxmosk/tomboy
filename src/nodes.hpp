#ifndef NODES_HPP_INCLUDED
#define NODES_HPP_INCLUDED

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
        std::optional<Int> left_val = left_->eval(table);
        std::optional<Int> right_val = right_->eval(table);

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

    virtual void dump(std::ostream &os) const
    {
        os << "node" << this << "[" << "label=Operation" << "];\n";
        if (left_ != nullptr)
        {
            os << "node" << this << "--" << "node" << left_ << ";\n";
            left_->dump(os);
        }
        if (right_ != nullptr)
        {
            os << "node" << this << "--" << "node" << right_ << ";\n";
            right_->dump(os);
        }
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

    virtual void dump(std::ostream &os) const
    {
        os << "node" << this << "[" << "label=Integer" << "];\n";
    }
};

class Print final : public INode
{
public:
    Print(pINode left)
        : INode{left, nullptr} {}

    std::optional<Int> eval(SymTab &table) const override try
    {
        std::optional<Int> value = left_->eval(table);

        std::cout << value.value() << std::endl;

        return std::nullopt;
    }
    catch (const std::bad_optional_access &except)
    {
        throw GenericError{"operand hasn't value", __LINE__};
    }

    virtual void dump(std::ostream &os) const
    {
        os << "node" << this << "[" << "label=Print" << "];\n";
        if (left_ != nullptr)
        {
            os << "node" << this << "--" << "node" << left_ << ";\n";
            left_->dump(os);
        }
    }
};

class Compound final : public INode
{
public:
    Compound(pINode left, pINode right)
        : INode{left, right} {}

    std::optional<Int> eval(SymTab &table) const override
    {
        if (left_ != nullptr)
        {
            left_->eval(table);
        }

        if (right_ != nullptr)
        {
            right_->eval(table);
        }

        return std::nullopt;
    }

    virtual void dump(std::ostream &os) const
    {
        os << "node" << this << "[" << "label=Compound" << "];\n";
        if (left_ != nullptr)
        {
            os << "node" << this << "--" << "node" << left_ << ";\n";
            left_->dump(os);
        }
        if (right_ != nullptr)
        {
            os << "node" << this << "--" << "node" << right_ << ";\n";
            right_->dump(os);
        }
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
            left_->eval(table);
            table.pop_scope();
        }
        else if (right_ != nullptr)
        {
            table.push_scope();
            right_->eval(table);
            table.pop_scope();
        }

        return std::nullopt;
    }
    catch (const std::bad_optional_access &except)
    {
        throw GenericError{"condition hasn't value", __LINE__};
    }

    virtual void dump(std::ostream &os) const
    {
        os << "node" << this << "[" << "label=If" << "];\n";
        if (cond_ != nullptr)
        {
            os << "node" << this << "--" << "node" << cond_ << ";\n";
            cond_->dump(os);
        }
        if (left_ != nullptr)
        {
            os << "node" << this << "--" << "node" << left_ << ";\n";
            left_->dump(os);
        }
        if (right_ != nullptr)
        {
            os << "node" << this << "--" << "node" << right_ << ";\n";
            right_->dump(os);
        }
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

        while (((cond = left_->eval(table)) != std::nullopt) && (cond != 0))
        {
            table.push_scope();
            right_->eval(table);
            table.pop_scope();
        }

        if (cond == std::nullopt)
        {
            throw GenericError{"condition hasn't value", __LINE__};
        }

        return std::nullopt;
    }

    virtual void dump(std::ostream &os) const
    {
        os << "node" << this << "[" << "label=While" << "];\n";
        if (left_ != nullptr)
        {
            os << "node" << this << "--" << "node" << left_ << ";\n";
            left_->dump(os);
        }
        if (right_ != nullptr)
        {
            os << "node" << this << "--" << "node" << right_ << ";\n";
            right_->dump(os);
        }
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

    virtual void dump(std::ostream &os) const
    {
        os << "node" << this << "[" << "label=Variable" << "];\n";
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
        auto value = left_->eval(table);

        table.assign(identifier_, value.value());

        return value;
    }
    catch (const std::bad_optional_access &except)
    {
        throw GenericError{"operand hasn't value", __LINE__};
    }

    virtual void dump(std::ostream &os) const
    {
        os << "node" << this << "[" << "label=Assign" << "];\n";
        if (left_ != nullptr)
        {
            os << "node" << this << "--" << "node" << left_ << ";\n";
            left_->dump(os);
        }
    }
};

class Input final : public INode
{
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

    virtual void dump(std::ostream &os) const
    {
        os << "node" << this << "[" << "label=Input" << "];\n";
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
        std::optional<Int> left_val = left_->eval(table);

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

    virtual void dump(std::ostream &os) const
    {
        os << "node" << this << "[" << "label=Unary" << "];\n";
        if (left_ != nullptr)
        {
            os << "node" << this << "--" << "node" << left_ << ";\n";
            left_->dump(os);
        }
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
            res = left_->eval(table).value() && right_->eval(table).value();
            break;

        case Operations::OR:
            res = left_->eval(table).value() || right_->eval(table).value();
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

    virtual void dump(std::ostream &os) const
    {
        os << "node" << this << "[" << "label=Logical" << "];\n";
        if (left_ != nullptr)
        {
            os << "node" << this << "--" << "node" << left_ << ";\n";
            left_->dump(os);
        }
        if (right_ != nullptr)
        {
            os << "node" << this << "--" << "node" << right_ << ";\n";
            right_->dump(os);
        }
    }
};
} // namespace Node
} // namespace AST
} // namespace Tomboy

#endif // NODES_HPP_INCLUDED
