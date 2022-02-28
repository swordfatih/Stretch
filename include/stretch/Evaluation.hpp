#include <optional>
#include "stretch/Arithmetique.hpp"
#include "stretch/Variable.hpp"

namespace pe = tao::pegtl;

namespace stretch {

struct Retour {
    std::optional< std::vector<Variable> > valeurs;
};

template <typename Noeud>
Retour evaluer(std::unique_ptr<Noeud>& noeud) {
    if(noeud->type == pe::demangle<stretch::operation>()) {
        // evaluer<stretch::operation>(noeud);
    } 
    // else if(noeud->type == pe::demangle<stretch::condition>()) {
    //     evaluer<stretch::condition>(noeud);
    // }
}

// template <typename Noeud>
// Retour evaluer<stretch::operation, Noeud>(std::unique_ptr<Noeud>& noeud) {
//     return noeud->children.empty() ? std::stoi(noeud->string()) : stretch::arithmetique::operation<int>(noeud->type, evaluer(noeud->children[0]), evaluer(noeud->children[1]));
// }

// template <typename Noeud>
// Retour evaluer<stretch::condition>(std::unique_ptr<Noeud>& noeud) {
//     if(evaluer(noeud->children[0]) != 0) 
//         evaluer(noeud->children[1]);
// }

template <typename T>
static std::map<std::string_view, std::function<T(const T)>> evaluations = {
    {
        pe::demangle<stretch::bloc>(), 
        [](const T noeud) {
            return T{};
        }
    },
    {
        pe::demangle<stretch::operation>(), 
        [](const T first, const T second) {
            return T{};
        }
    }
};

} // namespace stretch