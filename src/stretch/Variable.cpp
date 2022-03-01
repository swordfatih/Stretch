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

Variable::Variable(VariantValeur valeur) : m_valeur(std::move(valeur))
{
    m_type = static_cast<Nature>(valeur.index());

    if(m_type == Nature::Reel && std::get<BigDecimal>(m_valeur).getDecPart() == "0")
        m_type = Nature::Entier;
}

Variable::Variable(std::string valeur) : Variable(VariantValeur(std::string{std::move(valeur)}))
{
    
}

std::string Variable::to_string() const 
{
    if(m_type == Nature::Chaine)
        return std::get<std::string>(m_valeur);
    else if(m_type == Nature::Booleen)
        return std::get<bool>(m_valeur) == true ? "vrai" : "faux";
    else if(m_type == Nature::Entier) 
        return std::get<BigDecimal>(m_valeur).getIntPart();
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
    else if (type == pe::demangle< stretch::entier >())
        return Nature::Entier;
    else if (type == pe::demangle< stretch::reel >())
        return Nature::Reel;

    return Nature::Nul;
}

VariantValeur Variable::sto_valeur(Nature type, std::string valeur) 
{
    if(type == Nature::Chaine)
        return std::move(valeur);
    else if(type == Nature::Booleen)
        return static_cast<bool>(valeur == "vrai");
    else if(type == Nature::Entier) 
        return BigDecimal(std::move(valeur));
    else if(type == Nature::Reel) 
        return BigDecimal(std::move(valeur));
    
    return {};
}

Variable Variable::parse(std::string valeur) {
    
}

} // namespace stretch