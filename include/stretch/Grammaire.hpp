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
struct pour_chaque : pe::istring< 'p', 'o', 'u', 'r', ' ', 'c', 'h', 'a', 'q', 'u', 'e' > {};
struct continuer : pe::istring< 'c', 'o', 'n', 't', 'i', 'n', 'u', 'e', 'r' > {};
struct dans : pe::istring< 'd', 'a', 'n', 's' > {};

/// Comparateurs
struct plus_grand_que : pe::istring< 'p', 'l', 'u', 's', ' ', 'g', 'r', 'a', 'n', 'd', ' ', 'q', 'u', 'e' > {};
struct plus_petit_que : pe::istring< 'p', 'l', 'u', 's', ' ', 'p', 'e', 't', 'i', 't', ' ', 'q', 'u', 'e' > {};
struct egal : pe::sor< pe::istring< 'e', 'g', 'a', 'l' >, pe::istring< 'e', 's', 't' > > {};
struct different : pe::sor< pe::istring< 'd', 'i', 'f', 'f', 'e', 'r', 'e', 'n', 't' >, pe::istring< 'n', '\'', 'e', 's', 't', ' ', 'p', 'a', 's' > > {};

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
struct mono_commentaire : pe::one< '|' > {};
struct double_commentaire : pe::string< '|', '|' > {};

/// Priorités
struct parenthese_ouvrante : pe::one< '(' > {};
struct parenthese_fermante : pe::one< ')' > {};

/// Tableaux
struct crochet_ouvrant : pe::one< '[' > {};
struct crochet_fermant : pe::one< ']' > {};

/// Ponctuation
struct virgule : pe::one< ',' > {};
struct point : pe::one< '.' > {};
struct guillemets : pe::one< '"' > {};
struct apostrophe : pe::one< '\'' > {};

/////////////////////////////////////////////////
/// @brief Separateurs
/////////////////////////////////////////////////
// struct espaces : pe::plus< pe::space > {};
// struct commentaire : pe::sor< pe::seq< double_commentaire, pe::until< double_commentaire > >, pe::seq<mono_commentaire, pe::until<pe::eol>>> {};
// struct separateur : pe::star< pe::sor< commentaire, espaces, pe::eol > > {};

struct espaces : pe::plus< pe::space > {};
struct commentaire : pe::seq< double_commentaire, pe::until< double_commentaire > > {};
struct separateur : pe::star< pe::sor < commentaire, espaces, pe::eol > > {}; 

/////////////////////////////////////////////////
/// @brief Valeurs
/////////////////////////////////////////////////
// struct expression {}; //TODO à faire

struct variable : pe::identifier {};
struct alias : pe::seq< pe::one< '@' >, variable > {};

struct identifieur : pe::sor< alias, variable > {};

// struct entier : pe::list< pe::digit, apostrophe > {};
// struct reel : pe::seq< pe::opt< pe::sor< plus, moins > >, entier, pe::opt< point, entier > > {}; ///< ie. 4'500.5 
// struct chaine : pe::seq< guillemets, pe::until< guillemets > > {}; ///< ie. "hello"
// struct booleen : pe::sor < vrai, faux > {}; ///< ie. vrai

struct entier : pe::plus< pe::seq< pe::digit, pe::star< apostrophe > > > {};    ///< ie. 4'500 
struct reel : pe::seq< entier, point, pe::opt< entier > > {};                   ///< ie. 4'500.5 
struct chaine : pe::seq< guillemets, pe::until< guillemets > > {};              ///< ie. "hello"
struct booleen : pe::sor < vrai, faux > {};                                     ///< ie. vrai

struct operation;
struct parentheses : pe::seq< parenthese_ouvrante, operation, parenthese_fermante > {};
struct valeur : pe::sor< variable, entier, reel, chaine, booleen, parentheses > {};

/////////////////////////////////////////////////
/// @brief Operateurs
/////////////////////////////////////////////////
// struct operation_unaire : pe::seq < pe::opt < pe::sor < non, plus, moins > >, pe::sor < separateur, valeur, separateur > > {};
struct operation_produit : pe::list< pe::seq< separateur, valeur, separateur >, pe::sor < facteur, fraction, modulo > > {};
struct operation_somme : pe::list< operation_produit, pe::sor < plus, moins > > {};
struct operation_ordre : pe::list< operation_somme, pe::sor < plus_grand_que, plus_petit_que > > {};
struct operation_egalite : pe::list< operation_ordre, pe::sor < egal, different > > {};
struct operation_et : pe::list< operation_egalite, pe::sor < et > > {}; 
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
/// @brief Assignation
/////////////////////////////////////////////////
struct liste_valeur : pe::list< valeur, virgule > {};
struct assignation : pe::seq< pe::list< identifieur, virgule >, separateur, fleche_gauche, separateur, liste_valeur > {}; // a, b, c <- 2, 5, 10
// struct assignation : pe::seq<variable, separateur, fleche_gauche, separateur, operation> {};

/////////////////////////////////////////////////
/// @brief Objets et tableaux
/////////////////////////////////////////////////
struct affectation : pe::seq< variable, fleche_gauche, operation > {};

struct tableau : pe::seq< crochet_ouvrant, liste_valeur, crochet_fermant > {};
struct objet : pe::seq< crochet_ouvrant, pe::list< pe::sor< affectation, objet >, virgule >, crochet_fermant > {};

struct indexation : pe::seq< tableau, hashtag, operation > {};
struct acces : pe::seq< identifieur, de, objet > {};

/////////////////////////////////////////////////
/// @brief Conditions
/////////////////////////////////////////////////
// struct condition : pe::seq< si, operation, pe::opt< alors >, bloc, 
//                         // pe::star< sinon, si, operation, pe::opt< alors >, bloc >,
//                         // pe::opt< sinon, bloc >,
//                         fin > {};

// struct fin_condition : pe::sor < fin, sinon > {};
// struct condition : pe::seq< si, operation, pe::opt< alors >, bloc, 
//                         pe::opt< sinon, bloc, pe::at< fin > >,
//                         pe::at < fin > > {};

struct bloc;
struct condition : pe::seq< si, separateur, operation, separateur, bloc, 
                                pe::opt< pe::seq < sinon, separateur, bloc > >
                            > {};

//struct condition : pe::seq< si, separateur, operation, separateur, bloc > {};

/////////////////////////////////////////////////
/// @brief Fonctions
/////////////////////////////////////////////////
struct definition_fonction : pe::seq< fonction, identifieur, pe::opt< fleche_gauche, liste_valeur >, pe::one< ':' >, bloc > {};
struct appel_fonction : pe::seq< identifieur, parenthese_ouvrante, pe::opt< liste_valeur >, parenthese_fermante > {};

/////////////////////////////////////////////////
/// @brief Boucles
/////////////////////////////////////////////////
struct boucle_tant_que : pe::seq< tant_que, operation, faire, bloc > {};
struct boucle_repeter : pe::seq< repeter, operation, fois, pe::opt< dans, identifieur >, bloc > {};
struct boucle_pour_chaque : pe::seq< pour_chaque, identifieur, dans, operation, faire, bloc > {};

/////////////////////////////////////////////////
/// @brief Blocs d'instructions
/////////////////////////////////////////////////
struct debut_bloc : pe::sor< faire, alors > {};
struct fin_bloc : pe::sor< fin, sinon > {};

struct instruction : pe::sor< assignation, condition > {};

struct bloc : pe::seq< debut_bloc, separateur, pe::until< fin_bloc, pe::star< instruction, separateur > > > {};
struct fichier : pe::seq< pe::bof, separateur, pe::until< pe::eof, pe::star< instruction, separateur > > > {};

/////////////////////////////////////////////////
/// @brief Grammaire
/////////////////////////////////////////////////
struct grammaire : fichier {};

// store_content, remove_content, apply
template< typename Rule >
using selector = tao::pegtl::parse_tree::selector< Rule,
    tao::pegtl::parse_tree::store_content::on<
        // valeurs 
        entier,
        reel,
        chaine,
        booleen,

        // operations
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
        //operation_unaire,
        operation_ou,
        operation_et,
        operation_egalite,
        operation_ordre,
        operation_somme,
        operation_produit
    > >;

} // namespace stretch

