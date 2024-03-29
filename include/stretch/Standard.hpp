#ifndef STANDARD_HPP
#define STANDARD_HPP

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "stretch/Fonction.hpp"

#include "stretch/standard/Aleatoire.hpp"
#include "stretch/standard/Chaine.hpp"
#include "stretch/standard/Mathematiques.hpp"

/////////////////////////////////////////////////
namespace stretch::standard {

/////////////////////////////////////////////////
static std::unique_ptr< pe::Noeud > null_node = nullptr;

/////////////////////////////////////////////////
void charger() 
{
    Fonction::enregistrer("aleatoire", Fonction(null_node, {"from", "to", "real"}, fonction::aleatoire));
    Fonction::enregistrer("separer", Fonction(null_node, {"chaine", "separateur"}, fonction::separer));
    Fonction::enregistrer("tailler", Fonction(null_node, {"chaine"}, fonction::tailler));
    Fonction::enregistrer("puissance", Fonction(null_node, {"base", "exposant"}, fonction::puissance));
    Fonction::enregistrer("racine", Fonction(null_node, {"base", "exposant"}, fonction::racine));
}

} // namespace stretch::standard

#endif // STANDARD_HPP

