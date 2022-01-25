#include <tao/pegtl.hpp>

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

/// Operateurs arithmétiques
struct fleche_gauche : pe::string< '<', '-' > {};
struct fraction : pe::string< '/' > {};
struct facteur : pe::string< '*' > {};
struct moins : pe::string< '-' > {};
struct modulo : pe::string< '%' > {};
struct plus : pe::string< '+' > {};

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

struct boucle_nombre : pe::seq<repeter, pe::plus<pe::space>, pe::plus<pe::digit>, pe::plus<pe::space>, fois> {};

struct expression;

struct operande : pe::sor< pe::plus<pe::digit>, expression > {};
struct addition : pe::seq< operande, pe::plus<pe::space>, pe::plus<plus, pe::plus<pe::space>, operande> > {};

// struct addition : pe::seq< pe::plus<pe::digit>, pe::plus<pe::space>, plus, pe::plus<pe::space>, pe::plus<pe::digit> > {};

struct soustraction : pe::seq< pe::plus<pe::digit>, pe::plus<pe::space>, moins, pe::plus<pe::space>, pe::plus<pe::digit> > {};

struct expression : pe::sor< addition, soustraction > {};

struct affectation : pe::seq<pe::plus<pe::alpha>, pe::plus<pe::space>, fleche_gauche, pe::plus<pe::space>, expression >  {};

struct grammaire : pe::star<pe::seq<affectation, pe::eol>> {};

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