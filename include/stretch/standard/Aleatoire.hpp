#ifndef ALEATOIRE_HPP
#define ALEATOIRE_HPP

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <random>

#include "stretch/Variable.hpp"

/////////////////////////////////////////////////
namespace stretch::standard::fonction {

/////////////////////////////////////////////////
static std::random_device device;
static std::mt19937 generator(device());

/////////////////////////////////////////////////
Tableau aleatoire(const Tableau& valeurs) 
{
    BigDecimal from = std::get< BigDecimal >(valeurs[0].get_valeur());
    BigDecimal to = std::get< BigDecimal >(valeurs[1].get_valeur());
    BigDecimal resultat;

    if(std::get<bool>(valeurs[2].get_valeur()) == true) 
    {
        std::uniform_real_distribution<> distribution(from.toDouble(), to.toDouble());
        resultat = distribution(generator);
    }
    else 
    {
        std::uniform_int_distribution<> distribution(from.toInt(), to.toInt());
        resultat = distribution(generator);
    }
    
    return { { std::move(resultat) } };
}

} // namespace stretch::standard::fonction

#endif // ALEATOIRE_HPP
