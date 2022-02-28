#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>


namespace pe = tao::pegtl;

namespace stretch {

/////////////////////////////////////////////////
/// @brief Mots clés
/////////////////////////////////////////////////

/// Structuration
struct fin : pe::istring< 'f', 'i', 'n' > {};

/// Boucles
struct repeter : pe::istring< 'r', 'e', 'p', 'e', 't', 'e', 'r' > {};
struct tant_que : pe::istring< 't', 'a', 'n', 't', ' ', 'q', 'u', 'e' > {};
struct fois : pe::istring< 'f', 'o', 'i', 's' > {};
struct sortir : pe::istring< 's', 'o', 'r', 't', 'i', 'r' > {};
struct faire : pe::istring< 'f', 'a', 'i', 'r', 'e' > {};

/// Comparateurs
struct plus_grand_que : pe::istring< 'p', 'l', 'u', 's', ' ', 'g', 'r', 'a', 'n', 'd', ' ', 'q', 'u', 'e' > {};
struct plus_petit_que : pe::istring< 'p', 'l', 'u', 's', ' ', 'p', 'e', 't', 'i', 't', ' ', 'q', 'u', 'e' > {};
struct egal : pe::istring< 'e', 'g', 'a', 'l' > {};

/// Conditions
struct si : pe::istring< 's', 'i' > {};
struct sinon : pe::istring< 's', 'i', 'n', 'o', 'n' > {};
struct alors : pe::istring< 'a', 'l', 'o', 'r', 's' > {};

/// Operateur d'affectation
struct fleche_gauche : pe::istring< '<', '-' > {};

/// Operateurs arithmétiques
struct fraction : pe::one< '/' > {};
struct facteur : pe::one< '*' > {};
struct moins : pe::one< '-' > {};
struct modulo : pe::one< '%' > {};
struct plus : pe::one< '+' > {};
struct operateur_arithmetique : pe::sor<fraction, facteur, moins, modulo, plus> {};

/// Operateurs logiques
struct et : pe::istring< 'e', 't' > {};
struct ou : pe::istring< 'o', 'u' > {};
struct non : pe::istring < 'n', 'o', 'n' > {};
struct est : pe::istring< 'e', 's', 't' > {};
struct vrai : pe::istring< 'v', 'r', 'a', 'i' > {};
struct faux : pe::istring< 'f', 'a', 'u', 'x' > {};

/// Fonctions
struct fonction : pe::istring< 'f', 'o', 'n', 'c', 't', 'i', 'o', 'n' > {};
struct retourner : pe::istring< 'r', 'e', 't', 'o', 'u', 'r', 'n', 'e', 'r' > {};
struct quitter : pe::istring< 'q', 'u', 'i', 't', 't', 'e', 'r' > {};
struct afficher : pe::istring< 'a', 'f', 'f', 'i', 'c', 'h', 'e','r' > {};

/// Commentaires
struct debut_commentaire : pe::istring< '|', '|' > {};
struct fin_commentaire : pe::istring< '|', '|' > {};
struct mono_commentaire : pe::one< '|' > {};

/// Priorités
struct parenthese_ouvrante : pe::one< '(' > {};
struct parenthese_fermante : pe::one< ')' > {};

/// Ponctuation
struct virgule : pe::one<','> {};
struct point : pe::one<'.'> {};

/////////////////////////////////////////////////
/// @brief Separateurs
/////////////////////////////////////////////////
struct espaces : pe::plus< pe::space > {};
struct commentaire : pe::seq< debut_commentaire, pe::until< fin_commentaire > > {};
struct separateur : pe::star< pe::sor < commentaire, espaces, pe::eol >> {}; 

/////////////////////////////////////////////////
/// @brief Operations
/////////////////////////////////////////////////
struct nombre : pe::plus<pe::digit> {};
struct variable : pe::identifier {};

struct operation;
struct entre_parentheses : pe::seq< parenthese_ouvrante, operation, parenthese_fermante > {};
struct valeur : pe::seq< separateur, pe::sor< nombre, entre_parentheses, variable >, separateur > {};

struct operation_produit : pe::list< valeur, pe::sor < facteur, fraction, modulo > > {};
struct operation_somme : pe::list< operation_produit, pe::sor < plus, moins > > {};
struct operation_ordre : pe::list< operation_somme, pe::sor < plus_grand_que, plus_petit_que > > {};
struct operation_egal : pe::list< operation_ordre, pe::sor < egal > > {};
struct operation_et : pe::list< operation_egal, pe::sor < et > > {}; 
struct operation_ou : pe::list< operation_et, pe::sor < ou > > {};

struct operation : operation_ou {};

struct rearrange_operation : pe::parse_tree::apply< rearrange_operation > 
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

/////////////////////////////////////////////////
/// @brief Conditions
/////////////////////////////////////////////////
struct bloc;
struct condition : pe::seq< si, separateur, operation, separateur, bloc > {};

/////////////////////////////////////////////////
/// @brief Assignation
/////////////////////////////////////////////////
struct assignation : pe::seq<variable, separateur, fleche_gauche, separateur, operation> {};

/////////////////////////////////////////////////
/// @brief Blocs d'instructions
/////////////////////////////////////////////////
struct debut_bloc : pe::sor< pe::bof, faire, alors > {};
struct fin_bloc : pe::sor< pe::eof, fin > {};
struct instruction : pe::sor< assignation, condition > {};
struct bloc : pe::seq< debut_bloc, separateur, pe::star< instruction, separateur >, separateur, pe::until< fin_bloc > > {};

/////////////////////////////////////////////////
/// @brief Grammaire
/////////////////////////////////////////////////
struct grammaire : bloc {};

// store_content, remove_content, apply
template< typename Rule >
using selector = tao::pegtl::parse_tree::selector< Rule,
    tao::pegtl::parse_tree::store_content::on<
        // operations
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
        variable,

        // instructions
        bloc,
        assignation,
        condition
    >,
    rearrange_operation::on<
        operation_ou,
        operation_et,
        operation_egal,
        operation_ordre,
        operation_somme,
        operation_produit
    > >;

/////////////////////////////////////////////////
/// @brief Fonctions
/////////////////////////////////////////////////
// struct liste_parametres : pe::list<expression, virgule, separateur> {};
// struct liste_parametres_opt : pe::opt<liste_parametres> {};
// struct instruction_retourner : pe::seq<retourner, liste_parametres_opt> {};
// struct definition_fonction : pe::seq<creer, pe::identifier, pe::opt<fleche_gauche, liste_parametres>, pe::opt<bloc_code>, fin> {}; ///TODO définir instruction (assignement | retourner | quitter | condition | boucle...)
// struct appel_fonction : pe::seq<pe::identifier, parenthese_ouvrante, liste_parametres_opt, parenthese_fermante> {}; //si la fonction ne prend pas de paramètre l'appel se fait uniquement en écrivant son nom, ça fera bizarre IMO

/////////////////////////////////////////////////
/// @brief Boucles
/////////////////////////////////////////////////
// struct boucle_tant_que : pe::seq<tant_que, expression, bloc_code, fin> {};
// struct boucle_repeter_x_fois : pe::seq<repeter, expression, fois, bloc_code, fin> {};

/////////////////////////////////////////////////
/// @brief Chaine
/////////////////////////////////////////////////
// struct chaine : pe::sor<
//     pe::seq<guillemet_double, pe::until<guillemet_double>>,
//     pe::seq<guillemet_simple, pe::until<guillemet_simple>>
// > {};

} // namespace stretch