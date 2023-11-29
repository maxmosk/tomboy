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

    virtual std::optional<Int> eval() const override
    {
        Int res = 0;
        std::optional<Int> left_val = left_->eval();
        std::optional<Int> right_val = right_->eval();
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

    virtual std::optional<Int> eval() const override
    {
        return value_;
    }
};

class Print final : public INode
{
    Int value_;
public:
    Print(pINode left) : INode{left, nullptr} {}

    virtual std::optional<Int> eval() const override
    {
        std::optional<Int> value = left_->eval();
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

    virtual std::optional<Int> eval() const override
    {
        if (left_ != nullptr)
        {
            left_->eval();
        }

        if (right_ != nullptr)
        {
            right_->eval();
        }

        return std::nullopt;
    }
};

class If final : public INode
{
public:
    If(pINode cond, pINode right) : INode{cond, right} {}

    virtual std::optional<Int> eval() const override
    {
        auto cond = left_->eval();

        if (cond == std::nullopt)
        {
            // TODO: exception
        }

        if (cond != 0)
        {
            right_->eval();
        }

        return std::nullopt;
    }
};

class While final : public INode
{
public:
    While(pINode cond, pINode right) : INode{cond, right} {}

    virtual std::optional<Int> eval() const override
    {
        std::optional<Int> cond = std::nullopt;

        while (((cond = left_->eval()) != std::nullopt) && (cond != 0))
        {
            right_->eval();
        }

        if (cond == std::nullopt)
        {
            // TODO: exception
        }

        return std::nullopt;
    }
};
} // namespace Node
} // namespace AST

#endif // INODE_HPP_INCLUDED
