/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
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
// identifieurs
struct variable : pe::identifier {};
struct alias : pe::seq< pe::one< '@' >, variable > {};
struct identifieur : pe::sor< alias, variable > {};

// nombres
struct entier : pe::list< pe::plus< pe::digit >, mot::apostrophe > {};
struct reel : pe::seq< entier, pe::opt< mot::point, entier > > {}; ///< ie. 4'500.5 
struct booleen : pe::sor < mot::vrai, mot::faux > {}; ///< ie. vrai
struct priorite : pe::seq< mot::parenthese_ouvrante, struct operation, mot::parenthese_fermante > {};

// littéraux
struct caractere : pe::if_then_else< mot::echappeur, pe::opt< mot::echapper >, mot::unicode > {};
struct chaine : pe::star< pe::not_at< mot::guillemets >, pe::sor< caractere, pe::eol > > {};
struct texte : pe::seq< mot::guillemets, chaine, mot::guillemets > {}; ///< ie. "hello"

// listes
// struct affectation : pe::seq< variable, fleche_gauche, operation >  {};
// struct objet : pe::seq< crochet_ouvrant, pe::list< pe::sor< affectation, objet >, virgule >, crochet_fermant > {};
// struct acces : pe::seq< identifieur, de, objet > {};
struct tableau : pe::seq< mot::crochet_ouvrant, struct operations, mot::crochet_fermant > {};

// valeur
struct valeur : pe::sor< priorite, booleen, identifieur, tableau, reel, texte > {};

/////////////////////////////////////////////////
/// Operations
/////////////////////////////////////////////////
struct negation : pe::sor< mot::non > {};
struct taille : pe::seq< mot::taille, separateur, mot::de > {};
struct nature : pe::seq< mot::nature, separateur, mot::de > {};
struct addition : pe::sor< mot::symbole::plus > {};
struct soustraction : pe::sor< mot::symbole::moins > {};
struct division : pe::sor< mot::symbole::division > {};
struct multiplication : pe::sor< mot::symbole::fois > {};
struct reste : pe::sor< mot::symbole::modulo, mot::reste > {};
struct inferieur : pe::seq< mot::plus, separateur, mot::petit, separateur, mot::que > {};
struct superieur : pe::seq< mot::plus, separateur, mot::grand, separateur, mot::que > {};
struct egalite : pe::sor< mot::egal, mot::est > {};
struct inegalite : pe::sor< mot::different, pe::seq< mot::nest, separateur, mot::pas > > {};
struct indexation : pe::sor< mot::indice > {};
struct ou : pe::sor < mot::ou > {};
struct et : pe::sor < mot::et > {};

struct operation_appel : pe::seq< separateur, valeur, separateur, pe::opt < struct appel, separateur > > {};
struct operation_unaire : pe::seq< pe::opt< separateur, pe::sor< negation, addition, soustraction, taille, nature > >, operation_appel > {};
struct operation_indice : pe::list< operation_unaire, indexation > {};
struct operation_produit : pe::list< operation_indice, pe::sor< multiplication, division, reste > > {};
struct operation_somme : pe::list< operation_produit, pe::sor< addition, soustraction > > {};
struct operation_ordre : pe::list< operation_somme, pe::seq< pe::sor< inferieur, superieur > /*, pe::opt< separateur, ou, separateur, egalite > */ > > {};
struct operation_egalite : pe::list< operation_ordre, pe::sor< egalite, inegalite > > {};
struct operation_et : pe::list< operation_egalite, pe::sor< mot::et > > {}; 
struct operation_ou : pe::list< operation_et, pe::sor< mot::ou > > {};

struct operation : operation_ou {};
struct operations : pe::list< operation, mot::virgule > {};

/////////////////////////////////////////////////
/// Assignation
/////////////////////////////////////////////////
struct assignation : pe::seq< pe::list< identifieur, mot::virgule >, separateur, mot::fleche, separateur, operations > {}; // a, b, c <- 2, 5, 10

/////////////////////////////////////////////////
/// Conditions
///////////////////////////////////////////////// 
struct bloc;
struct condition : pe::seq< mot::si, operation, pe::opt< mot::alors, separateur >, bloc, 
                        pe::star< mot::sinon, separateur, mot::si, operation, pe::opt< mot::alors, separateur >, bloc >,
                        pe::opt< mot::sinon, separateur, bloc >,
                        mot::fin > {};

/////////////////////////////////////////////////
/// Boucles
/////////////////////////////////////////////////
struct ranger : pe::opt< mot::dans, separateur, identifieur, separateur > {}; // ranger la valeur actuelle dans une variable
struct repeter : pe::seq< mot::repeter, operation, mot::fois, separateur, ranger, bloc, mot::fin > {};
struct tant_que : pe::seq< mot::tant, separateur, mot::que, operation, pe::opt< mot::faire, separateur >, bloc, mot::fin > {};
struct pour_chaque : pe::seq< mot::pour, separateur, mot::chaque, separateur, operation, separateur, mot::dans, separateur, operation, separateur, pe::opt< mot::faire, separateur >, bloc, mot::fin > {};
struct boucle : pe::sor< repeter, pour_chaque, tant_que > {};

/////////////////////////////////////////////////
/// Fonctions
/////////////////////////////////////////////////
struct retour : pe::seq< pe::sor< mot::sortir, mot::retourner >, pe::opt< separateur, mot::avec, separateur, operations > > {};

struct parametres : pe::opt< mot::fleche, separateur, pe::list< pe::seq< variable, separateur >, mot::virgule > > {};
struct definition : pe::seq< mot::fonction, separateur, variable, separateur, parametres, separateur, bloc, mot::fin > {};

struct appel : pe::seq< mot::parenthese_ouvrante, separateur, pe::opt< operations >, separateur, mot::parenthese_fermante > {};

/////////////////////////////////////////////////
/// Instructions standards
/////////////////////////////////////////////////
struct sortie : pe::seq< mot::afficher, separateur, operations > {};
struct entree : pe::seq< mot::lire, separateur, mot::dans, separateur, identifieur > {};

/////////////////////////////////////////////////
/// Blocs d'instructions
/////////////////////////////////////////////////
struct instruction : pe::sor< retour, mot::quitter, entree, sortie, mot::arreter, mot::continuer, definition, condition, boucle, assignation > {};
struct bloc : pe::star< instruction, separateur > {};

/////////////////////////////////////////////////
/// Grammaire
/////////////////////////////////////////////////
struct fichier : pe::seq< pe::bof, separateur, pe::until< pe::eof, bloc > > {};
struct grammaire : fichier {};

} // namespace stretch

