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

pINode make_print(pINode left)
{
    return new Node::Print{left};
}

pINode make_compound(pINode left, pINode right)
{
    return new Node::Compound{left, right};
}

pINode make_if(pINode left, pINode right)
{
    return new Node::If{left, right};
}

pINode make_while(pINode left, pINode right)
{
    return new Node::While{left, right};
}

pINode make_variable(std::string *identifier)
{
    if (identifier == nullptr)
    {
        throw TomboyError{"nullptr to variable identifier", __LINE__};
    }

    return new Node::Variable{*identifier};
}

pINode make_assign(pINode left, std::string *identifier)
{
    if (identifier == nullptr)
    {
        throw TomboyError{"nullptr to variable identifier", __LINE__};
    }

    return new Node::Assign{left, *identifier};
}

pINode make_input()
{
    return new Node::Input{};
}
} // namespace AST
} // namespace Tomboy
