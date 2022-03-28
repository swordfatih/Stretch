/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <tao/pegtl/demangle.hpp>

#include "stretch/Variable.hpp"
#include "stretch/Fonction.hpp"
#include "stretch/Operations.hpp"

namespace pe = tao::pegtl;

namespace stretch {

/////////////////////////////////////////////////
template <typename Noeud>
Variable evaluer(std::unique_ptr<Noeud>& noeud, Scope& scope) 
{
    // appel de fonction
    if(noeud->template is_type< appel >()) {
        if(!noeud->children.front()->template is_type< variable >())
            throw std::runtime_error("Le nom de la fonction n'est pas un identificateur valide.");

        Tableau valeurs;
        for(size_t i = 1; i < noeud->children.size(); ++i)
            valeurs.push_back(evaluer(noeud->children[i], scope));

        Tableau&& retour = Fonction::invoquer(scope, noeud->children.front()->string(), valeurs);
        
        return retour.empty() ? Variable() : (retour.size() == 1 ? retour[0] : Variable(std::move(retour)));
    }

    // variable
    if(noeud->template is_type< variable >()) {
        return scope.lire(noeud->string());
    }   

    // valeur
    if(noeud->children.empty()) 
        return Variable(noeud);

    // tableau
    if(noeud->template is_type< tableau >()) {
        Tableau tableau;

        for(auto& fils : noeud->children)
            tableau.push_back(evaluer(fils, scope));

        return Variable(std::move(tableau));
    }

    // operation unaire
    if(noeud->children.size() == 1)
        return stretch::operer(noeud->type, evaluer(noeud->children[0], scope), Variable());

    // operation binaire
    return stretch::operer(noeud->type, evaluer(noeud->children[0], scope), evaluer(noeud->children[1], scope));
}

} // namespace stretch