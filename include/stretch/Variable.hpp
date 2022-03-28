#pragma once 

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <variant>
#include <string>
#include <memory>
#include <regex>
#include <unordered_map>

#include <tao/pegtl/demangle.hpp>

#include "decimal/BigDecimal.h"
#include "stretch/Arbre.hpp"

/////////////////////////////////////////////////
using Noeud = tao::pegtl::parse_tree::node;

/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
/// @brief Les différents types de valeur
/////////////////////////////////////////////////
enum class Nature {
    Nul = -1,
    Chaine,
    Booleen,
    Reel,
    Tableau,
    Objet
};

/////////////////////////////////////////////////
class Variable;

/////////////////////////////////////////////////
/// @brief Les types utilisés en mémoire pour
/// représenter les différents types de valeurs
/////////////////////////////////////////////////
using Tableau = std::vector< Variable >;

using Objet = std::unordered_map<std::string, Variable>;

using VariantValeur = std::variant< std::string, bool, BigDecimal, Tableau, Objet>;

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
    /// @brief Convertir le type en string
    ///
    /// @param type Le type à convertir
    /// @return La chaîne correspondante
    /////////////////////////////////////////////////
    static std::string type_tos(Nature type);

    /////////////////////////////////////////////////
    /// @brief Convertir un string en valeur
    ///
    /// @param type Le type de la valeur
    /// @param valeur La chaîne à affecter
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
    Variable(std::unique_ptr<Noeud>& noeud);

    Nature get_nature() const;
    VariantValeur get_valeur() const;

    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& o, const Variable& v) { return o << v.to_string(); }

    bool est(Nature type) const;

private:
    /////////////////////////////////////////////////
    static void remplacer(std::string& source, const std::string& from, const std::string& to);

    /////////////////////////////////////////////////   
    static std::vector<std::string> split_structure(const std::string& s, const std::regex& regex); 

    Nature m_type;          ///< Le type de la variable
    VariantValeur m_valeur; ///< La valeur de la variable

};

} // namespace stretch