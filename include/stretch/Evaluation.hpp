#include "stretch/Arithmetique.hpp"
#include "stretch/Bloc.hpp"
#include "stretch/StretchException.hpp"

#include <algorithm>
#include <stdexcept>
 
namespace stretch {

using Retour = std::vector<Variable>;

/////////////////////////////////////////////////
// Memoire
/////////////////////////////////////////////////
template <typename T>
static std::map<std::string, Bloc<T>> definitions;

static std::map<std::string, Variable> variables;

/////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////
template <typename Noeud>
Variable evaluer(std::unique_ptr<Noeud>& noeud);

template <typename Noeud>
Retour executer(std::unique_ptr<Noeud>& noeud);

void assigner(const std::string& nom, Variable valeur);

template <typename Noeud>
void assigner(std::unique_ptr<Noeud>& variable, Variable valeur);

template<typename T>
void ajout_fonction(std::string nom_fonction, Bloc<T> bloc);

template<typename T>
Retour invoquer_fonction(std::string nom_fonction, std::unique_ptr<T>& noeud);

/////////////////////////////////////////////////
template <typename Noeud>
Retour executer(std::unique_ptr<Noeud>& noeud) 
{
    /////////////////////////////////////////////////
    if(noeud->is_root() || noeud->template is_type<stretch::bloc>())    
    {
        for(auto& c: noeud->children) 
        {
            auto&& valeurs_retour = executer(c);
            
            if(!valeurs_retour.empty())
                return valeurs_retour;
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
            Variable&& resultat = evaluer(noeud->children[i]);
        
            if(resultat.get_nature() != Nature::Booleen) 
                throw std::runtime_error("La condition ne retourne pas un booléen.");

            if(std::get<bool>(resultat.get_valeur()) == true) {
                return executer(noeud->children[i + 1]);
            }
        }
        
        if(noeud->children.size() % 2 != 0) // vérifier l'existence d'un sinon final
            return executer(noeud->children.back());
    } 
    /////////////////////////////////////////////////
    else if(noeud->template is_type<stretch::boucle_repeter>())
    {
        Variable compteur = evaluer(noeud->children.front());

        if(compteur.get_nature() != Nature::Reel) 
            throw std::runtime_error("Le compteur de la boucle n'est pas un entier.");

        BigDecimal n = std::get<BigDecimal>(compteur.get_valeur());
        n.round(1);
 
        for(BigDecimal i = 1; i < n + 1; i += 1) 
        {
            auto& ranger = noeud->children[1];

            if(ranger->template is_type<stretch::variable>()) 
                assigner(ranger, Variable(i));

            Retour&& retour = executer(noeud->children.back());

            if(!retour.empty()) // déléguer la valeur de retour à la fonction au dessus s'il y en
                return retour;
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type<stretch::boucle_tant_que>())
    {
        // on récupère la condition elle-même puis on l'évalue, elle renvoie une Variable à laquelle on va récupérer le type utilisé dans le variant, ici c'est un booléen.
        bool resultat = std::get<bool>(evaluer(noeud->children.front()).get_valeur());

        while(resultat) 
        {
            //on execute ce qu'il y a à l'intérieur 
            Retour&& retour = executer(noeud->children.back());

            if(!retour.empty()) // déléguer la valeur de retour à la fonction au dessus s'il y en
                return retour;

            resultat = std::get<bool>(evaluer(noeud->children.front()).get_valeur());
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type<stretch::definition_fonction>())
    {
        std::vector<std::string> parametres;

        // puis on récupère tous les noms de parametres (tous les fils de stretch::parametres) jusqu'à atteindre le fond 
        if(noeud->children[1]->template is_type<stretch::parametres>()) 
        {
            for(int i = 0; i < noeud->children[1]->children.size(); ++i)
                parametres.push_back(noeud->children[1]->children[i]->string());
        }

        // noeud->children.front() = nom de la fonction
        ajout_fonction(noeud->children.front()->string(), Bloc(&(noeud->children.back()), parametres));
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type<stretch::retourner_valeurs>()) 
    {
        Retour valeurs;
        
        for(auto& valeur: noeud->children) {
            valeurs.push_back(evaluer(valeur));
        }

        return valeurs;
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type<stretch::quitter>()) 
    {
        throw StretchException("Arrêt du programme");
    }

    return {};
}

/////////////////////////////////////////////////
template <typename Noeud>
Variable evaluer(std::unique_ptr<Noeud>& noeud) 
{
    if(noeud->template is_type<stretch::appel_fonction>()) {
        if(!noeud->children.front()->template is_type< stretch::variable >())
            throw std::runtime_error("Le nom de la fonction n'est pas un identificateur valide.");

        Retour&& retour = invoquer_fonction(noeud->children.front()->string(), noeud);
        
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

/////////////////////////////////////////////////
void assigner(const std::string& nom, Variable valeur) 
{
    variables[nom] = valeur;
    std::cerr << "[DEBUG] Affectation de " << variables[nom].to_string() << " (" << Variable::type_tos(variables[nom].get_nature()) << ") dans la variable " << nom << std::endl;
}

/////////////////////////////////////////////////
template <typename Noeud>
void assigner(std::unique_ptr<Noeud>& variable, Variable valeur) 
{
    assigner(static_cast<std::string>(variable->string()), valeur);
}

/////////////////////////////////////////////////
template<typename T>
void ajout_fonction(std::string nom_fonction, Bloc<T> bloc)
{
    definitions<T>.insert(std::make_pair(nom_fonction, std::move(bloc)));
    std::cerr << "[DEBUG] Création de la fonction " << nom_fonction << " (" << bloc.get_root() << ")" << std::endl;
}

/////////////////////////////////////////////////
template<typename T>
Retour invoquer_fonction(std::string nom_fonction, std::unique_ptr<T>& noeud)
{
    //est-ce que la clé existe ?
    if(definitions<T>.count(nom_fonction) == 0)
    {
        throw std::runtime_error("La fonction " + nom_fonction + " n'est pas définie");
    }

    auto& bloc = definitions<T>.at(nom_fonction);

    // si il met des paramètres à l'appel alors qu'il n'y en a pas dans la définition
    auto& liste_parametres = noeud->children.back();
    if(liste_parametres->template is_type<stretch::valeur_parametres>() && liste_parametres->children.size() != bloc.get_parametres().size()
        || !liste_parametres->template is_type<stretch::valeur_parametres>() && !bloc.get_parametres().empty()) 
    {
            throw std::runtime_error("L'appel à la fonction n'a pas le même nombre de paramètres que sa définition");
    }

    // itération sur tous les parametres envoyés dans l'arbre
    for(auto i = 0; i < bloc.get_parametres().size(); ++i)
    {
        assigner(bloc.get_parametres()[i], evaluer(liste_parametres->children[i]));
    }

    return executer(*(bloc.get_root())); // bloc est un itérateur, on le déférence
}

} // namespace stretch