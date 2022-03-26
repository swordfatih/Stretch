/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <tao/pegtl/contrib/parse_tree.hpp>
#include "stretch/Mots.hpp"

/////////////////////////////////////////////////
namespace pe = tao::pegtl;

/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
/// Separateurs
/////////////////////////////////////////////////
struct espaces : pe::plus< pe::space > {};
struct commentaires : pe::seq< mot::commentaire, pe::sor < pe::seq< mot::commentaire, pe::until< pe::rep< 2, mot::commentaire > > >, pe::until< pe::sor < pe::eol, pe::eof > > > > {};
struct separateur : pe::star< pe::sor < commentaires, espaces, pe::eol > > {}; 

/////////////////////////////////////////////////
/// Valeurs
/////////////////////////////////////////////////
struct variable : pe::identifier {};
struct alias : pe::seq< pe::one< '@' >, variable > {};
struct identifieur : pe::sor< alias, variable > {};

struct entier : pe::list< pe::plus< pe::digit >, mot::apostrophe > {};
struct reel : pe::seq< entier, pe::opt< mot::point, entier > > {}; ///< ie. 4'500.5 
struct booleen : pe::sor < mot::vrai, mot::faux > {}; ///< ie. vrai
struct chaine : pe::star< pe::not_at< guillemets >, pe::sor< pe::alnum, pe::space > > {};
struct texte : pe::seq< mot::guillemets, chaine, mot::guillemets > {}; ///< ie. "hello"

// struct affectation : pe::seq< variable, fleche_gauche, operation > {};
// struct objet : pe::seq< crochet_ouvrant, pe::list< pe::sor< affectation, objet >, virgule >, crochet_fermant > {};
// struct acces : pe::seq< identifieur, de, objet > {};

struct priorite : pe::seq< mot::parenthese_ouvrante, struct operation, mot::parenthese_fermante > {};
struct tableau : pe::seq< mot::crochet_ouvrant, struct operations, mot::crochet_fermant > {};

struct valeur : pe::sor< priorite, booleen, identifieur, tableau, reel, texte > {};

/////////////////////////////////////////////////
/// Operations
/////////////////////////////////////////////////
struct negation : pe::sor< operateur::non > {};
struct addition : pe::sor< symbole::addition > {};
struct soustraction : pe::sor< symbole::soustraction > {};
struct division : pe::sor< symbole::division > {};
struct multiplication : pe::sor< symbole::multiplication > {};
struct reste : pe::sor< symbole::modulo, mot::reste > {};
struct inferieur : pe::seq< mot::plus, separateur, mot::petit, separateur, mot::que > {};
struct superieur : pe::seq< mot::plus, separateur, mot::grand, separateur, mot::que > {};
struct egalite : pe::sor< mot::egal, mot::est > {};
struct inegalite : pe::sor< mot::different, pe::seq< mot::nest, separateur, mot::pas > > {};
struct indexation : pe::sor< mot::indice > {};
struct ou : pe::sor < mot::ou > {};
struct et : pe::sor < mot::et > {};

struct appel;
struct operation_appel : pe::seq< separateur, valeur, separateur, pe::opt < appel > > {};
struct operation_unaire : pe::seq< pe::opt< separateur, pe::sor< negation, addition, soustraction > >, operation_fonction > {};
struct operation_indice : pe::list< operation_unaire, indexation > {};
struct operation_produit : pe::list< operation_indice, pe::sor< multiplication, division, reste > > {};
struct operation_somme : pe::list< operation_produit, pe::sor< addition, soustraction > > {};
struct operation_ordre : pe::list< operation_somme, pe::seq < pe::sor< inferieur, superieur > /*, pe::opt< separateur, ou, separateur, egalite > */ > > {};
struct operation_egalite : pe::list< operation_ordre, pe::sor< egalite, inegalite > > {};
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
/// Assignation
/////////////////////////////////////////////////
struct assignation : pe::seq< pe::list< identifieur, virgule >, separateur, fleche_gauche, separateur, operations > {}; // a, b, c <- 2, 5, 10

/////////////////////////////////////////////////
/// Conditions
///////////////////////////////////////////////// 
struct bloc;

struct condition : pe::seq< si, operation, pe::opt< alors, separateur >, bloc, 
                        pe::star< sinon, separateur, si, operation, pe::opt< alors, separateur >, bloc >,
                        pe::opt< sinon, separateur, bloc >,
                        fin > {};

/////////////////////////////////////////////////
/// Boucles
/////////////////////////////////////////////////
struct ranger : pe::opt< dans, separateur, identifieur, separateur > {}; // ranger la valeur actuelle dans une variable
struct boucle_repeter : pe::seq< repeter, operation, fois, separateur, ranger, bloc, fin > {};
struct boucle_tant_que : pe::seq< tant, separateur, que, operation, pe::opt< faire, separateur >, bloc, fin > {};
struct boucle_pour_chaque : pe::seq< pour, separateur, chaque, separateur, operation, separateur, dans, separateur, operation, separateur, pe::opt< faire, separateur >, bloc, fin > {};
struct boucle : pe::sor< boucle_repeter, boucle_pour_chaque, boucle_tant_que > {};

/////////////////////////////////////////////////
/// Fonctions
/////////////////////////////////////////////////
struct retour : pe::seq< pe::sor< sortir, retourner >, pe::opt< separateur, avec, separateur, operations > > {};

struct parametres : pe::opt< fleche_gauche, separateur, pe::list< pe::seq < variable, separateur >, virgule > > {};
struct definition : pe::seq< fonction, separateur, variable, separateur, parametres, separateur, bloc, fin > {};

struct appel : pe::seq< parenthese_ouvrante, separateur, pe::opt< operations >, separateur, parenthese_fermante > {};

/////////////////////////////////////////////////
/// Blocs d'instructions
/////////////////////////////////////////////////
struct instruction : pe::sor< retour, quitter, arreter, definition, condition, boucle, assignation > {};
struct bloc : pe::star< instruction, separateur > {};

/////////////////////////////////////////////////
/// Grammaire
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
        negation,
        
        // operations binaires
        indexation,
        addition,
        soustraction,
        multiplication,
        division,
        reste,
    
        // operations logiques
        superieur,
        inferieur,
        egalite,
        inegalite,
        ou,
        et,

        // boucles       
        boucle_tant_que,
        boucle_repeter,
        boucle_pour_chaque,
        arreter,

        // fonctions
        definition,
        parametres,
        appel,
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
        operation_appel
    > >;

} // namespace stretch

