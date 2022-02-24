#include <optional>
#include "stretch/Arithmetique.hpp"

namespace pe = tao::pegtl;

struct Retour {
    std::optional< std::vector<Variable> > valeurs;
};

template <typename Rule, typename Noeud>
Retour evaluer(std::unique_ptr<Noeud>& noeud);

template <typename Noeud>
Retour evaluer<stretch::grammaire>(std::unique_ptr<Noeud>& noeud) {
    if(noeud->type == pe::demangle<stretch::operation>()) {
        evaluer<stretch::operation>(noeud);
    } else if(noeud->type == pe:demangle<stretch::condition>()) {
        evaluer<stretch::condition>(noeud);
    }
}

template <typename Noeud>
Retour evaluer<stretch::operation>(std::unique_ptr<Noeud>& noeud) {
    return noeud->children.empty() ? std::stoi(noeud->string()) : stretch::arithmetique::operation<int>(noeud->type, evaluer(noeud->children[0]), evaluer(noeud->children[1]));
}

template <typename Noeud>
Retour evaluer<stretch::condition>(std::unique_ptr<Noeud>& noeud) {
    if(evaluer(noeud->children[0]) != 0) 
        evaluer(noeud->children[1]);
}
