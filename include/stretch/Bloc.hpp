/////////////////////////////////////////////////
/// @brief Headers
/////////////////////////////////////////////////
#include <vector>
#include <iostream>
#include <memory>

namespace stretch {

/////////////////////////////////////////////////
class Variable;

/////////////////////////////////////////////////
/// @brief Classe représentant un bloc d'instruction
/////////////////////////////////////////////////
template <typename T>
class Bloc {
    public:
        /////////////////////////////////////////////////
        /// @brief Constructeur 
        ///
        /// @param root Noeud de départ du bloc
        /// @param variables Variables locaux déclarés dans le bloc
        /// @param parametres Parametres du bloc, s'il y en a
        /////////////////////////////////////////////////
        Bloc(std::unique_ptr<T>* root, std::vector<std::string> parametres = {}, std::map<std::string, Variable> variables = {}) : m_root(root), m_parametres(std::move(parametres)), m_variables(std::move(variables)) 
        {
            
        }
    
        auto& get_variables()
        {  
            return m_variables; 
        }

        std::vector<std::string>& get_parametres()
        {  
            return m_parametres; 
        }

        std::unique_ptr<T>* get_root()
        {
            return m_root;
        }

        /////////////////////////////////////////////////
        /// @brief ToString
        /////////////////////////////////////////////////
        friend std::ostream& operator<<(std::ostream& os, const Bloc& b) 
        {
            return os << "Information sur le bloc à l'adresse " << b.m_root << std::endl
                    << "Liste des paramètres " << b.parametres_string() << std::endl;

        }
    
    private:
        std::string parametres_string(std::ostream &os) 
        {
            for(const std::string& param : m_parametres) {
                os << param;
            }
        } 

        std::unique_ptr<T> *m_root;                     ///< noeud racine du bloc
        
        std::map<std::string, Variable> m_variables;    ///< variables déclarés dans le bloc

        std::vector<std::string> m_parametres;          ///< paramètres du bloc
};

} // stretch