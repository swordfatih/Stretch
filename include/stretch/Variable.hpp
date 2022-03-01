#include <cstdint>
#include <variant>
#include <string>
#include <memory>
#include <algorithm>
#include <regex>
#include <iostream>

#include <tao/pegtl/demangle.hpp>

#include "decimal/BigDecimal.h"
#include "stretch/Grammaire.hpp"

namespace pe = tao::pegtl;

namespace stretch {

enum Nature {
    Nul = -1,
    Chaine,
    Booleen,
    Reel
};

using VariantValeur = std::variant< std::string, bool, BigDecimal >;

/////////////////////////////////////////////////
/// @brief Classe représentant une variable
///
/// Elle doit pouvoir créer une variable depuis:
/// - La nature et la valeur
/// - Un std::variant
/// - Un std::string
/// - Un noeud
/////////////////////////////////////////////////
struct Variable
{
public:
    static Nature sto_nature(std::string_view type);
    static VariantValeur sto_valeur(Nature type, std::string valeur);
    static Variable parse(std::string valeur);

    Variable(Nature type = Nature::Nul, VariantValeur valeur = {});
    Variable(Nature type, std::string valeur);

    Variable(VariantValeur valeur);
    Variable(std::string valeur);

    template <typename T>
    Variable(std::unique_ptr<T>& noeud) : Variable(sto_nature(noeud->type), noeud->string()) {}

    Nature get_nature() const;
    VariantValeur get_valeur() const;

    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& o, const Variable& v) { return o << v.to_string(); }

private:
    Nature m_type; 
    VariantValeur m_valeur; 
};

} // namespace stretch