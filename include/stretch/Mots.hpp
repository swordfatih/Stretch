#ifndef MOTS_HPP
#define MOTS_HPP

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <tao/pegtl/ascii.hpp>
#include <tao/pegtl/rules.hpp>
#include <tao/pegtl/utf8.hpp>

/////////////////////////////////////////////////
namespace pe = tao::pegtl;

/////////////////////////////////////////////////
namespace stretch::mot {

/////////////////////////////////////////////////
/// Mots clés
/////////////////////////////////////////////////
namespace symbole {
    struct plus : pe::one< '+' > {};
    struct moins : pe::one< '-' > {};
    struct division : pe::one< '/' > {};
    struct fois : pe::one< '*' > {};
    struct modulo : pe::one< '%' > {};
} // namespace symbole

/// Commentaires
struct commentaire : pe::one< '|' > {};

/// Structures de contrôle
struct fin : pe::istring< 'f', 'i', 'n' > {};
struct tant : pe::istring< 't', 'a', 'n', 't' > {};
struct que : pe::istring< 'q', 'u', 'e' > {}; 
struct repeter : pe::istring< 'r', 'e', 'p', 'e', 't', 'e', 'r' > {};
struct fois : pe::istring< 'f', 'o', 'i', 's' > {};
struct dans : pe::istring< 'd', 'a', 'n', 's' > {};
struct pour : pe::istring< 'p', 'o', 'u', 'r' > {};
struct chaque : pe::istring< 'c', 'h', 'a', 'q', 'u', 'e' > {};
struct faire : pe::istring< 'f', 'a', 'i', 'r', 'e' > {};
struct arreter : pe::istring< 'a', 'r', 'r', 'e', 't', 'e', 'r' > {};
struct continuer : pe::istring< 'c', 'o', 'n', 't', 'i', 'n', 'u', 'e', 'r' > {};
struct si : pe::istring< 's', 'i' > {};
struct sinon : pe::istring< 's', 'i', 'n', 'o', 'n' > {};
struct alors : pe::istring< 'a', 'l', 'o', 'r', 's' > {};

/// Assignation
struct fleche : pe::string< '<', '-' > {};

/// Fonctions
struct fonction : pe::istring< 'f', 'o', 'n', 'c', 't', 'i', 'o', 'n' > {};
struct sortir : pe::istring< 's', 'o', 'r', 't', 'i', 'r' > {};
struct retourner : pe::istring< 'r', 'e', 't', 'o', 'u', 'r', 'n', 'e', 'r' > {};
struct avec : pe::istring< 'a', 'v', 'e', 'c' > {};
struct quitter : pe::istring< 'q', 'u', 'i', 't', 't', 'e', 'r' > {};

/// Valeurs
struct vrai : pe::istring< 'v', 'r', 'a', 'i' > {};
struct faux : pe::istring< 'f', 'a', 'u', 'x' > {};
struct unicode : pe::utf8::range< 0x20, 0x10FFFF > {};

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

/// Echappement
struct echappeur : pe::one< '\\' > {};
struct echapper : pe::one< '"', 'n' > {};

/// Operateurs de forme
struct taille : pe::istring< 't', 'a', 'i', 'l', 'l', 'e' > {};
struct nature : pe::istring< 'n', 'a', 't', 'u', 'r', 'e' > {};

/// Operateurs arithmétiques
struct reste : pe::istring< 'r', 'e', 's', 't', 'e' > {};

/// Operateurs logiques
struct et : pe::istring< 'e', 't' > {};
struct ou : pe::istring< 'o', 'u' > {};
struct non : pe::istring < 'n', 'o', 'n' > {};

/// Operateur d'indexation
struct indice : pe::one< '#' > {};

/// Operateur d'accès
struct de : pe::istring< 'd', 'e' > {};

// Operateurs de comparaison
struct plus : pe::istring< 'p', 'l', 'u', 's' > {}; 
struct petit : pe::istring< 'p', 'e', 't', 'i', 't' > {};
struct grand : pe::istring< 'g', 'r', 'a', 'n', 'd' > {}; 
struct egal : pe::istring< 'e', 'g', 'a', 'l' > {};
struct est : pe::istring< 'e', 's', 't' > {};
struct different : pe::istring< 'd', 'i', 'f', 'f', 'e', 'r', 'e', 'n', 't' > {};
struct nest : pe::istring< 'n', '\'', 'e', 's', 't' > {};
struct pas : pe::istring< 'p', 'a', 's' > {};

// Instructions
struct afficher : pe::istring< 'a', 'f', 'f', 'i', 'c', 'h', 'e', 'r' > {};
struct lire : pe::istring< 'l', 'i', 'r', 'e' > {};

// Mot clés
struct cles : pe::sor< symbole::plus, symbole::moins, symbole::division, symbole::fois, symbole::modulo, commentaire, fin, tant, 
    que, repeter, fois, dans, pour, chaque, faire, arreter, continuer, si, sinon, alors, fleche, fonction, sortir, retourner, avec, quitter,
    vrai, faux, crochet_ouvrant, crochet_fermant, parenthese_ouvrante, parenthese_fermante, virgule, point, guillemets, apostrophe, echappeur,
    taille, nature, reste, et, ou, non, indice, de, plus, petit, grand, egal, est, different, nest, pas, afficher, lire > {};

} // namespace stretch::mot

#endif // MOTS_HPP