/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/unescape.hpp>

#include "stretch/Grammaire.hpp"

/////////////////////////////////////////////////
namespace pe = tao::pegtl;

/////////////////////////////////////////////////
namespace stretch {

/////////////////////////////////////////////////
/// @brief Structure pour réarranger les noeuds 
/// des différents opérateurs
/////////////////////////////////////////////////
struct rearrange_operations : pe::parse_tree::apply< rearrange_operations > 
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
        else if( noeud->template is_type< operation_appel >() ) {
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
/// @brief Structure pour réarranger les noeuds 
/// des séquences d'échappements
/////////////////////////////////////////////////
struct rearrange_echappements : pe::parse_tree::apply< rearrange_echappements > 
{
    template< typename Node, typename... States >
    static void transform( std::unique_ptr< Node >& noeud, States&&... st )
    {
        
    }
};

/////////////////////////////////////////////////
/// @brief Selectionner les noeuds utiles
///
/// @see store_content, remove_content, apply
/////////////////////////////////////////////////
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
        taille,
        nature,
        
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
        tant_que,
        repeter,
        pour_chaque,
        mot::arreter,
        mot::continuer,

        // fonctions
        definition,
        parametres,
        appel,
        retour,

        // instructions
        assignation,
        condition, 
        bloc,
        mot::quitter,
        entree,
        sortie
    >,
    rearrange_operations::on<
        operation,
        operation_ou,
        operation_et,
        operation_egalite,
        operation_ordre,
        operation_somme,
        operation_produit,
        operation_unaire,
        operation_appel
    >, 
    rearrange_echappements::on< 
        /*echapper_x,
        echapper_u,
        echapper_U,*/
        echapper_c
    > 
>;

} // namespace stretch