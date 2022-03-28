/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "stretch/Variable.hpp"

/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
Variable::Variable(Nature type, VariantValeur valeur) : m_type(std::move(type)), m_valeur(std::move(valeur))
{
    // default
}

/////////////////////////////////////////////////
Variable::Variable(Nature type, std::string valeur) : Variable::Variable(type, VariantValeur(std::string{std::move(valeur)})) 
{
    // default
}

/////////////////////////////////////////////////
Variable::Variable(VariantValeur valeur) : Variable::Variable(static_cast<Nature>(valeur.index()), std::move(valeur))
{
    // default
}

/////////////////////////////////////////////////
Variable::Variable(std::string valeur) : Variable(VariantValeur(std::string{std::move(valeur)}))
{
    // default
}

/////////////////////////////////////////////////
Variable::Variable(std::unique_ptr<pe::Noeud>& noeud) : m_type(sto_nature(noeud->type)) 
{
    std::string&& valeur = noeud->string();
    remplacer(valeur, "\\\"", "\"");
    remplacer(valeur, "\\n", "\n");
    
    m_valeur = sto_valeur(m_type, valeur);
}

/////////////////////////////////////////////////
Nature Variable::get_nature() const 
{
    return m_type;
}

/////////////////////////////////////////////////
VariantValeur Variable::get_valeur() const 
{
    return m_valeur;
}

/////////////////////////////////////////////////
std::string Variable::to_string() const 
{
    if(m_type == Nature::Chaine)
        return std::get<std::string>(m_valeur);
    else if(m_type == Nature::Booleen)
        return std::get<bool>(m_valeur) == true ? "vrai" : "faux";
    else if(m_type == Nature::Reel) 
        return std::get<BigDecimal>(m_valeur).toString();
    else if(m_type == Nature::Tableau) {
        auto& tableau = std::get<Tableau>(m_valeur);
        
        std::stringstream chaine;
        chaine << "[";
        
        for(int i = 0; i < tableau.size(); ++i) {
            if(i != 0)
                chaine << ", ";
            
            chaine << tableau[i].to_string();
        }

        chaine << "]";

        return chaine.str();
    }
    
    return "nul";
}

/////////////////////////////////////////////////
bool Variable::est(Nature type) const
{
    return m_type == type;
}

/////////////////////////////////////////////////
Nature Variable::sto_nature(std::string_view type) 
{
    if (type == pe::demangle< stretch::chaine >())
        return Nature::Chaine;
    else if (type == pe::demangle< stretch::booleen >())
        return Nature::Booleen;
    else if (type == pe::demangle< stretch::reel >() || type == pe::demangle< stretch::entier >())
        return Nature::Reel;
    else if (type == pe::demangle< stretch::tableau >())
        return Nature::Tableau;

    return Nature::Nul;
}

/////////////////////////////////////////////////
std::string Variable::type_tos(Nature type)
{
    if(type == Nature::Chaine)
        return "chaine";
    else if(type == Nature::Booleen)
        return "booleen";
    else if(type == Nature::Reel)
        return "reel";
    else if(type == Nature::Tableau)
        return "tableau";
    
    return "nul";
}

/////////////////////////////////////////////////
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

    /*
    else if(type == Nature::Tableau)
    {
        valeur.erase(valeur.begin());
        valeur.erase(valeur.end());

        std::istringstream sstream(valeur);
        std::vector<std::string> elements;
        std::string element;
        
        Tableau tableau;
        
        while(std::getline(sstream, element, ',')) //[1, 2, 3, [5,"salut"]]
            tableau.push_back(parse(element));

        return tableau;
    } // A gérer plus tard : parsing en ayant un tableau comme une chaine de caractères
    */
    
    return {};
}

/////////////////////////////////////////////////
Variable Variable::parse(std::string valeur) 
{
    std::string lower = valeur;
    for(auto& c: lower) c = std::tolower(c);

    std::regex number_regex("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");
    std::regex tableau_regex("^\\[.*(,.*)*\\]$");
    std::smatch match;

    if(lower == "vrai" || lower == "faux")
        return Variable(Nature::Booleen, lower);
    else if(std::regex_match(valeur, match, number_regex)) 
        return Variable(BigDecimal(std::move(valeur)));

    /*
    else if(std::regex_match(valeur, match, tableau_regex)) {
        std::vector<Variable> variables;

        for(auto& v : split_tableau(valeur, tableau_regex))
            variables.push_back(parse(v));

        return Variable(variables);
    }
    */

    return Variable(std::move(valeur));
}

/////////////////////////////////////////////////
void Variable::remplacer(std::string& source, const std::string& from, const std::string& to) 
{
    size_t start_pos = 0;

    while((start_pos = source.find(from, start_pos)) != std::string::npos) {
        source.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}

/////////////////////////////////////////////////
std::vector<std::string> Variable::split_tableau(const std::string& s, const std::regex& tableau_regex) 
{
    std::vector<std::string> elems;

    std::sregex_token_iterator iter(s.begin(), s.end(), tableau_regex, -1);
    std::sregex_token_iterator end;

    while (iter != end)  {
        elems.push_back(*iter);
        ++iter;
    }

    return elems;
}

} // namespace stretch