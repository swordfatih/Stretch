#include "stretch/Arithmetique.hpp"

namespace stretch {

template <typename Noeud>
int evaluer(std::unique_ptr<Noeud>& noeud) {
    return noeud->children.empty() ? std::stoi(noeud->string()) : stretch::arithmetique::operation<int>(noeud->type, evaluer(noeud->children[0]), evaluer(noeud->children[1]));
}

} // namespace stretch