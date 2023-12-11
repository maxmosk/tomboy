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
            throw TomboyError{"empty symtab stack", __LINE__};
        }
    }

    std::optional<Int> value_of(const std::string &id) const
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

        return std::nullopt;
    }
};

enum class Operations
{
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,

    NEG = 5,
    NOT = 6,

    EQ  = 7,
    NEQ = 8,
    GT  = 9,
    LT  = 10,
    GE  = 11,
    LE  = 12
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

    INode(pINode left, pINode right) : left_{left}, right_{right} {}
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

pINode make_integer(Int value);

pINode make_operation(pINode left, pINode right, Operations op);

pINode make_print(pINode expr);

pINode make_compound(pINode first, pINode second);

pINode make_if(pINode cond, pINode then_branch, pINode else_branch = nullptr);

pINode make_while(pINode cond, pINode body);

pINode make_variable(std::string *identifier);

pINode make_assign(pINode expr, std::string *identifier);

pINode make_input();

pINode make_unary(pINode value, Operations op);
} // namespace AST
} // namespace Tomboy

#endif // INODE_HPP_INCLUDED
