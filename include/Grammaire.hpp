#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

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

/// Blocs
struct debut_commentaire : pe::string<'/', '*'> {};
struct fin_commentaire : pe::string<'*', '/'> {};
struct parenthese_ouvrante : pe::one< '(' > {};
struct parenthese_fermante : pe::one< ')' > {};

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

// struct operation_unaire : pe::list<> {};

struct operation;

struct variable : pe::identifier {};

struct entre_parentheses : pe::seq< parenthese_ouvrante, operation, parenthese_fermante > {};
struct valeur : pe::seq< separateur, pe::sor< nombre, entre_parentheses, variable >, separateur > {};

struct operation_produit : pe::list< valeur, pe::sor < facteur, fraction, modulo > > {};
struct operation_somme : pe::list< operation_produit, pe::sor < plus, moins > > {};
struct operation_ordre : pe::list< operation_somme, pe::sor < plus_grand_que, plus_petit_que > > {};
struct operation_egal : pe::list< operation_ordre, pe::sor < egal > > {};
struct operation_et : pe::list< operation_egal, pe::sor < et > > {}; 
struct operation_ou : pe::list< operation_et, pe::sor < ou > > {};

struct operation : operation_ou {};

struct grammaire : operation {};

// struct expression_f : pe::opt< pe::seq< operateur_arithmetique, separateur, nombre, separateur, expression_f > > {};
// struct expression : pe::seq< nombre, separateur, expression_f > {};

// struct expression_f : pe::sor< pe::seq<nombre, separateur, operateur_arithmetique, separateur, expression_f>, nombre > {};
// struct expression : pe::seq<nombre, separateur, operateur_arithmetique, separateur, expression_f> {}; 

struct rearrange : pe::parse_tree::apply< rearrange > 
{
    template< typename Node, typename... States >
    static void transform( std::unique_ptr< Node >& n, States&&... st )
    {
        if( n->children.size() == 1 ) {
            n = std::move( n->children.back() );
        }
        else {
            n->remove_content();
            auto& c = n->children;
            auto r = std::move( c.back() );
            c.pop_back();
            auto o = std::move( c.back() );
            c.pop_back();
            o->children.emplace_back( std::move( n ) );
            o->children.emplace_back( std::move( r ) );
            n = std::move( o );

            transform( n->children.front(), st... );
        }
    }
};

template< typename Rule >
using selector = tao::pegtl::parse_tree::selector< Rule,
    tao::pegtl::parse_tree::store_content::on<
        nombre,
        plus,
        moins,
        facteur,
        fraction,
        modulo,
        ou,
        et,
        plus_grand_que,
        plus_petit_que,
        egal
    >,
    rearrange::on<
        operation_ou,
        operation_et,
        operation_egal,
        operation_ordre,
        operation_somme,
        operation_produit
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