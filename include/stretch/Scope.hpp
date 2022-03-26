/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <map>
#include <iostream>

#include <tao/pegtl/contrib/parse_tree.hpp>

#include "stretch/Variable.hpp"

/////////////////////////////////////////////////
using Noeud = tao::pegtl::parse_tree::node;

/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
/// @brief Scope d'un bloc
/////////////////////////////////////////////////
class Scope {
public:
    /////////////////////////////////////////////////
    Scope(std::unique_ptr<Noeud>& root) : m_root(root) 
    {
        
    }

    /////////////////////////////////////////////////
    std::unique_ptr<Noeud>& get_root()
    {
        return m_root;
    }

    /////////////////////////////////////////////////
    void assigner(const std::string& nom, Variable valeur) 
    {
        m_variables[nom] = valeur;
        std::cerr << "[DEBUG] Affectation de " << m_variables[nom].to_string() << " (" << Variable::type_tos(m_variables[nom].get_nature()) << ") dans la variable " << nom << std::endl;   
    }

    /////////////////////////////////////////////////
    void assigner(std::unique_ptr<Noeud>& variable, Variable valeur) 
    {
        assigner(static_cast<std::string>(variable->string()), valeur);
    }

    /////////////////////////////////////////////////
    Variable lire(const std::string& nom) 
    {
        return m_variables.count(nom) == 0 ? Variable() : m_variables.at(nom);
    }

private:
    /////////////////////////////////////////////////
    std::unique_ptr<Noeud>& m_root;
    std::map<std::string, Variable> m_variables;
};

} // namespace stretch