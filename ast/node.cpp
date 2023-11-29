#include "node.hpp"


namespace AST
{
pINode make_integer(Int value)
{
    return new Node::Integer{value};
}

pINode make_operation(pINode left, pINode right, Operations op)
{
    return new Node::Operation{left, right, op};
}

pINode make_print(pINode left)
{
    return new Node::Print{left};
}

pINode make_compound(pINode left, pINode right)
{
    return new Node::Compound{left, right};
}
} // namespace AST
