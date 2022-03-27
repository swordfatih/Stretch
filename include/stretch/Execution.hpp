/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <stdexcept>

#include "stretch/Evaluation.hpp"
 
/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
Tableau executer(std::unique_ptr<Noeud>& noeud, Scope& scope) 
{
    /////////////////////////////////////////////////
    if(noeud->is_root() || noeud->template is_type< bloc >())    
    {
        for(auto& c: noeud->children) 
        {
            auto&& resultat = executer(c, scope);
            
            if(!resultat.empty())
                return std::move(resultat);
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< assignation >()) // a <- 5 + 5 + 5
    {
        scope.assigner(noeud->children.front(), evaluer(noeud->children.back(), scope));
    } 
    /////////////////////////////////////////////////
    else if(noeud->template is_type< condition >()) // si 5 == 5 alors bloc fin
    {
        for(auto i = 0; i < noeud->children.size() - 1; i += 2) {
            Variable&& resultat = evaluer(noeud->children[i], scope);
        
            if(resultat.get_nature() != Nature::Booleen) 
                throw std::runtime_error("La condition ne retourne pas un booléen.");

            if(std::get<bool>(resultat.get_valeur()) == true) {
                return executer(noeud->children[i + 1], scope);
            }
        }
        
        if(noeud->children.size() % 2 != 0) // vérifier l'existence d'un sinon final
            return executer(noeud->children.back(), scope);
    } 
    /////////////////////////////////////////////////
    else if(noeud->template is_type< repeter >())
    {
        Variable compteur = evaluer(noeud->children.front(), scope);

        if(compteur.get_nature() != Nature::Reel) 
            throw std::runtime_error("Le compteur de la boucle n'est pas un entier.");

        BigDecimal n = std::get<BigDecimal>(compteur.get_valeur());
        n.round(1);
 
        for(BigDecimal i = 1; i < n + 1; i += 1) 
        {
            auto& ranger = noeud->children[1];

            if(ranger->template is_type< variable >()) 
                scope.assigner(ranger, Variable(i));

            /* if(noeud->children.back()->children->template is_type< mot::arreter >()) {
                std::cout << "[DEBUG] La boucle est terminée." << std::endl;
                break;
            } */

            Tableau&& retour = executer(noeud->children.back(), scope);

            if(!retour.empty()) // déléguer la valeur de retour à la fonction au dessus s'il y en
                return retour;
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< tant_que >())
    {
        // on récupère la condition elle-même puis on l'évalue, elle renvoie une Variable à laquelle on va récupérer le type utilisé dans le variant, ici c'est un booléen.
        bool resultat = std::get<bool>(evaluer(noeud->children.front(), scope).get_valeur());

        while(resultat) 
        {
            /* if(noeud->children.back()->children->template is_type< mot::arreter >()) {
                 std::cout << "[DEBUG] La boucle est terminée." << std::endl;
                break;
            } */

            //on execute ce qu'il y a à l'intérieur 
            Tableau&& retour = executer(noeud->children.back(), scope);

            if(!retour.empty()) // déléguer la valeur de retour à la fonction au dessus s'il y en
                return retour;

            resultat = std::get<bool>(evaluer(noeud->children.front(), scope).get_valeur());
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< pour_chaque >())
    {
        auto& element = noeud->children.front();
        Variable tableau = evaluer(noeud->children[1], scope);

        if(tableau.get_nature() != Nature::Tableau) 
            throw std::runtime_error("Un tableau est requis pour la boucle pour chaque.");

        if(!(element->template is_type< variable >()))
            throw std::runtime_error(element->string() + " n'est pas un nom de variable valide.");

        Tableau tab = std::get<Tableau>(tableau.get_valeur());
        for(auto& v: tab) 
        {
            scope.assigner(element, v);

            /* if(noeud->children.back()->children->template is_type< mot::arreter >()) {
                std::cout << "[DEBUG] La boucle est terminée." << std::endl;
                break;
            } */ 
                
            executer(noeud->children.back(), scope);
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< mot::arreter >())
    {
        /* if(!noeud->parent->template is_type< repeter >() || !noeud->parent->template is_type< tant_que >()
        || !noeud->parent->template is_type< pour_chaque >()) {
            throw std::runtime_error("Vous n'êtes pas dans une boucle !");  
        } */

        std::cout << "[DEBUG] La boucle est terminée mais chelou pcq normalement la bonne vérif se fait dans les else if des boucles" << std::endl;
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< definition >())
    {
        std::vector<std::string> params;

        // puis on récupère tous les noms de parametres (tous les fils de stretch::parametres) jusqu'à atteindre le fond 
        if(noeud->children[1]->template is_type< parametres >())
        {
            for(int i = 0; i < noeud->children[1]->children.size(); ++i)
                params.push_back(noeud->children[1]->children[i]->string());
        }

        // noeud->children.front() = nom de la fonction
        Fonction::enregistrer(noeud->children.front()->string(), Fonction(noeud->children.back(), params));
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< retour >()) 
    {
        Tableau valeurs;
        
        for(auto& valeur: noeud->children)
            valeurs.push_back(evaluer(valeur, scope));

        return std::move(valeurs);
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< mot::quitter >()) 
    {
        throw exception::QuitterException("Arrêt du programme");
    }

    return {};
}

} // namespace stretch