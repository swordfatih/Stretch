/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <map>

#include <tao/pegtl/contrib/parse_tree.hpp>

#include "stretch/Variable.hpp"
#include "stretch/Exceptions.hpp"

/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
class Fonction;

/////////////////////////////////////////////////
/// @brief Scope d'un bloc
/////////////////////////////////////////////////
class Scope {
public:
    /////////////////////////////////////////////////
    Scope(Scope* scope = nullptr) : m_parent(scope) 
    {
        
    }

    /////////////////////////////////////////////////
    Scope* get_parent()
    {
        return m_parent;
    }

    /////////////////////////////////////////////////
    void assigner(const std::string& nom, Variable valeur) 
    {
        m_variables[nom] = valeur;
    }

    /////////////////////////////////////////////////
    void assigner(std::unique_ptr<Noeud>& variable, Variable valeur) 
    {
        assigner(static_cast<std::string>(variable->string()), valeur);
    }

    /////////////////////////////////////////////////
    Variable lire(const std::string& nom) 
    {
        if(m_variables.count(nom) != 0)
            return m_variables[nom];

        /* 
         * Si la variable n'est pas dans le scope courant, on essaye de la chercher dans le scope parent
         * Si la variable n'est pas dans le scope parent, on lève une exception
        
        else if(m_parent)
            return m_parent->lire(nom); 
        */

        throw exception::VariableInconnue(nom);
    }

private:
    /////////////////////////////////////////////////
    Scope* m_parent;                                ///< Scope parente
    std::map<std::string, Variable> m_variables;    ///< Variables locales
};

} // namespace stretch