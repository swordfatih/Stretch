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
Variable evaluer(std::unique_ptr<pe::Noeud>& noeud, Scope& scope);

/////////////////////////////////////////////////
inline Variable evaluer_fonction(std::unique_ptr<pe::Noeud>& noeud, Scope& scope)
{
    Tableau valeurs;

    for(size_t i = 1; i < noeud->children.size(); ++i)
        valeurs.push_back(evaluer(noeud->children[i], scope));

    try 
    {
        Fonction::invoquer(scope, noeud->children.front()->string(), valeurs);
    }
    catch(const std::runtime_error& e)
    {
        throw stretch::exception::Runtime(noeud->begin(), e.what(), noeud->string());
    }
    catch(exception::Retour& e) // S'il y a une valeur de retour
    {
        Tableau& retour = e.get_retour();
        return retour.size() == 1 ? retour[0] : Variable(retour);
    }
    
    return Variable();
}

/////////////////////////////////////////////////
inline Variable evaluer(std::unique_ptr<pe::Noeud>& noeud, Scope& scope) 
{
    // appel de fonction
    if(noeud->template is_type< appel >()) 
    {
        return evaluer_fonction(noeud, scope);
    }

    // variable
    if(noeud->template is_type< variable >()) 
    {
        if(!scope.existe(noeud->string()))
            throw exception::Runtime(noeud->begin(), "La variable est inconnue (vérifie que t'as bien mis le bon nom et que t'y as acces depuis ce bloc)", noeud->string());

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