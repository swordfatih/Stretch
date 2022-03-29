#ifndef EVALUATION_HPP
#define EVALUATION_HPP

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <tao/pegtl/demangle.hpp>

#include "stretch/Variable.hpp"
#include "stretch/Fonction.hpp"
#include "stretch/Operations.hpp"

/////////////////////////////////////////////////
namespace pe = tao::pegtl;

/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
inline Variable evaluer(std::unique_ptr<pe::Noeud>& noeud, Scope& scope) 
{
    // appel de fonction
    if(noeud->template is_type< appel >()) {
        Tableau valeurs;
        for(size_t i = 1; i < noeud->children.size(); ++i)
            valeurs.push_back(evaluer(noeud->children[i], scope));

        std::string nom = noeud->children.front()->string();
        if(!Fonction::existe(nom))
            throw exception::Runtime(noeud->children.front()->begin(), "La fonction n'existe pas.", nom);

        Fonction& fonction = Fonction::recuperer(nom);
        if(valeurs.size() != fonction.get_parametres().size() || valeurs.empty() && !fonction.get_parametres().empty()) 
            throw exception::Runtime(noeud->begin(), "La fonction n'a pas le bon nombre de paramètres.", noeud->string());

        Tableau&& retour = Fonction::invoquer(scope, nom, valeurs);
        
        return retour.empty() ? Variable() : (retour.size() == 1 ? retour[0] : Variable(std::move(retour)));
    }

    // variable
    if(noeud->template is_type< variable >()) {
        if(!scope.existe(noeud->string()))
            throw exception::Runtime(noeud->begin(), "La variable est inconnue.", noeud->string());

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

#endif // EVALUATION_HPP