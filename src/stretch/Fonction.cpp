/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "stretch/Fonction.hpp"

/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
Tableau executer(std::unique_ptr<Noeud>& noeud, Fonction& fonction);

/////////////////////////////////////////////////
Fonction::Fonction(std::unique_ptr<Noeud>& root, std::vector<std::string> parametres) : m_scope(Scope(root)), m_parametres(std::move(parametres)) 
{
    
}

/////////////////////////////////////////////////
Scope& Fonction::get_scope() 
{
    return m_scope;
}

/////////////////////////////////////////////////
void Fonction::enregistrer(const std::string& nom, Fonction fonction) 
{
    if(definitions.count(nom) != 0)
        throw std::runtime_error("La fonction " + nom + " est déjà enregistrée.");

    definitions.insert(std::make_pair(nom, std::move(fonction)));
}

/////////////////////////////////////////////////
Fonction& Fonction::recuperer(const std::string& nom) 
{
    if(definitions.count(nom) == 0)
        throw std::runtime_error("La fonction " + nom + " n'est pas enregistrée");

    return definitions.at(nom);
}

/////////////////////////////////////////////////
Tableau Fonction::invoquer(std::string nom, Tableau& valeurs)
{
    Fonction& fonction = Fonction::recuperer(nom);

    // si il met des paramètres à l'appel alors qu'il n'y en a pas dans la définition
    // nombre de valeurs = tous les fils sauf le nom de la fonction
    if(valeurs.size() != fonction.m_parametres.size() || valeurs.empty() && !fonction.m_parametres.empty()) 
        throw std::runtime_error("L'appel à la fonction n'a pas le même nombre de paramètres que sa définition");

    // itération sur tous les parametres envoyés dans l'arbre
    for(size_t i = 0; i < valeurs.size(); ++i)
        fonction.m_scope.assigner(fonction.m_parametres[i], valeurs[i]);

    return executer(fonction.m_scope.get_root(), fonction); // root est un pointeur, on le déférence
}

} // namespace stretch