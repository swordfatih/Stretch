#ifndef CHAINE_HPP
#define CHAINE_HPP

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <regex>
#include <string>

#include "stretch/Variable.hpp"

/////////////////////////////////////////////////
namespace stretch::standard::fonction {

/////////////////////////////////////////////////
Tableau separer(const Tableau& valeurs) 
{
    if(valeurs[0].get_nature() != Nature::Chaine || valeurs[1].get_nature() != Nature::Chaine) 
    {
        throw std::runtime_error("Les deux paramètres doivent être des chaînes.");
    }

    std::string chaine = std::get< std::string >(valeurs[0].get_valeur());
    std::string separateur = std::get< std::string >(valeurs[1].get_valeur());

    std::string::size_type last_pos = 0;
    std::string::size_type sep_pos = chaine.find(separateur, 0);

    Tableau resultat;
    while (std::string::npos != sep_pos)
    {
        std::string token = chaine.substr(last_pos, sep_pos - last_pos);

        if(!token.empty())
            resultat.push_back(Variable(token));

        last_pos += token.length() + separateur.length();
        sep_pos = chaine.find(separateur, last_pos);

        if(std::string::npos == sep_pos && last_pos < chaine.length())
            resultat.push_back(Variable(chaine.substr(last_pos)));
    }

    return resultat;
}

/////////////////////////////////////////////////
Tableau tailler(const Tableau& valeurs) 
{
    if(valeurs[0].get_nature() != Nature::Chaine) 
    {
        throw std::runtime_error("Le paramètre doit être une chaîne.");
    }

    std::string chaine = std::get< std::string >(valeurs[0].get_valeur());

    std::regex regex("^\\s+|\\s+$");
    return { { std::regex_replace(chaine, regex, "") } };
}

} // namespace stretch::standard::fonction

#endif // CHAINE_HPP