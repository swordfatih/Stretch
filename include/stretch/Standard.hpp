#ifndef STANDARD_HPP
#define STANDARD_HPP

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "stretch/Fonction.hpp"

#include "stretch/standard/Aleatoire.hpp"

/////////////////////////////////////////////////
namespace stretch::standard {

/////////////////////////////////////////////////
static std::unique_ptr< pe::Noeud > null_node = nullptr;

/////////////////////////////////////////////////
void charger() 
{
    Fonction::enregistrer("aleatoire", Fonction(null_node, {"from", "to", "real"}, fonction::aleatoire));
}

} // namespace stretch::standard

#endif // STANDARD_HPP

