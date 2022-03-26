#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

namespace pe = tao::pegtl;

namespace stretch {

/////////////////////////////////////////////////
/// @brief Mots clés
/////////////////////////////////////////////////

/// Structuration
struct fin : pe::istring< 'f', 'i', 'n' > {};
struct que : pe::istring< 'q', 'u', 'e' > {}; 

/// Boucles
struct repeter : pe::istring< 'r', 'e', 'p', 'e', 't', 'e', 'r' > {};
struct tant : pe::istring< 't', 'a', 'n', 't' > {};
struct fois : pe::istring< 'f', 'o', 'i', 's' > {};
struct arreter : pe::istring< 'a', 'r', 'r', 'e', 't', 'e', 'r' > {};
struct faire : pe::istring< 'f', 'a', 'i', 'r', 'e' > {};
struct continuer : pe::istring< 'c', 'o', 'n', 't', 'i', 'n', 'u', 'e', 'r' > {};
struct dans : pe::istring< 'd', 'a', 'n', 's' > {};

struct pour : pe::istring< 'p', 'o', 'u', 'r' > {};
struct chaque : pe::istring< 'c', 'h', 'a', 'q', 'u', 'e' > {};

/// Comparateurs
struct plus : pe::istring< 'p', 'l', 'u', 's' > {}; 
struct petit: pe::istring< 'p', 'e', 't', 'i', 't' > {};
struct grand : pe::istring< 'g', 'r', 'a', 'n', 'd' > {}; 
struct egal : pe::istring< 'e', 'g', 'a', 'l' > {};
struct est : pe::istring< 'e', 's', 't' > {};
struct different : pe::istring< 'd', 'i', 'f', 'f', 'e', 'r', 'e', 'n', 't' > {};
struct nest : pe::istring< 'n', '\'', 'e', 's', 't' > {};
struct pas : pe::istring< 'p', 'a', 's' > {};

/// Conditions
struct si : pe::istring< 's', 'i' > {};
struct sinon : pe::istring< 's', 'i', 'n', 'o', 'n' > {};
struct alors : pe::istring< 'a', 'l', 'o', 'r', 's' > {};

/// Assignation
struct fleche_gauche : pe::string< '<', '-' > {};

/// Valeurs
struct vrai : pe::istring< 'v', 'r', 'a', 'i' > {};
struct faux : pe::istring< 'f', 'a', 'u', 'x' > {};

// Namespace operateur
namespace operateur {

/// Operateurs arithmétiques
struct plus : pe::one< '+' > {};
struct moins : pe::one< '-' > {};
struct fraction : pe::one< '/' > {};
struct facteur : pe::one< '*' > {};
struct modulo : pe::one< '%' > {};

/// Operateurs logiques
struct et : pe::istring< 'e', 't' > {};
struct ou : pe::istring< 'o', 'u' > {};
struct non : pe::istring < 'n', 'o', 'n' > {};

/// Operateur d'indexation
struct indice : pe::one< '#' > {};

/// Operateur d'accès
struct de : pe::istring< 'd', 'e' > {};

} // namespace operateur

/// Fonctions
struct debut_fonction : pe::one< ':' > {};
struct fonction : pe::istring< 'f', 'o', 'n', 'c', 't', 'i', 'o', 'n' > {};
struct sortir : pe::istring< 's', 'o', 'r', 't', 'i', 'r' > {};
struct retourner : pe::istring< 'r', 'e', 't', 'o', 'u', 'r', 'n', 'e', 'r' > {};
struct avec : pe::istring< 'a', 'v', 'e', 'c' > {};
struct quitter : pe::istring< 'q', 'u', 'i', 't', 't', 'e', 'r' > {};
struct afficher : pe::istring< 'a', 'f', 'f', 'i', 'c', 'h', 'e','r' > {};

/// Commentaires
struct commentaire : pe::one< '|' > {};

/// Tableau
struct crochet_ouvrant : pe::one< '[' > {};
struct crochet_fermant : pe::one< ']' > {};

/// Priorités
struct parenthese_ouvrante : pe::one< '(' > {};
struct parenthese_fermante : pe::one< ')' > {};

/// Ponctuation
struct virgule : pe::one< ',' > {};
struct point : pe::one< '.' > {};
struct guillemets : pe::one< '"' > {};
struct apostrophe : pe::one< '\'' > {};
 
/////////////////////////////////////////////////
/// @brief Separateurs
/////////////////////////////////////////////////
struct espaces : pe::plus< pe::space > {};
struct commentaires : pe::seq< commentaire, pe::sor < pe::seq< commentaire, pe::until< pe::rep< 2, commentaire > > >, pe::until< pe::sor < pe::eol, pe::eof > > > > {};
struct separateur : pe::star< pe::sor < commentaires, espaces, pe::eol > > {}; 

/////////////////////////////////////////////////
/// @brief Valeurs
/////////////////////////////////////////////////
struct variable : pe::identifier {};
struct alias : pe::seq< pe::one< '@' >, variable > {};

struct identifieur : pe::sor< alias, variable > {};

struct entier : pe::list< pe::plus< pe::digit >, apostrophe > {};
struct reel : pe::seq< entier, pe::opt< point, entier > > {}; ///< ie. 4'500.5 
struct booleen : pe::sor < vrai, faux > {}; ///< ie. vrai

struct chaine : pe::star< pe::not_at< guillemets >, pe::sor< pe::alnum, pe::space > > {};
struct texte : pe::seq< guillemets, chaine, guillemets > {}; ///< ie. "hello"

// struct affectation : pe::seq< variable, fleche_gauche, operation > {};
// struct objet : pe::seq< crochet_ouvrant, pe::list< pe::sor< affectation, objet >, virgule >, crochet_fermant > {};
// struct acces : pe::seq< identifieur, de, objet > {};

struct operation;
struct operations;

struct tableau : pe::seq< crochet_ouvrant, operations, crochet_fermant > {};

struct parentheses : pe::seq< parenthese_ouvrante, operation, parenthese_fermante > {};
struct valeur : pe::sor< parentheses, booleen, identifieur, tableau, reel, texte > {};

/////////////////////////////////////////////////
/// @brief Operateurs
/////////////////////////////////////////////////
struct egalite : pe::sor< egal, est > {};
struct difference : pe::sor< different, pe::seq< nest, separateur, pas > > {};
struct inferieur : pe::seq< plus, separateur, petit, separateur, que > {};
struct superieur : pe::seq< plus, separateur, grand, separateur, que > {};

struct appel_fonction;
struct operation_fonction : pe::seq< separateur, valeur, separateur, pe::opt < appel_fonction > > {};
struct operation_unaire : pe::seq< pe::opt< separateur, pe::sor< operateur::non, operateur::plus, operateur::moins > >, operation_fonction > {};
struct operation_indice : pe::list< operation_unaire, pe::sor< operateur::indice > > {};
struct operation_produit : pe::list< operation_indice, pe::sor< operateur::facteur, operateur::fraction, operateur::modulo > > {};
struct operation_somme : pe::list< operation_produit, pe::sor< operateur::plus, operateur::moins > > {};
struct operation_ordre : pe::list< operation_somme, pe::sor< inferieur, superieur > > {};
struct operation_egalite : pe::list< operation_ordre, pe::sor< egalite, difference > > {};
struct operation_et : pe::list< operation_egalite, pe::sor< operateur::et > > {}; 
struct operation_ou : pe::list< operation_et, pe::sor< operateur::ou > > {};

struct operation : operation_ou {};
struct operations : pe::list< operation, virgule > {};

struct rearrange_operation : pe::parse_tree::apply< rearrange_operation > 
{
    template< typename Node, typename... States >
    static void transform( std::unique_ptr< Node >& noeud, States&&... st )
    {
        // pour les opérations non utilisés
        if( noeud->children.size() == 1 ) {
            noeud = std::move( noeud->children.back() );
        } 
        // pour les opérations unaires
        else if( noeud->template is_type< operation_unaire >() && noeud->children.size() == 2 ) {
            noeud->remove_content();
            auto& fils = noeud->children;

            auto valeur = std::move( fils.back() );
            fils.pop_back();

            auto operateur = std::move( fils.back() );
            fils.pop_back();

            noeud = std::move( operateur );
            noeud->children.emplace_back( std::move( valeur ) );
        }
        // pour les appels de fonctions
        else if( noeud->template is_type< operation_fonction >() ) {
            noeud->remove_content();
            auto& fils = noeud->children;

            auto parametres = std::move( fils.back() );
            fils.pop_back();

            auto identifieur = std::move( fils.back() );
            fils.pop_back();  

            noeud = std::move( parametres );
            noeud->children.insert( noeud->children.begin(), std::move( identifieur ) );
        }
        // pour les opérations binaires
        else {
            noeud->remove_content();
            auto& fils = noeud->children;

            auto droite = std::move( fils.back() );
            fils.pop_back();

            auto operateur = std::move( fils.back() );
            fils.pop_back();

            operateur->children.emplace_back( std::move( noeud ) );
            operateur->children.emplace_back( std::move( droite ) );

            noeud = std::move( operateur );
            transform( noeud->children.front(), st... );
        }
    }
};

/////////////////////////////////////////////////
/// @brief Assignation
/////////////////////////////////////////////////
struct assignation : pe::seq< pe::list< identifieur, virgule >, separateur, fleche_gauche, separateur, operations > {}; // a, b, c <- 2, 5, 10

/////////////////////////////////////////////////
/// @brief Conditions
///////////////////////////////////////////////// 
struct bloc;

struct condition : pe::seq< si, operation, pe::opt< alors, separateur >, bloc, 
                        pe::star< sinon, separateur, si, operation, pe::opt< alors, separateur >, bloc >,
                        pe::opt< sinon, separateur, bloc >,
                        fin > {};

/////////////////////////////////////////////////
/// @brief Boucles
/////////////////////////////////////////////////
struct ranger : pe::opt< dans, separateur, identifieur, separateur > {}; // ranger la valeur actuelle dans une variable
struct boucle_repeter : pe::seq< repeter, operation, fois, separateur, ranger, bloc, fin > {};
struct boucle_tant_que : pe::seq< tant, separateur, que, operation, pe::opt< faire, separateur >, bloc, fin > {};
struct boucle_pour_chaque : pe::seq< pour, separateur, chaque, separateur, operation, separateur, dans, separateur, operation, separateur, pe::opt< faire, separateur >, bloc, fin > {};
struct boucle : pe::sor< boucle_repeter, boucle_pour_chaque, boucle_tant_que > {};

/////////////////////////////////////////////////
/// @brief Fonctions
/////////////////////////////////////////////////
struct retour : pe::seq< pe::sor< sortir, retourner >, pe::opt< separateur, avec, separateur, operations > > {};

struct parametres : pe::opt< fleche_gauche, separateur, pe::list< pe::seq < variable, separateur >, virgule > > {};
struct definition_fonction : pe::seq< fonction, separateur, variable, separateur, parametres, separateur, bloc, fin > {};

struct valeur_parametres : pe::opt< operations > {};
struct appel_fonction : pe::seq< parenthese_ouvrante, separateur, valeur_parametres, separateur, parenthese_fermante > {};

/////////////////////////////////////////////////
/// @brief Blocs d'instructions
/////////////////////////////////////////////////
struct instruction : pe::sor< retour, quitter, arreter, definition_fonction, condition, boucle, assignation > {};
struct bloc : pe::star< instruction, separateur > {};

/////////////////////////////////////////////////
/// @brief Grammaire
/////////////////////////////////////////////////
struct fichier : pe::seq< pe::bof, separateur, pe::until< pe::eof, bloc > > {};
struct grammaire : fichier {};

// store_content, remove_content, apply
template< typename Rule >
using selector = tao::pegtl::parse_tree::selector< Rule,
    tao::pegtl::parse_tree::store_content::on<
        // valeurs 
        reel,
        booleen,
        chaine,
        variable,
        alias,
        tableau,

        // operation unaire
        operateur::non,
        operateur::plus,
        operateur::moins,
        
        // operations binaires
        operateur::indice,
        operateur::facteur,
        operateur::fraction,
        operateur::modulo,
    
        // operations logiques
        superieur,
        inferieur,
        egalite,
        difference,
        operateur::ou,
        operateur::et,

        // boucles       
        boucle_tant_que,
        boucle_repeter,
        boucle_pour_chaque,
        arreter,

        // fonctions
        definition_fonction,
        parametres,
        appel_fonction,
        valeur_parametres,
        retour,

        // instructions
        assignation,
        condition, 
        bloc,
        quitter
    >,
    rearrange_operation::on<
        operation,
        operation_ou,
        operation_et,
        operation_egalite,
        operation_ordre,
        operation_somme,
        operation_produit,
        operation_unaire,
        operation_fonction
    > >;

} // namespace stretch

