#include <tao/pegtl/demangle.hpp>

#include "stretch/Variable.hpp"

namespace pe = tao::pegtl;

namespace stretch::arithmetique {

/////////////////////////////////////////////////
template <typename Noeud>
Variable evaluer(std::unique_ptr<Noeud>& noeud) 
{
    if(noeud->template is_type<stretch::appel_fonction>()) {
        if(!noeud->children.front()->template is_type< stretch::variable >())
            throw std::runtime_error("Le nom de la fonction n'est pas un identificateur valide.");

        Tableau&& retour = invoquer_fonction(noeud->children.front()->string(), noeud);
        
        if(retour.empty())
            return Variable();
            
        return retour[0];
    }

    // variable
    if(noeud->template is_type< stretch::variable >()) {
        if(variables.count(noeud->string()) == 0)
            return Variable();

        return variables.at(noeud->string());
    }   

    // valeur
    if(noeud->children.empty() || noeud->template is_type< stretch::tableau >()) 
        return Variable(noeud);

    // operation unaire
    if(noeud->children.size() == 1)
        return stretch::arithmetique::operation(noeud->type, evaluer(noeud->children[0]), Variable());

    // operation binaire
    return stretch::arithmetique::operation(noeud->type, evaluer(noeud->children[0]), evaluer(noeud->children[1]));
}

} // namespace stretch::arithmetique