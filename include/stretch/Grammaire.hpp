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
struct fonction : pe::string< 'f', 'o', 'n', 'c', 't', 'i', 'o', 'n' > {};
struct retourner : pe::string< 'r', 'e', 't', 'o', 'u', 'r', 'n', 'e', 'r' > {};
struct quitter : pe::string< 'q', 'u', 'i', 't', 't', 'e', 'r'> {};
struct afficher : pe::string< 'a', 'f', 'f', 'i', 'c', 'h', 'e','r'> {};

/// Blocs
struct debut_commentaire : pe::string<'/', '*'> {};
struct fin_commentaire : pe::string<'*', '/'> {};
struct parenthese_ouvrante : pe::one< '(' > {};
struct parenthese_fermante : pe::one< ')' > {};

/**
 * @brief Langage
 * 
 */
struct espaces : pe::plus<pe::space> {};
struct commentaire : pe::seq<debut_commentaire, pe::until<fin_commentaire>> {};
struct separateur : pe::star<pe::sor<commentaire, espaces>> {}; 

/**
 * @brief Operations
 * 
 */
struct nombre : pe::plus<pe::digit> {};

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

struct rearrange_operation : pe::parse_tree::apply< rearrange > 
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

/**
 * @brief Condi
 * 
 */



/**
 * @brief Grammaire
 * 
 */
struct grammaire : operation {};

// store_content, remove_content, apply

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
        egal,
        variable
    >,
    rearrange_operation::on<
        operation_ou,
        operation_et,
        operation_egal,
        operation_ordre,
        operation_somme,
        operation_produit
    > >;