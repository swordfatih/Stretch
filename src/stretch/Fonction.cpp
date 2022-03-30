/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "stretch/Fonction.hpp"

/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
void executer(std::unique_ptr<pe::Noeud>& noeud, Scope& scope);

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
std::vector<std::string>& Fonction::get_parametres()
{
    return m_parametres;
}

/////////////////////////////////////////////////
void Fonction::enregistrer(const std::string& nom, Fonction fonction) 
{
    if(existe(nom))
        throw std::runtime_error("Le nom de la fonction existe deja, essaye un autre nom.");

    definitions.insert(std::make_pair(nom, std::move(fonction)));
}

/////////////////////////////////////////////////
bool Fonction::existe(const std::string& nom) 
{
    return definitions.count(nom) != 0;
}

/////////////////////////////////////////////////
Fonction& Fonction::recuperer(const std::string& nom) 
{
    if(existe(nom))
        return definitions.at(nom);

    throw std::runtime_error("La fonction n'existe pas (vérifie que t'as bien mis le bon nom, les majuscules comptent)");
}

/////////////////////////////////////////////////
void Fonction::invoquer(Scope& parent, const std::string& nom, const Tableau& valeurs)
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
    {
        Tableau retour = fonction.m_interne(valeurs);
        
        if(!retour.empty())
            throw exception::Retour(retour);

        return;    
    }

    // itération sur tous les parametres envoyés dans l'arbre
    for(size_t i = 0; i < valeurs.size(); ++i)
        scope.assigner(fonction.m_parametres[i], valeurs[i]);

    executer(fonction.get_root(), scope); 
}

} // namespace stretch