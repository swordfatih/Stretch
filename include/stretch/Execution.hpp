#ifndef EXECUTION_HPP
#define EXECUTION_HPP

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <stdexcept>

#include "stretch/Evaluation.hpp"
 
/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
Tableau executer_internal(std::unique_ptr<pe::Noeud>& noeud, Scope& scope);

/////////////////////////////////////////////////
Tableau executer(std::unique_ptr<pe::Noeud>& noeud, Scope& scope)
{
    try 
    {
        return executer_internal(noeud, scope);
    } 
    catch(exception::Runtime& e) 
    {
        std::cerr << e.what() << std::endl;
        return {};
    } 
}

/////////////////////////////////////////////////
Tableau executer_internal(std::unique_ptr<pe::Noeud>& noeud, Scope& scope) 
{
    /////////////////////////////////////////////////
    if(noeud->is_root() || noeud->template is_type< bloc< mot::fin > >() || noeud->template is_type< bloc< mot::fin, mot::sinon > >())    
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
        if(noeud->children.size() % 2 != 0)
            throw stretch::exception::Runtime(noeud->begin(), "Il faut le meme nombre de variable et de valeurs dans une assignation.", noeud->string());

        size_t mid = noeud->children.size() / 2;

        for(size_t i = 0; i < mid; ++i) 
        {
            if(!(noeud->children[i]->template is_type< variable >()))
                throw stretch::exception::Runtime(noeud->children[i]->begin(), "Ce n'est pas une variable, verifiez qu'il y a le meme nombre de variables a gauche et de valeurs a droite.", noeud->children[i]->string());
            
            scope.assigner(noeud->children[i], evaluer(noeud->children[mid + i], scope));
        }
    } 
    /////////////////////////////////////////////////
    else if(noeud->template is_type< condition >()) // si 5 == 5 alors bloc fin
    {
        for(auto i = 0; i < noeud->children.size() - 1; i += 2) {
            Variable&& resultat = evaluer(noeud->children[i], scope);
        
            if(resultat.get_nature() != Nature::Booleen) 
                throw stretch::exception::Runtime(noeud->children[i]->begin(), "La condition n'est pas un booléen.", noeud->children[i]->string());

            if(std::get<bool>(resultat.get_valeur()) == true)
                return executer(noeud->children[i + 1], scope);
        }
        
        if(noeud->children.size() % 2 != 0) // vérifier l'existence d'un sinon final
            return executer(noeud->children.back(), scope);
    } 
    /////////////////////////////////////////////////
    else if(noeud->template is_type< entree >())
    {
        std::string in;
        std::getline(std::cin, in);

        scope.assigner(noeud->children.front()->string(), Variable::parse(in));
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< sortie >())
    {
        for(int i = 0; i < noeud->children.size(); i++) {
            if(i != 0)
                std::cout << " ";

            std::cout << evaluer(noeud->children[i], scope).to_string() << std::flush;
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< repeter >())
    {
        Variable compteur = evaluer(noeud->children.front(), scope);

        if(compteur.get_nature() != Nature::Reel) 
            throw stretch::exception::Runtime(noeud->children.front()->begin(), "Le compteur de la boucle n'est pas un entier.", noeud->children.front()->string());

        BigDecimal n = std::get<BigDecimal>(compteur.get_valeur());
        n.round(1);
 
        for(BigDecimal i = 1; i < n + 1; i += 1) 
        {
            auto& ranger = noeud->children[1];

            if(ranger->template is_type< variable >()) 
                scope.assigner(ranger, Variable(i));

            Tableau retour; 
            
            try 
            {
                retour = executer(noeud->children.back(), scope);
            } 
            catch(exception::Boucle& e) 
            {
                if(e.get_type() == exception::Boucle::Type::Arreter)
                    break;
                else if(e.get_type() == exception::Boucle::Type::Continuer)
                    continue;
            }

            if(!retour.empty()) // déléguer la valeur de retour à la fonction au dessus s'il y en
                return retour;
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< tant_que >())
    {
        // on récupère la condition elle-même puis on l'évalue, elle renvoie une Variable à laquelle on va récupérer le type utilisé dans le variant, ici c'est un booléen.
        Variable condition = evaluer(noeud->children.front(), scope).get_valeur();
        
        if(condition.get_nature() != Nature::Booleen) 
            throw stretch::exception::Runtime(noeud->children.front()->begin(), "La condition de la boucle n'est pas un booléen.", noeud->children.front()->string());

        bool resultat = std::get<bool>(condition.get_valeur());

        while(resultat) 
        {
            Tableau retour;
            
            try {
                retour = executer(noeud->children.back(), scope);
            } 
            catch(exception::Boucle& e) 
            {
                if(e.get_type() == exception::Boucle::Type::Arreter)
                    break;
                else if(e.get_type() == exception::Boucle::Type::Continuer)
                    continue;
            }
            
            if(!retour.empty()) 
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
            throw stretch::exception::Runtime(noeud->children[1]->begin(), "La variable n'est pas un tableau.", noeud->children[1]->string());

        Tableau tab = std::get<Tableau>(tableau.get_valeur());
        for(auto& v: tab) 
        {
            scope.assigner(element, v);

            Tableau retour;

            try {
                retour = executer(noeud->children.back(), scope);
            } 
            catch(exception::Boucle& e) 
            {
                if(e.get_type() == exception::Boucle::Type::Arreter)
                    break;
                else if(e.get_type() == exception::Boucle::Type::Continuer)
                    continue;
            }    

            if(!retour.empty()) 
                return retour; 
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< mot::arreter >())
    {
        throw exception::Boucle(exception::Boucle::Type::Arreter, noeud->begin());
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< mot::continuer >())
    {
        throw exception::Boucle(exception::Boucle::Type::Continuer, noeud->begin());
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type< definition >())
    {
        if(Fonction::existe(noeud->children.front()->string()))
            throw stretch::exception::Runtime(noeud->children.front()->begin(), "La fonction existe deja.", noeud->children.front()->string());

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
        throw exception::Quitter(noeud->begin(), "Arrêt du programme");
    }

    return {};
}

} // namespace stretch

#endif // EXECUTION_HPP