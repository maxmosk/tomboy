#ifndef INODE_HPP_INCLUDED
#define INODE_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "except.hpp"

namespace Tomboy
{
using Int = std::int32_t;

class SymTab
{
    using ScopeTable = std::unordered_map<std::string, Int>;
    std::vector<ScopeTable> tables_;

public:
    void push_scope()
    {
        tables_.push_back(ScopeTable{});
    }

    void pop_scope()
    {
        tables_.pop_back();
    }

    void assign(const std::string &id, Int value)
    {
        auto end_it = tables_.rend();
        for (auto it = tables_.rbegin(); it != end_it; ++it)
        {
            auto founded = (*it).find(id);
            if (founded != (*it).end())
            {
                (*founded).second = value;
                return;
            }
        }

        if (!tables_.empty())
        {
            (*tables_.rbegin())[id] = value;
        }
        else
        {
            throw GenericError{"Empty variable table", __LINE__};
        }
    }

    Int value_of(const std::string &id) const
    {
        auto end_it = tables_.crend();
        for (auto it = tables_.crbegin(); it != end_it; ++it)
        {
            auto founded = (*it).find(id);
            if (founded != (*it).end())
            {
                return (*founded).second;
            }
        }

        throw UndefinedVariable(__LINE__);
    }

};

enum class Operations
{
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    MOD = 5,

    NEG = 6,
    NOT = 7,

    EQ  = 8,
    NEQ = 9,
    GT  = 10,
    LT  = 11,
    GE  = 12,
    LE  = 13,

    AND = 14,
    OR  = 15
};

namespace AST
{
class INode;

using pINode = INode *;

class INode
{
protected:
    pINode left_;
    pINode right_;
    pINode third_;

public:
    virtual std::optional<Int> eval(SymTab &table) const = 0;
    virtual void dump(std::ostream &os) const = 0;

    INode(pINode left, pINode right, pINode third = nullptr)
        : left_{left}, right_{right}, third_{third} {}
    INode(const INode &) = delete;
    INode(const INode &&) = delete;
    INode &operator=(const INode &) = delete;
    INode &operator=(const INode &&) = delete;

    virtual ~INode() = default;

    pINode get_left()
    {
        return left_;
    }

    pINode get_right()
    {
        return right_;
    }

    pINode get_third()
    {
        return third_;
    }
};

class Scope : public INode
{
public:
    Scope(pINode next) : INode(next, nullptr)
    {
    }

    std::optional<Int> eval(SymTab &table) const override
    {
        if (left_ != nullptr)
        {
            table.push_scope();
            left_->eval(table);
            table.pop_scope();
        }
        return std::nullopt;
    }

    void dump(std::ostream &os) const override
    {
        os << "node" << this << "[" << "label=Scope" << "];\n";
        if (left_ != nullptr)
        {
            os << "node" << this << "--" << "node" << left_ << ";\n";
            left_->dump(os);
        }
    }
};
} // namespace AST
} // namespace Tomboy

#endif // INODE_HPP_INCLUDED
