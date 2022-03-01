#include "stretch/Variable.hpp"

namespace stretch {

Variable::Variable(Nature type, VariantValeur valeur) : m_type(std::move(type)), m_valeur(std::move(valeur))
{
    // default
}

Variable::Variable(Nature type, std::string valeur) : Variable::Variable(type, Variable::sto_valeur(type, std::move(valeur))) 
{
    // default
}

Variable::Variable(VariantValeur valeur) : m_valeur(std::move(valeur)), m_type(static_cast<Nature>(valeur.index()))
{
    // default
}

Variable::Variable(std::string valeur) : Variable(VariantValeur(std::string{std::move(valeur)}))
{
    // default
}

Nature Variable::get_nature() const 
{
    return m_type;
}

VariantValeur Variable::get_valeur() const 
{
    return m_valeur;
}

std::string Variable::to_string() const 
{
    if(m_type == Nature::Chaine)
        return std::get<std::string>(m_valeur);
    else if(m_type == Nature::Booleen)
        return std::get<bool>(m_valeur) == true ? "vrai" : "faux";
    else if(m_type == Nature::Reel) 
        return std::get<BigDecimal>(m_valeur).toString();
    
    return "nul";
}

Nature Variable::sto_nature(std::string_view type) 
{
    if (type == pe::demangle< stretch::chaine >())
        return Nature::Chaine;
    else if (type == pe::demangle< stretch::booleen >())
        return Nature::Booleen;
    else if (type == pe::demangle< stretch::reel >())
        return Nature::Reel;

    return Nature::Nul;
}

VariantValeur Variable::sto_valeur(Nature type, std::string valeur) 
{
    if(type == Nature::Chaine)
        return std::move(valeur);
    else if(type == Nature::Booleen) {
        std::for_each(valeur.begin(), valeur.end(), [](char& c){ c = std::tolower(c); });
        return static_cast<bool>(valeur == "vrai");
    } 
    else if(type == Nature::Reel) 
        return BigDecimal(std::move(valeur));
    
    return {};
}

Variable Variable::parse(std::string valeur) 
{
    std::string lower = valeur;
    std::for_each(lower.begin(), lower.end(), [](char& c){ c = std::tolower(c); });

    std::regex number_regex("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");
    std::smatch match;

    if(lower == "vrai" || lower == "faux")
        return Variable(Nature::Booleen, lower);
    else if(std::regex_match(valeur, match, number_regex)) 
        return Variable(BigDecimal(std::move(valeur)));

    return Variable(std::move(valeur));
}

} // namespace stretch