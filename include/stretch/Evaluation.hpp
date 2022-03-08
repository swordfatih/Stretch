#include "stretch/Arithmetique.hpp"

#include <algorithm>
#include <stdexcept>

namespace stretch {

static std::map<std::string_view, Variable> variables;

template <typename Noeud>
Variable evaluer(std::unique_ptr<Noeud>& noeud) 
{
    if(noeud->children.empty()) 
    {
        return Variable(noeud);
    }
        
    return stretch::arithmetique::operation(noeud->type, evaluer(noeud->children[0]), evaluer(noeud->children[1]));
}

template <typename Noeud>
void executer(std::unique_ptr<Noeud>& noeud) 
{
    if(noeud->is_root() || noeud->template is_type<stretch::bloc>())    
    {
        for(auto& c: noeud->children) 
        {
            executer(c);
        }
    }
    else if(noeud->template is_type<stretch::assignation>()) // a <- 5 + 5 + 5
    {
        variables[noeud->children[0]->string()] = evaluer(noeud->children[1]);
        std::cout << "Affectation de " << variables[noeud->children[0]->string()].to_string() << " dans la variable " <<  noeud->children[0]->string() << std::endl;
    } 
    else if(noeud->template is_type<stretch::condition>()) // si 5 == 5 alors bloc fin
    {
        Variable resultat = evaluer(noeud->children[0]);
        
        if(resultat.get_nature() != Nature::Booleen) 
        {
            throw std::runtime_error("La condition ne retourne pas un booléen.");
        }
        
        if(std::get<bool>(resultat.get_valeur()) == true) 
        {
            executer(noeud->children[1]);
        } 
        else if(noeud->children.size() == 3) 
        {
            executer(noeud->children[2]);
        }
    }
}

} // namespace stretch