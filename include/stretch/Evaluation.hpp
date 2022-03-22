#include "stretch/Arithmetique.hpp"
#include "stretch/Bloc.hpp"

#include <algorithm>
#include <stdexcept>
 
namespace stretch {

template <typename Noeud>
void executer(std::unique_ptr<Noeud>& noeud);

/////////////////////////////////////////////////
static std::map<std::string, Variable> variables;

template <typename T>
static std::map<std::string, Bloc<T>> definitions_fonctions;

/////////////////////////////////////////////////
template <typename Noeud>
void assigner(std::unique_ptr<Noeud>& variable, const Variable& valeur) 
{
    variables.insert(std::make_pair(std::string{variable->string()}, valeur));
    std::cerr << "[DEBUG] Affectation de " << variables[variable->string()].to_string() << " (" << Variable::type_tos(variables[variable->string()].get_nature()) << ") dans la variable " <<  variable->string() << std::endl;
}

template<typename T>
void ajout_map(std::string nom_fonction, Bloc<T> bloc)
{
    definitions_fonctions<T>.insert(std::make_pair(nom_fonction, bloc));
}

template<typename T>
void verification_fonction(std::unique_ptr<T>& noeud, std::string nom_fonction)
{
    auto bloc = definitions_fonctions<T>.find(nom_fonction); //retourne un std::pair
    if(bloc == definitions_fonctions<T>.end())
    {
        throw std::runtime_error("Votre fonction n'est pas définie !");
    }

    //si il met des paramètres à l'appel alors qu'il n'y en a pas dans la définition
    auto& liste_parametres = noeud->children[1];
    if(liste_parametres->template is_type<stretch::variables_fonction>())
    {
        std::vector<std::string> parametres_entrees;

        //itération sur tous les parametres envoyés dans l'arbre
        for(auto& param : liste_parametres->children)
        {
            parametres_entrees.push_back(param->string());
        }

        if(parametres_entrees.size() != bloc->second.get_parametres().size())
        {
            throw std::runtime_error("L'appel à la fonction n'a pas le même nombre de paramètres que sa définition.");
        }
    }
        
    executer(*(bloc->second.get_root())); //bloc est un itérateur, on le déférence
}
/////////////////////////////////////////////////
template <typename Noeud>
Variable evaluer(std::unique_ptr<Noeud>& noeud) 
{
    if(noeud->template is_type< stretch::variable > ()) {
        if(variables.count(noeud->string()) == 0)
            return Variable();
            
        return variables.at(noeud->string());
    }   

    if(noeud->children.empty()) 
        return Variable(noeud);

    if(noeud->children.size() == 1)
        return stretch::arithmetique::operation(noeud->type, evaluer(noeud->children[0]), Variable());

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
        for(auto i = 0; i < noeud->children.size() - 1; i += 2) {
            Variable resultat = evaluer(noeud->children[i]);
        
            if(resultat.get_nature() != Nature::Booleen) 
                throw std::runtime_error("La condition ne retourne pas un booléen.");

            if(std::get<bool>(resultat.get_valeur()) == true) {
                executer(noeud->children[i + 1]);
                return;
            }
        }
        
        executer(noeud->children.back());
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
        std::vector<std::string> parametres;


        //puis on récupère tous les noms de parametres (tous les fils de stretch::parametres) jusqu'à atteindre le fond 
        if(noeud->children[1]->template is_type<stretch::parametres>()) 
        {
            for(int i = 0; i < noeud->children[1]->children.size(); ++i)
            {
                parametres.push_back(noeud->children[1]->children[i]->string());
            }
        }

        //noeud->children.front() = nom de la fonction
        ajout_map(noeud->children.front()->string(), Bloc(&(noeud->children.back()), parametres));
    }
    else if(noeud->template is_type<stretch::appel_fonction>())
    {   
        verification_fonction(noeud, noeud->children.front()->string());
    }
}

} // namespace stretch