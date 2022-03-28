/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "stretch/Scope.hpp"

/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
Scope::Scope(Scope* scope) : m_parent(scope) 
{
    
}

/////////////////////////////////////////////////
Scope* Scope::get_parent() const
{
    return m_parent;
}

/////////////////////////////////////////////////
void Scope::assigner(const std::string& nom, Variable valeur) 
{
    m_variables[nom] = valeur;
}

/////////////////////////////////////////////////
void Scope::assigner(std::unique_ptr<pe::Noeud>& variable, Variable valeur) 
{
    assigner(static_cast<std::string>(variable->string()), valeur);
}

/////////////////////////////////////////////////
Variable Scope::lire(const std::string& nom) 
{
    if(m_variables.count(nom) != 0)
        return m_variables[nom];

    /* 
     * Si la variable n'est pas dans le scope courant, on essaye de la chercher dans le scope parent
     * Si la variable n'est pas dans le scope parent, on lève une exception
    
    else if(m_parent)
        return m_parent->lire(nom); 
    */

    throw exception::Variable(exception::Variable::Type::Inconnue, nom);
}

} // namespace stretch