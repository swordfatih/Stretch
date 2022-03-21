#include "stretch/Arithmetique.hpp"
#include "stretch/Bloc.hpp"

#include <algorithm>
#include <stdexcept>
 
namespace stretch {
 
/////////////////////////////////////////////////
static std::map<std::string_view, Variable> variables;

template <typename T>
static std::map<std::string_view, Bloc<T>> definitions_fonctions;

/////////////////////////////////////////////////
template <typename Noeud>
void assigner(std::unique_ptr<Noeud>& variable, Variable valeur) 
{
    variables[variable->string()] = valeur;
    // std::cout << "Affectation de " << variables[variable->string()].to_string() << " dans la variable " <<  variable->string() << std::endl;
}
 
/////////////////////////////////////////////////
template <typename Noeud>
Variable evaluer(std::unique_ptr<Noeud>& noeud) 
{
    if(noeud->children.empty()) 
        return Variable(noeud);

    // if(noeud->children.size() == 1)
    //     return stretch::arithmetique::operation(noeud->type, evaluer(noeud->children[0]), Variable());

    return stretch::arithmetique::operation(noeud->type, evaluer(noeud->children[0]), evaluer(noeud->children[1]));
}
 
/////////////////////////////////////////////////
template <typename Noeud>
void executer(std::unique_ptr<Noeud>& noeud) 
{
    /////////////////////////////////////////////////
    if(noeud->is_root() || noeud->template is_type<stretch::bloc>())    
    {
        for(auto& c: noeud->children) 
        {
            executer(c);
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type<stretch::assignation>()) // a <- 5 + 5 + 5
    {
        assigner(noeud->children.front(), evaluer(noeud->children.back()));
    } 
    /////////////////////////////////////////////////
    else if(noeud->template is_type<stretch::condition>()) // si 5 == 5 alors bloc fin
    {
        Variable resultat = evaluer(noeud->children.front());
        
        if(resultat.get_nature() != Nature::Booleen) 
        {
            throw std::runtime_error("La condition ne retourne pas un booléen.");
        }
        
        if(std::get<bool>(resultat.get_valeur()) == true) 
        {
            executer(noeud->children[1]);
        } 
        else if(noeud->children.size() == 3) 
        {
            executer(noeud->children[2]);
        } 
    } 
    /////////////////////////////////////////////////
    else if(noeud->template is_type<stretch::boucle_repeter>())
    {
        Variable compteur = evaluer(noeud->children.front());

        if(compteur.get_nature() != Nature::Reel) 
        {
            throw std::runtime_error("Le compteur de la boucle n'est pas un entier.");
        }

        BigDecimal n = std::get<BigDecimal>(compteur.get_valeur());
        n.round(1);
 
        for(BigDecimal i = 0; i < n; i += 1) 
        {
            auto& ranger = noeud->children[1];
            if(ranger->template is_type<stretch::ranger>()) 
            {
                assigner(ranger->children[1]->children[0], Variable(i));
            }
            
            executer(noeud->children.back());
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type<stretch::tant_que>())
    {
        //on récupère la condition elle-même puis on l'évalue, elle renvoie une Variable à laquelle on va récupérer le type utilisé dans le variant, ici c'est un booléen.
        bool resultat = std::get<bool>(evaluer(noeud->children.front()).get_valeur());
        while(resultat) 
        {
            //on execute ce qu'il y a à l'intérieur 
            executer(noeud->children.back());
            resultat = std::get<bool>(evaluer(noeud->children.front()).get_valeur());
        }
    }
    else if(noeud->template is_type<stretch::definition_fonction>())
    {
        /*
            auto nom_fonction = noeud->children.front();
            std::vector<std::string> parametres;


            //puis on récupère tous les noms de parametre (tous les fils de stretch::parametres) jusqu'à atteindre le fond 
            if(noeud->children[1]->template is_type<stretch::parametres>() ) 
            {
                for(int i = 0; i < noeud->children.size(); ++i)
                {
                    parametres.put(noeud->children[i]->string());
                }
            }

            definitions_fonctions[nom_fonction->string()] = Bloc(&noeud, parametres});

        */
        ;
    }
}

} // namespace stretch