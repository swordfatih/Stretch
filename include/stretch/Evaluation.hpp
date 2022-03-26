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
Variable evaluer(std::unique_ptr<Noeud>& noeud, Fonction& fonction) 
{
    // appel de fonction
    if(noeud->template is_type< appel >()) {
        if(!noeud->children.front()->template is_type< variable >())
            throw std::runtime_error("Le nom de la fonction n'est pas un identificateur valide.");

        Tableau valeurs;
        for(size_t i = 1; i < noeud->children.size(); ++i)
            valeurs.push_back(evaluer(noeud->children[i], fonction));

        Tableau&& retour = Fonction::invoquer(noeud->children.front()->string(), valeurs);
        
        return retour.empty() ?  Variable() : retour[0];
    }

    // variable
    if(noeud->template is_type< variable >()) {
        return fonction.get_scope().lire(noeud->string());
    }   

    // valeur
    if(noeud->children.empty() || noeud->template is_type< tableau >()) 
        return Variable(noeud);

    // operation unaire
    if(noeud->children.size() == 1)
        return stretch::operer(noeud->type, evaluer(noeud->children[0], fonction), Variable());

    // operation binaire
    return stretch::operer(noeud->type, evaluer(noeud->children[0], fonction), evaluer(noeud->children[1], fonction));
}

} // namespace stretch