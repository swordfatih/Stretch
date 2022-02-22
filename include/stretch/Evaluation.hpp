#include "stretch/Arithmetique.hpp"

namespace pe = tao::pegtl;

template <typename Rule, typename Noeud>
void evaluer(std::unique_ptr<Noeud>& noeud) {
    if(noeud->type == pe::demangle<stretch::operation>()) {
        evaluer<stretch::operation>(noeud);
    } else if(noeud->type == pe:demangle<stretch::condition>()) {
        evaluer<stretch::condition>(noeud);
    }
}

template <typename Noeud>
int evaluer<stretch::operation>(std::unique_ptr<Noeud>& noeud) {
    return noeud->children.empty() ? std::stoi(noeud->string()) : stretch::arithmetique::operation<int>(noeud->type, evaluer(noeud->children[0]), evaluer(noeud->children[1]));
}

template <typename Noeud>
int evaluer<stretch::condition>(std::unique_ptr<Noeud>& noeud) {
    

    return evaluer(noeud->);
}
