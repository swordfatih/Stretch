/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "stretch/Fonction.hpp"

/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
Tableau executer(std::unique_ptr<pe::Noeud>& noeud, Scope& scope);

/////////////////////////////////////////////////
Fonction::Fonction(std::unique_ptr<pe::Noeud>& root, std::vector<std::string> parametres, const std::function<Tableau(const Tableau&)>& interne) : m_root(root), m_parametres(std::move(parametres)), m_interne(interne) 
{
    
}

/////////////////////////////////////////////////
std::unique_ptr<pe::Noeud>& Fonction::get_root()
{
    return m_root;
}

/////////////////////////////////////////////////
Fonction& Fonction::enregistrer(const std::string& nom, Fonction fonction) 
{
    if(definitions.count(nom) != 0)
        throw std::runtime_error("La fonction " + nom + " est déjà enregistrée.");

    definitions.insert(std::make_pair(nom, std::move(fonction)));

    return definitions.at(nom);
}

/////////////////////////////////////////////////
Fonction& Fonction::recuperer(const std::string& nom) 
{
    if(definitions.count(nom) == 0)
        throw std::runtime_error("La fonction " + nom + " n'est pas enregistrée");

    return definitions.at(nom);
}

/////////////////////////////////////////////////
Tableau Fonction::invoquer(Scope& parent, const std::string& nom, const Tableau& valeurs)
{
    Fonction& fonction = Fonction::recuperer(nom);
    Scope scope(&parent);

    // si il met des paramètres à l'appel alors qu'il n'y en a pas dans la définition
    // nombre de valeurs = tous les fils sauf le nom de la fonction
    if(valeurs.size() != fonction.m_parametres.size() || valeurs.empty() && !fonction.m_parametres.empty()) 
        throw std::runtime_error("L'appel à la fonction n'a pas le même nombre de paramètres que sa définition");

    // si la fonction a une fonction interne
    // c'est une fonction standard
    if(fonction.m_interne) 
        return fonction.m_interne(valeurs);

    // itération sur tous les parametres envoyés dans l'arbre
    for(size_t i = 0; i < valeurs.size(); ++i)
        scope.assigner(fonction.m_parametres[i], valeurs[i]);

    return executer(fonction.get_root(), scope); // root est un pointeur, on le déférence
}

} // namespace stretch