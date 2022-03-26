/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <stdexcept>

#include "stretch/Evaluation.hpp"
#include "stretch/Exceptions.hpp"
 
/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////
template <typename Noeud>
Tableau executer(std::unique_ptr<Noeud>& noeud);

void assigner(const std::string& nom, Variable valeur);

template <typename Noeud>
void assigner(std::unique_ptr<Noeud>& variable, Variable valeur);

template<typename T>
void ajout_fonction(std::string nom_fonction, Bloc<T> bloc);

template<typename T>
Tableau invoquer_fonction(std::string nom_fonction, std::unique_ptr<T>& noeud);

/////////////////////////////////////////////////
template <typename Noeud>
Tableau executer(std::unique_ptr<Noeud>& noeud) 
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

            if(noeud->children.back()->children->template is_type<stretch::arreter>()) {
                std::cout << "[DEBUG] La boucle est terminée." << std::endl;
                break;
            }

            Tableau&& retour = executer(noeud->children.back());

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
            if(noeud->children.back()->children->template is_type<stretch::arreter>()) {
                 std::cout << "[DEBUG] La boucle est terminée." << std::endl;
                break;
            }

            //on execute ce qu'il y a à l'intérieur 
            Tableau&& retour = executer(noeud->children.back());

            if(!retour.empty()) // déléguer la valeur de retour à la fonction au dessus s'il y en
                return retour;

            resultat = std::get<bool>(evaluer(noeud->children.front()).get_valeur());
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type<stretch::boucle_pour_chaque>())
    {
        auto& element = noeud->children.front();
        Variable tableau = evaluer(noeud->children[1]);

        if(tableau.get_nature() != Nature::Tableau) 
            throw std::runtime_error("Un tableau est requis pour la boucle pour chaque.");

        if(!(element->template is_type<stretch::variable>()))
            throw std::runtime_error(element->string() + " n'est pas un nom de variable valide.");

        Tableau tab = std::get<Tableau>(tableau.get_valeur());
        for(auto& v: tab) 
        {
            assigner(element, v);
            if(noeud->children.back()->children->template is_type<stretch::arreter>()) {
                std::cout << "[DEBUG] La boucle est terminée." << std::endl;
                break;
            }
                
            executer(noeud->children.back());
        }
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type<stretch::arreter>())
    {
        if(!noeud->parent->template is_type<stretch::boucle_repeter>() || !noeud->parent->template is_type<stretch::boucle_tant_que>()
        || !noeud->parent->template is_type<stretch::boucle_pour_chaque>()) {
            throw std::runtime_error("Vous n'êtes pas dans une boucle !");  
        }  

        std::cout << "[DEBUG] La boucle est terminée mais chelou pcq normalement la bonne vérif se fait dans les else if des boucles" << std::endl;
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
    else if(noeud->template is_type<stretch::retour>()) 
    {
        Tableau valeurs;
        
        for(auto& valeur: noeud->children) {
            valeurs.push_back(evaluer(valeur));
        }

        return valeurs;
    }
    /////////////////////////////////////////////////
    else if(noeud->template is_type<stretch::quitter>()) 
    {
        throw exception::QuitterException("Arrêt du programme");
    }

    return {};
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
void ajout_fonction(std::string nom, Bloc<T> bloc)
{
    definitions<T>.insert(std::make_pair(nom, std::move(bloc)));
    std::cerr << "[DEBUG] Création de la fonction " << nom << " (" << bloc.get_root() << ")" << std::endl;
}

/////////////////////////////////////////////////
template<typename T>
Tableau invoquer_fonction(std::string nom_fonction, std::unique_ptr<T>& noeud)
{
    //est-ce que la fonction existe ?
    if(definitions<T>.count(nom_fonction) == 0)
        throw std::runtime_error("La fonction " + nom_fonction + " n'est pas définie");

    auto& bloc = definitions<T>.at(nom_fonction);

    // si il met des paramètres à l'appel alors qu'il n'y en a pas dans la définition
    // nombre de valeurs = tous les fils sauf le nom de la fonction
    if(noeud->children.size() - 1 != bloc.get_parametres().size() || noeud->children.size() == 1 && !bloc.get_parametres().empty()) 
        throw std::runtime_error("L'appel à la fonction n'a pas le même nombre de paramètres que sa définition");

    // itération sur tous les parametres envoyés dans l'arbre
    for(auto i = 0; i < bloc.get_parametres().size(); ++i)
        assigner(bloc.get_parametres()[i], evaluer(valeurs->children[i]));

    return executer(*(bloc.get_root())); // bloc est un itérateur, on le déférence
}

} // namespace stretch