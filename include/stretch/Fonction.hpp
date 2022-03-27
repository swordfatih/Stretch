/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
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
    /// @param root Noeud de départ du bloc
    /// @param variables Variables locaux déclarés dans le bloc
    /// @param parametres Parametres du bloc, s'il y en a
    /////////////////////////////////////////////////
    Fonction(std::unique_ptr<Noeud>& root, std::vector<std::string> parametres = {});

    /////////////////////////////////////////////////
    std::unique_ptr<Noeud>& get_root();

    /////////////////////////////////////////////////
    static Fonction& enregistrer(const std::string& nom, Fonction fonction);

    /////////////////////////////////////////////////
    static Fonction& recuperer(const std::string& nom);

    /////////////////////////////////////////////////
    static Tableau invoquer(Scope& parent, std::string nom, Tableau& valeurs);

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
    std::vector<std::string> m_parametres;  ///< paramètres de la fonction
    std::unique_ptr<Noeud>& m_root;         ///< noeud racine de la fonction
};

} // stretch