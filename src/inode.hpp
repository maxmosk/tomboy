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

public:
    virtual std::optional<Int> eval(SymTab &table) const = 0;
    virtual void dump(std::ostream &os) const = 0;

    INode(pINode left, pINode right)
        : left_{left}, right_{right} {}
    INode(const INode &) = delete;
    INode(const INode &&) = delete;
    INode &operator=(const INode &) = delete;
    INode &operator=(const INode &&) = delete;

    virtual ~INode()
    {
        delete left_;
        delete right_;
    }
};
} // namespace AST
} // namespace Tomboy

#endif // INODE_HPP_INCLUDED
