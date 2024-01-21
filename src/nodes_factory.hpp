#ifndef NODES_FACTORY_HPP_INCLUDED
#define NODES_FACTORY_HPP_INCLUDED

#include "inode.hpp"


namespace Tomboy
{
namespace AST
{
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

pINode make_logical(pINode left, pINode right, Operations op);
} // namespace AST
} // namespace Tomboy

#endif // NODES_FACTORY_HPP_INCLUDED
