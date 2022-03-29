#ifndef ERREURS_HPP
#define ERREURS_HPP

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <tao/pegtl.hpp>

#include "stretch/Grammaire.hpp"

/////////////////////////////////////////////////
namespace pe = tao::pegtl;

/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
template< typename > inline constexpr const char* error_message = nullptr;
template<> inline constexpr auto error_message< operation > = "Il faut mettre une valeur ou une operation ici";
template<> inline constexpr auto error_message< operations > = "Il faut mettre une ou plusieurs valeurs ou operations ici";

template<> inline constexpr auto error_message< identifieur > = "Il faut mettre un nom de variable ici (verifie que le nom n'est pas un mot reserve)";

template<> inline constexpr auto error_message< pe::sor< mot::fin > > = "Il faut mettre 'fin' pour terminer le bloc";
template<> inline constexpr auto error_message< pe::sor< mot::fin, pe::at< mot::sinon > > > = "Il faut 'fin' pour terminer la condition";
template<> inline constexpr auto error_message< pe::sor< pe::eof > > = "Le fichier n'a pas de fin ?!";

template<> inline constexpr auto error_message< instruction > = "Ce n'est pas une instruction valide";

template<> inline constexpr auto error_message< mot::fois > = "Il faut le mot 'fois' pour indiquer le nombre de fois que l'on doit boucler";
template<> inline constexpr auto error_message< mot::chaque > = "Il faut le mot 'chaque' apres 'pour'";
template<> inline constexpr auto error_message< mot::dans > = "Il faut le mot 'dans' suivi d'une variable pour ranger la valeur";
template<> inline constexpr auto error_message< mot::que > = "Il faut le mot 'que' apres 'tant'";

template<> inline constexpr auto error_message< identifieurs > = "Il faut mettre un ou plusieurs parametres ici";

/////////////////////////////////////////////////
struct error
{
    template< typename Rule > static constexpr bool raise_on_failure = false;

    template< typename Rule >
    static constexpr auto message = error_message< Rule >;
};

/////////////////////////////////////////////////
template< typename Rule >
using control = pe::must_if< error >::control< Rule >;

} // namespace stretch

#endif // ERREURS_HPP