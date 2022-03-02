/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
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

/////////////////////////////////////////////////
/// @brief Les différents types de valeur
/////////////////////////////////////////////////
enum class Nature {
    Nul = -1,
    Chaine,
    Booleen,
    Reel
};

static std::string type_tos(Nature type) {
    if(type == Nature::Chaine)
        return "chaine";
    else if(type == Nature::Booleen)
        return "booleen";
    else if(type == Nature::Reel)
        return "reel";
    
    return "nul";
}

/////////////////////////////////////////////////
/// @brief Les types utilisés en mémoire pour
/// représenter les différents types de valeurs
/////////////////////////////////////////////////
using VariantValeur = std::variant< std::string, bool, BigDecimal >;

/////////////////////////////////////////////////
/// @brief Classe représentant une variable
///
/// Une variable doit pouvoir être créée depuis
/// - Sa nature et sa valeur
/// - Un std::variant
/// - Parse depuis un std::string
/// - Un noeud
/////////////////////////////////////////////////
class Variable
{
public:
    /////////////////////////////////////////////////
    /// @brief Convertir un string en type
    ///
    /// @param type La chaîne à convertir
    /// @return L'énumération correspondante
    /////////////////////////////////////////////////
    static Nature sto_nature(std::string_view type);

    /////////////////////////////////////////////////
    /// @brief Convertir un string en valeur
    ///
    /// @param type La chaîne à convertir
    /// @return La valeur correspondante
    /////////////////////////////////////////////////
    static VariantValeur sto_valeur(Nature type, std::string valeur);

    /////////////////////////////////////////////////
    /// @brief Créer une variable à partir d'un string
    ///
    /// @param type La chaîne à convertir
    /// @return La variable correspondante
    /////////////////////////////////////////////////
    static Variable parse(std::string valeur);

    // Constructeurs à partir du type et de la valeur
    Variable(Nature type = Nature::Nul, VariantValeur valeur = {});
    Variable(Nature type, std::string valeur);

    // Constructeurs à partir de la valeur (type déduite)
    Variable(VariantValeur valeur);
    Variable(std::string valeur);

    // Constructeur à partir d'un noeud
    template <typename T>
    Variable(std::unique_ptr<T>& noeud) : Variable(sto_nature(noeud->type), noeud->string()) {}

    Nature get_nature() const;
    VariantValeur get_valeur() const;

    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& o, const Variable& v) { return o << v.to_string(); }

    bool est(Nature type) const;

private:
    Nature m_type;          ///< Le type de la variable
    VariantValeur m_valeur; ///< La valeur de la variable
};

} // namespace stretch