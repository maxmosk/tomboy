#include "node.hpp"


namespace Tomboy
{
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

pINode make_print(pINode expr)
{
    return new Node::Print{expr};
}

pINode make_compound(pINode first, pINode second)
{
    return new Node::Compound{first, second};
}

pINode make_if(pINode cond, pINode then_branch, pINode else_branch)
{
    return new Node::If{cond, then_branch, else_branch};
}

pINode make_while(pINode cond, pINode body)
{
    return new Node::While{cond, body};
}

pINode make_variable(std::string *identifier)
{
    if (identifier == nullptr)
    {
        throw TomboyError{"nullptr to variable identifier", __LINE__};
    }

    return new Node::Variable{*identifier};
}

pINode make_assign(pINode expr, std::string *identifier)
{
    if (identifier == nullptr)
    {
        throw TomboyError{"nullptr to variable identifier", __LINE__};
    }

    return new Node::Assign{expr, *identifier};
}

pINode make_input()
{
    return new Node::Input{};
}

pINode make_unary(pINode expr, Operations op)
{
    return new Node::Unary{expr, op};
}

pINode make_logical(pINode left, pINode right, Operations op)
{
    return new Node::Logical{left, right, op};
}
} // namespace AST
} // namespace Tomboy
