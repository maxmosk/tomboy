#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

#include <iostream>

#include "inode.hpp"


namespace AST
{
namespace Node
{
class Operation final : public INode
{
    Operations op_;
public:
    Operation(pINode left, pINode right, Operations op) : INode{left, right}, op_{op} {}

    virtual std::optional<Int> eval(SymTab &table) const override
    {
        Int res = 0;
        std::optional<Int> left_val = left_->eval(table);
        std::optional<Int> right_val = right_->eval(table);
        if ((left_val == std::nullopt) || (right_val == std::nullopt))
        {
            // TODO: exception
        }

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
                // TODO: exception
            }
            res = left_val.value() / right_val.value();
            break;
        
        default:
            // TODO: exception
            break;
        }

        return res;
    }
};

class Integer final : public INode
{
    Int value_;
public:
    Integer(Int value) : INode{nullptr, nullptr}, value_{value} {}

    virtual std::optional<Int> eval(SymTab &table) const override
    {
        return value_;
    }
};

class Print final : public INode
{
    Int value_;
public:
    Print(pINode left) : INode{left, nullptr} {}

    virtual std::optional<Int> eval(SymTab &table) const override
    {
        std::optional<Int> value = left_->eval(table);
        if (value == std::nullopt)
        {
            // TODO: exception
        }

        std::cout << value.value() << std::endl;

        return std::nullopt;
    }
};

class Compound final : public INode
{
public:
    Compound(pINode left, pINode right) : INode{left, right} {}

    virtual std::optional<Int> eval(SymTab &table) const override
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
};

class If final : public INode
{
public:
    If(pINode cond, pINode right) : INode{cond, right} {}

    virtual std::optional<Int> eval(SymTab &table) const override
    {
        auto cond = left_->eval(table);

        if (cond == std::nullopt)
        {
            // TODO: exception
        }

        if (cond != 0)
        {
            table.push_scope();
            right_->eval(table);
            table.pop_scope();
        }

        return std::nullopt;
    }
};

class While final : public INode
{
public:
    While(pINode cond, pINode right) : INode{cond, right} {}

    virtual std::optional<Int> eval(SymTab &table) const override
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
            // TODO: exception
        }

        return std::nullopt;
    }
};

class Variable final : public INode
{
    std::string identifier_;
public:
    Variable(std::string &identifier) : INode{nullptr, nullptr}, identifier_{identifier} {}

    virtual std::optional<Int> eval(SymTab &table) const override
    {
        return table.value_of(identifier_);
    }
};

class Assign final : public INode
{
    std::string identifier_;
public:
    Assign(pINode left, std::string &identifier) : INode{left, nullptr}, identifier_{identifier} {}

    virtual std::optional<Int> eval(SymTab &table) const override
    {
        auto value = left_->eval(table);
        if (value == std::nullopt)
        {
            // TODO: exception
        }

        table.assign(identifier_, value.value());

        return std::nullopt;
    }
};
} // namespace Node
} // namespace AST

#endif // INODE_HPP_INCLUDED
