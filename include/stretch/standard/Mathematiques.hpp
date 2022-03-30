#ifndef MATHEMATIQUES_HPP
#define MATHEMATIQUES_HPP

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <cmath>
#include <sstream>

#include "stretch/Variable.hpp"

/////////////////////////////////////////////////
namespace stretch::standard::fonction {

/////////////////////////////////////////////////
Tableau puissance(const Tableau& valeurs) 
{
    if(valeurs[0].get_nature() != Nature::Reel || valeurs[1].get_nature() != Nature::Reel) 
    {
        throw std::runtime_error("Les deux paramètres doivent être des réels.");
    }

    BigDecimal base = std::get< BigDecimal >(valeurs[0].get_valeur());
    BigDecimal exposant = std::get< BigDecimal >(valeurs[1].get_valeur());

    std::stringstream stream;
    stream << std::fixed << std::setprecision(10000) << std::pow(base.toDouble(), exposant.toDouble());
    std::string resultat = stream.str();

    return { { BigDecimal(resultat) } };
}

/////////////////////////////////////////////////
Tableau racine(const Tableau& valeurs) 
{
    if(valeurs[0].get_nature() != Nature::Reel || valeurs[1].get_nature() != Nature::Reel) 
    {
        throw std::runtime_error("Les deux paramètres doivent être des réels.");
    }

    BigDecimal base = std::get< BigDecimal >(valeurs[0].get_valeur());
    BigDecimal exposant = std::get< BigDecimal >(valeurs[1].get_valeur());

    std::stringstream stream;
    stream << std::fixed << std::setprecision(10000) << std::pow(base.toDouble(), (BigDecimal(1.0) / exposant).toDouble());
    std::string resultat = stream.str();

    return { { BigDecimal(resultat) } };
}

} // namespace stretch::standard::fonction

#endif // MATHEMATIQUES_HPP