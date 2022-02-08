#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

#include <functional>
#include <map>

namespace pe = tao::pegtl;

namespace stretch {

/**
 * @brief Mots clés
 *
 */

/// Structuration
struct fin : pe::string< 'f', 'i', 'n' > {};

/// Boucles
struct repeter : pe::istring< 'r', 'e', 'p', 'e', 't', 'e', 'r' > {};
struct tant_que : pe::string< 't', 'a', 'n', 't', ' ', 'q', 'u', 'e' > {};
struct fois : pe::string< 'f', 'o', 'i', 's' > {};
struct sortir : pe::string< 's', 'o', 'r', 't', 'i', 'r'> {};

/// Comparateurs
struct plus_grand_que : pe::string< 'p', 'l', 'u', 's', ' ', 'g', 'r', 'a', 'n', 'd', ' ', 'q', 'u', 'e' > {};
struct plus_petit_que : pe::string< 'p', 'l', 'u', 's', ' ', 'p', 'e', 't', 'i', 't', ' ', 'q', 'u', 'e'> {};
struct egal : pe::string< 'e', 'g', 'a', 'l' > {};

/// Conditions
struct si : pe::string< 's', 'i' > {};
struct sinon : pe::string< 's', 'i', 'n', 'o', 'n' > {};

/// Operateur d'affectation
struct fleche_gauche : pe::string< '<', '-' > {};

template <typename T>
static std::map<std::string_view, std::function<T(const T, const T)>> operations = {
    {
        "stretch::plus", 
        [](const T first, const T second) {
            return first + second;
        }
    },
    {
        "stretch::moins", 
        [](const T first, const T second) {
            return first - second;
        }
    },
    {
        "stretch::fraction", 
        [](const T first, const T second) {
            return first / second;
        }
    },
    {
        "stretch::facteur", 
        [](const T first, const T second) {
            return first * second;
        }
    },
    {
        "stretch::modulo", 
        [](const T first, const T second) {
            return first % second;
        }
    }
};

/// Operateurs arithmétiques
struct fraction : pe::string< '/' > {};
struct facteur : pe::string< '*' > {};
struct moins : pe::string< '-' > {};
struct modulo : pe::string< '%' > {};
struct plus : pe::string< '+' > {};
struct operateur_arithmetique : pe::sor<fraction, facteur, moins, modulo, plus> {};

/// Operateurs logiques
struct et : pe::string< 'e', 't' > {};
struct ou : pe::string< 'o', 'u' > {};
struct non : pe::string < 'n', 'o', 'n' > {};
struct est : pe::string< 'e', 's', 't'> {};
struct vrai : pe::string< 'v', 'r', 'a', 'i' > {};
struct faux : pe::string< 'f', 'a', 'u', 'x' > {};

/// Fonctions
struct creer : pe::string< 'c', 'r', 'e', 'e', 'r' > {};
struct retourner : pe::string< 'r', 'e', 't', 'o', 'u', 'r', 'n', 'e', 'r' > {};
struct quitter : pe::string< 'q', 'u', 'i', 't', 't', 'e', 'r'> {};
struct afficher : pe::string< 'a', 'f', 'f', 'i', 'c', 'h', 'e','r'> {};

/// Commentaires
struct debut_commentaire : pe::string<'/', '*'> {};
struct fin_commentaire : pe::string<'*', '/'> {};

/**
 * @brief Opérateurs
 * 
 */


/**
 * @brief Autre
 * 
 */

/**
 * @brief Grammaire
 * 
 */

// struct boucle_nombre : pe::seq<repeter, pe::plus<pe::space>, pe::plus<pe::digit>, pe::plus<pe::space>, fois> {};

// store_content, remove_content, apply

struct espaces : pe::plus<pe::space> {};
struct commentaire : pe::seq<debut_commentaire, pe::until<fin_commentaire>> {};
struct separateur : pe::star<pe::sor<commentaire, espaces>> {}; 

struct nombre : pe::plus<pe::digit> {};

struct expression_f : pe::opt< pe::seq< operateur_arithmetique, separateur, nombre, separateur, expression_f> > {};
struct expression : pe::seq<nombre, separateur, expression_f> {};

template< typename Rule >
using selector = tao::pegtl::parse_tree::selector< Rule,
    tao::pegtl::parse_tree::store_content::on<
        expression_f,
        nombre,
        plus,
        moins,
        facteur,
        fraction,
        modulo
    > >;

// // struct addition : pe::seq< pe::plus<pe::digit>, pe::plus<pe::space>, plus, pe::plus<pe::space>, pe::plus<pe::digit> > {};

// struct soustraction : pe::seq< pe::plus<pe::digit>, pe::plus<pe::space>, moins, pe::plus<pe::space>, pe::plus<pe::digit> > {};

// ([5 + 2] + 4) + 1

// struct affectation : pe::seq<pe::plus<pe::alpha>, pe::plus<pe::space>, fleche_gauche, pe::plus<pe::space>, expression >  {};

// struct grammaire : pe::star<pe::seq<affectation, pe::eol>> {}; 

// struct variable : pe::star<pe::alpha> {};
// struct plus_minus : pe::opt< pe::one< '+', '-' > > {};  
// struct addition : pe::seq< pe::digit, pe::star<space>, pe::plus_minus, pe::star<space>, pe::digit > {};

// struct dot : pe::one< '.' > {};

/*
struct grammar : star<
        seq<
            star<
                seq<
                    sor<repeter, egal>, 
                    space
                >
            >, 
            eolf
        >
    > {};
*/

}

/*
    struct number
    : tao::pegtl::plus< tao::pegtl::digit > {};

    struct addition;  // Forward declaration to break the cyclic dependency.

    struct bracket
    : tao::pegtl::if_must< tao::pegtl::one< '(' >, addition, tao::pegtl::one< ')' > > {};

    struct atomic
    : tao::pegtl::sor< number, bracket > {};

    struct addition
    : tao::pegtl::list< atomic, tao::pegtl::one< '+' > > {};

    struct inf : seq< istring< 'i', 'n', 'f' >,
                        opt< istring< 'i', 'n', 'i', 't', 'y' > > > {};

    struct nan : seq< istring< 'n', 'a', 'n' >,
                        opt< one< '(' >,
                            plus< alnum >,
                            one< ')' > > > {};

    template< typename D >
    struct number : if_then_else< dot,
                                    plus< D >,
                                    seq< plus< D >, opt< dot, star< D > > > > {};

    struct e : one< 'e', 'E' > {};
    struct p : one< 'p', 'P' > {};
    struct exponent : seq< plus_minus, plus< digit > > {};

    struct decimal : seq< number< digit >, opt< e, exponent > > {};
    struct hexadecimal : seq< one< '0' >, one< 'x', 'X' >, number< xdigit >, opt< p, exponent > > {};

    struct grammar : star<seq< plus<dot>, eolf >> {};

    star<plus<dot>, eolf>
*/