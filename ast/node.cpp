#include "node.hpp"

namespace AST
{
pINode make_integer(Int value)
{
    return std::make_shared<Node::Integer>(value);
}

pINode make_operation(pINode left, pINode right, Operations op)
{
    return std::make_shared<Node::Operation>(left, right, op);
}

pINode make_print(pINode left)
{
    return std::make_shared<Node::Print>(left);
}
} // namespace AST
