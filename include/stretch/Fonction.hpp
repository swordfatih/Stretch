#pragma once

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
    Fonction(std::unique_ptr<Noeud>& root, std::vector<std::string> parametres = {}, const std::function<Tableau(const Tableau&)>& interne = {});

    /////////////////////////////////////////////////
    std::unique_ptr<Noeud>& get_root();

    /////////////////////////////////////////////////
    static Fonction& enregistrer(const std::string& nom, Fonction fonction);

    /////////////////////////////////////////////////
    static Fonction& recuperer(const std::string& nom);

    /////////////////////////////////////////////////
    static Tableau invoquer(Scope& parent, const std::string& nom, const Tableau& valeurs);

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
    std::unique_ptr<Noeud>& m_root;                     ///< noeud racine de la fonction
    std::function<Tableau(const Tableau&)> m_interne;   ///< fonction interne
};

} // stretch