#include "stretch/Arithmetique.hpp"

namespace pe = tao::pegtl;

template <typename T>
void print_tree(std::unique_ptr<T>& root)
{
    pe::parse_tree::print_dot(std::cout, *root);
}

template <typename Noeud>
int evaluer(std::unique_ptr<Noeud>& noeud) {
    return noeud->children.empty() ? std::stoi(noeud->string()) : stretch::arithmetique::operation<int>(noeud->type, evaluer(noeud->children[0]), evaluer(noeud->children[1]));
}
