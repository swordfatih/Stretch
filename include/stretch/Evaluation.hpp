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
            throw exception::Runtime(noeud->children.front()->begin(), "La fonction n'existe pas (verifie que t'as bien mis le bon nom, les majuscules comptent)", nom);

        Fonction& fonction = Fonction::recuperer(nom);
        if(valeurs.size() != fonction.get_parametres().size() || valeurs.empty() && !fonction.get_parametres().empty()) 
            throw exception::Runtime(noeud->begin(), "La fonction n'a pas le bon nombre de parametres", noeud->string());

        Tableau&& retour = Fonction::invoquer(scope, nom, valeurs);
        
        return retour.empty() ? Variable() : (retour.size() == 1 ? retour[0] : Variable(std::move(retour)));
    }

    // variable
    if(noeud->template is_type< variable >()) {
        if(!scope.existe(noeud->string()))
            throw exception::Runtime(noeud->begin(), "La variable est inconnue (verifie que t'as bien mis le bon nom ou que t'y as acces depuis ce bloc)", noeud->string());

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

    // operation
    std::string_view operation = noeud->type;
    Variable first = evaluer(noeud->children.front(), scope);
    Variable second = noeud->children.size() == 1 ? Variable() : evaluer(noeud->children.back(), scope);

    // l'opération n'existe pas
    if(arithmetique::operations.count(operation) == 0) 
    {
        throw stretch::exception::Runtime(noeud->begin(), "L'opération n'existe pas", noeud->string());
        return Variable();
    } 

    // l'opération n'est pas supportée pour les types
    if(arithmetique::operations[operation].count(std::make_pair(first.get_nature(), second.get_nature())) == 0) 
    {
        throw stretch::exception::Runtime(noeud->begin(), "L'opération n'est pas supporté pour ces types (" + Variable::type_tos(first.get_nature()) + " et " + Variable::type_tos(second.get_nature()) + ")", noeud->string());
        return Variable();
    }

    return arithmetique::operations[operation][std::make_pair(first.get_nature(), second.get_nature())](first, second);
}

} // namespace stretch

#endif // EVALUATION_HPP