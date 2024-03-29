#ifndef FONCTION_HPP
#define FONCTION_HPP

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <functional>

#include "stretch/Scope.hpp"

namespace stretch {

/////////////////////////////////////////////////
class Fonction;
static std::map<std::string, Fonction> definitions;

/////////////////////////////////////////////////
/// @brief Classe représentant une fonction
/////////////////////////////////////////////////
class Fonction {
public:
    /////////////////////////////////////////////////
    /// @brief Constructeur 
    ///
    /// @param root Noeud racine de la fonction
    /// @param parametres Parametres de la fonction, s'il y en a
    /// @param interne Fonction interne, s'il y en a une
    /////////////////////////////////////////////////
    Fonction(std::unique_ptr<pe::Noeud>& root, std::vector<std::string> parametres = {}, const std::function<Tableau(const Tableau&)>& interne = {});

    /////////////////////////////////////////////////
    std::unique_ptr<pe::Noeud>& get_root();

    /////////////////////////////////////////////////
    std::vector<std::string>& get_parametres();

    /////////////////////////////////////////////////
    static void enregistrer(const std::string& nom, Fonction fonction);

    /////////////////////////////////////////////////
    static bool existe(const std::string& nom);

    /////////////////////////////////////////////////
    static Fonction& recuperer(const std::string& nom);

    /////////////////////////////////////////////////
    static void invoquer(Scope& parent, const std::string& nom, const Tableau& valeurs);

    /////////////////////////////////////////////////
    /// @brief Affichage
    /////////////////////////////////////////////////
    friend std::ostream& operator<<(std::ostream& os, const Fonction& f) 
    {
        os << "Liste des paramètres ";

        for(const std::string& param : f.m_parametres)
            os << param;
    
        return os << std::endl;
    }

private:
    /////////////////////////////////////////////////
    std::vector<std::string> m_parametres;              ///< paramètres de la fonction
    std::unique_ptr<pe::Noeud>& m_root;                 ///< noeud racine de la fonction
    std::function<Tableau(const Tableau&)> m_interne;   ///< fonction interne
};

} // stretch

#endif // FONCTION_HPP