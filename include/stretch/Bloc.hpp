/////////////////////////////////////////////////
/// @brief Headers
/////////////////////////////////////////////////
#include <vector>
#include <iostream>
#include "Variable.hpp"
#include <memory>

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
        Bloc(std::unique_ptr<T>* root, std::vector<Variable> variables = {}, std::vector<Variable> parametres = {}) : m_root(root), m_parametres(std::move(parametres)), m_variables(std::move(variables)) 
        {
    
        }
    
        std::vector<Variable>& getParametres()
        {  
            return parametres; 
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
            for(const Variable& param : parametres) {
                os << param;
            }
        } 

        std::unique_ptr<T> *m_root;                           ///< noeud racine du bloc

        std::vector<Variable> m_variables;      ///< variables déclarés dans le bloc
        std::vector<Variable> m_parametres;     ///< paramètres du bloc
};
