#ifndef INODE_HPP_INCLUDED
#define INODE_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <optional>


namespace AST
{
class INode;

using pINode = std::shared_ptr<INode>;
using Int = std::int32_t;

enum class Operations
{
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4
};

class INode
{
protected:
    pINode left_;
    pINode right_;
public:
    virtual std::optional<Int> eval() const = 0;

    INode(pINode left, pINode right) : left_{left}, right_{right} {}
    INode(const INode &) = delete;
    INode(const INode &&) = delete;
    INode &operator=(const INode &) = delete;
    INode &operator=(const INode &&) = delete;

    virtual ~INode() = default;
};

pINode make_integer(Int value);

pINode make_operation(pINode left, pINode right, Operations op);

pINode make_print(pINode left);
} // namespace AST

#endif // INODE_HPP_INCLUDED
