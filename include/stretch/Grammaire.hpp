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
struct pour_chaque : pe::istring<'p', 'o', 'u', 'r', ' ', 'c', 'h', 'a', 'q', 'u', 'e'> {};
struct continuer : pe::istring<'c', 'o', 'n', 't', 'i', 'n', 'u', 'e', 'r'> {};

/// Comparateurs
struct plus_grand_que : pe::istring< 'p', 'l', 'u', 's', ' ', 'g', 'r', 'a', 'n', 'd', ' ', 'q', 'u', 'e' > {};
struct plus_petit_que : pe::istring< 'p', 'l', 'u', 's', ' ', 'p', 'e', 't', 'i', 't', ' ', 'q', 'u', 'e' > {};
struct egal : pe::sor<pe::istring< 'e', 'g', 'a', 'l' >, pe::istring<'e', 's', 't'>> {};
struct different : pe::sor<pe::istring<'d', 'i', 'f', 'f', 'e', 'r', 'e', 'n', 't'>, pe::istring<'n', '\'', 'e', 's', 't', ' ', 'p', 'a', 's'>> {};

/// Conditions
struct si : pe::istring< 's', 'i' > {};
struct sinon : pe::istring< 's', 'i', 'n', 'o', 'n' > {};
struct alors : pe::istring< 'a', 'l', 'o', 'r', 's' > {};

/// Operateur d'affectation
struct fleche_gauche : pe::string< '<', '-' > {};

/// Operateurs arithmétiques
struct fraction : pe::one< '/' > {};
struct facteur : pe::one< '*' > {};
struct moins : pe::one< '-' > {};
struct modulo : pe::one< '%' > {};
struct plus : pe::one< '+' > {};

/// Operateurs logiques
struct et : pe::istring< 'e', 't' > {};
struct ou : pe::istring< 'o', 'u' > {};
struct non : pe::istring < 'n', 'o', 'n' > {};
struct vrai : pe::istring< 'v', 'r', 'a', 'i' > {};
struct faux : pe::istring< 'f', 'a', 'u', 'x' > {};

/// Operateur d'indexation
struct hashtag : pe::one< '#' > {};

/// Operateur d'accès

struct de : pe::istring< 'd', 'e' > {};

/// Fonctions
struct fonction : pe::istring< 'f', 'o', 'n', 'c', 't', 'i', 'o', 'n' > {};
struct retourner : pe::istring< 'r', 'e', 't', 'o', 'u', 'r', 'n', 'e', 'r' > {};
struct quitter : pe::istring< 'q', 'u', 'i', 't', 't', 'e', 'r' > {};
struct afficher : pe::istring< 'a', 'f', 'f', 'i', 'c', 'h', 'e','r' > {};

/// Commentaires
struct debut_commentaire : pe::string< '|', '|' > {};
struct fin_commentaire : pe::string< '|', '|' > {};
struct mono_commentaire : pe::one< '|' > {};

/// Priorités
struct parenthese_ouvrante : pe::one< '(' > {};
struct parenthese_fermante : pe::one< ')' > {};


/// Tableaux
struct crochet_ouvrant : pe::one<'['> {};
struct crochet_fermant : pe::one<']'> {};

/// Ponctuation
struct virgule : pe::one< ',' > {};
struct point : pe::one< '.' > {};
struct guillemets : pe::one< '"' > {};
struct apostrophe : pe::one< '\'' > {};

/////////////////////////////////////////////////
/// @brief Separateurs
/////////////////////////////////////////////////
struct espaces : pe::plus< pe::space > {};
struct commentaire : pe::sor<pe::seq< debut_commentaire, pe::until< fin_commentaire > >, pe::seq<mono_commentaire, pe::until<pe::eol>>> {};
struct separateur : pe::star< pe::sor < commentaire, espaces, pe::eol >> {}; 

/////////////////////////////////////////////////
/// @brief Valeurs
/////////////////////////////////////////////////
struct expression {}; //TODO à faire

struct variable : pe::identifier {};
struct alias : pe::seq<pe::one<'@'>, variable> {};

struct identifieur : pe::sor<alias, variable> {};

struct suite_chiffres : pe::list<pe::digit, pe::star<apostrophe>> {};
struct nombre : pe::seq<pe::opt<pe::sor<plus, moins>>, suite_chiffres, pe::opt<point, suite_chiffres>> {};           ///< ie. 4'500.5 

struct chaine : pe::seq<guillemets, pe::until<guillemets>> {};              ///< ie. "hello"
struct booleen : pe::sor <vrai, faux> {};                                     ///< ie. vrai

struct operation;
struct parentheses : pe::seq< parenthese_ouvrante, operation, parenthese_fermante > {};
struct valeur : pe::sor< variable, entier, reel, chaine, booleen, parentheses > {};

struct liste_expression : pe::list<expression, virgule, separateur> {};


struct tableau : pe::seq<crochet_ouvrant, liste_expression, crochet_fermant> {};

/////////////////////////////////////////////////
/// @brief Assignation
/////////////////////////////////////////////////

struct affectation : pe::seq<variable, fleche_gauche, expression> {};
struct assignation : pe::seq<pe::list<identifieur, virgule>, fleche_gauche, pe::list<expression, virgule>> {};


/////////////////////////////////////////////////
/// @brief Objets
/////////////////////////////////////////////////

struct objet : pe::seq<crochet_ouvrant, pe::list<pe::sor<affectation, objet>, virgule>, crochet_fermant> {};

/////////////////////////////////////////////////
/// @brief Opérateurs
/////////////////////////////////////////////////

struct indexation : pe::seq<expression, hashtag, expression> {};

struct acces : pe::seq<expression, de, expression> {};

/////////////////////////////////////////////////
/// @brief Operateurs
/////////////////////////////////////////////////

// Opérateurs arithmétiques

struct operation_produit : pe::list< pe::seq< separateur, valeur, separateur >, pe::sor < facteur, fraction, modulo > > {};
struct operation_somme : pe::list< operation_produit, pe::sor < plus, moins > > {};
struct operation_ordre : pe::list< operation_somme, pe::sor < plus_grand_que, plus_petit_que > > {};


struct operation : operation_ou {};

// Opérateurs de comparaison

struct comparaison_pgq : pe::seq<expression, plus_grand_que, expression> {};
struct comparaison_ppq : pe::seq<expression, plus_petit_que, expression> {};
struct comparaison_egal : pe::seq<expression, egal, expression> {};
struct comparaison_different : pe::seq<expression, different, expression> {};

// Opérateurs logiques

struct operateur_et : pe::seq<expression, et, expression> {};
struct operateur_ou : pe::seq<expression, ou, expression> {};
struct operateur_non : pe::seq<non, expression> {}};


/////////////////////////////////////////////////
/// @brief Structures de contrôle
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/// @brief Blocs d'instructions
/////////////////////////////////////////////////

struct debut_bloc : pe::sor< pe::bof, faire, alors > {};
struct fin_bloc : pe::sor< pe::eof, fin > {};
struct instruction : pe::sor< assignation, condition > {};
struct bloc : pe::seq< debut_bloc, separateur, pe::star< instruction, separateur >, separateur, pe::until< fin_bloc > > {};


// Condition

struct condition_si : pe::seq<si, expression, pe::opt<alors>, bloc, 
                        pe::star<sinon, si, expression, pe::opt<alors>, bloc>,
                        pe::opt<sinon, bloc>,
                        fin> {};
// Boucles  

struct boucle_tant_que : pe::seq<tant_que, expression, faire, bloc> {};
struct boucle_repeter : pe::seq<repeter, expression, fois, pe::opt<dans, identifieur>, bloc> {};
struct boucle_pour_chaque : pe::seq<pour_chaque, identifieur, dans, expression, faire, bloc²> {};


/////////////////////////////////////////////////
/// @brief Fonctions
/////////////////////////////////////////////////


struct definition_fonction : pe::seq<fonction, identifieur, pe::opt<fleche_gauche, liste_expression>, pe::one<':'>, bloc> {};
struct appel_fonction : pe::seq<identifieur, parenthese_ouvrante, pe::opt<liste_expression>, parenthese_fermante> {};



/////////////////////////////////////////////////
/// @brief Grammaire
/////////////////////////////////////////////////
struct grammaire : bloc {};

// store_content, remove_content, apply
template< typename Rule >
using selector = tao::pegtl::parse_tree::selector< Rule,
    tao::pegtl::parse_tree::store_content::on<
        // operations
        valeur,
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

/*

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

*/

} // namespace stretch