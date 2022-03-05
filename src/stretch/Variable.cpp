#include "stretch/Variable.hpp"

namespace stretch {

Variable::Variable(Nature type, VariantValeur valeur) : m_type(std::move(type)), m_valeur(std::move(valeur))
{
    // default
}

Variable::Variable(Nature type, std::string valeur) : Variable::Variable(type, VariantValeur(std::string{std::move(valeur)})) 
{
    // default
}

Variable::Variable(VariantValeur valeur) : Variable::Variable(static_cast<Nature>(valeur.index()), std::move(valeur))
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

bool Variable::est(Nature type) const
{
    return m_type == type;
}

Nature Variable::sto_nature(std::string_view type) 
{
    if (type == pe::demangle< stretch::chaine >())
        return Nature::Chaine;
    else if (type == pe::demangle< stretch::booleen >())
        return Nature::Booleen;
    else if (type == pe::demangle< stretch::reel >() || type == pe::demangle< stretch::entier >())
        return Nature::Reel;

    return Nature::Nul;
}

std::string Variable::type_tos(Nature type)
{
    if(type == Nature::Chaine)
        return "chaine";
    else if(type == Nature::Booleen)
        return "booleen";
    else if(type == Nature::Reel)
        return "reel";
    
    return "nul";
}

VariantValeur Variable::sto_valeur(Nature type, std::string valeur) 
{
    if(type == Nature::Chaine)
        return std::move(valeur);
    else if(type == Nature::Booleen) {
        for(auto& c: valeur) c = std::tolower(c);
        return valeur == "vrai";
    } 
    else if(type == Nature::Reel) 
        return BigDecimal(std::move(valeur));
    
    return {};
}

Variable Variable::parse(std::string valeur) 
{
    std::string lower = valeur;
    for(auto& c: lower) c = std::tolower(c);

    std::regex number_regex("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");
    std::smatch match;

    if(lower == "vrai" || lower == "faux")
        return Variable(Nature::Booleen, lower);
    else if(std::regex_match(valeur, match, number_regex)) 
        return Variable(BigDecimal(std::move(valeur)));

    return Variable(std::move(valeur));
}

} // namespace stretch