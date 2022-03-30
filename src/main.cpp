/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>
#include <tao/pegtl/contrib/analyze.hpp>

/////////////////////////////////////////////////
#include "stretch/Execution.hpp"
#include "stretch/Standard.hpp"
#include "stretch/Erreurs.hpp"
#include "stretch/Output.hpp"

/////////////////////////////////////////////////
#include <algorithm> // pour les arguments

/////////////////////////////////////////////////
namespace pe = tao::pegtl;

/////////////////////////////////////////////////
/// @brief Récuperer une option donnée en
/// argument de la ligne de commande
/////////////////////////////////////////////////
std::string get_option(char** begin, char** end, const std::string& option)
{
    char ** itr = std::find(begin, end, option);

    if (itr != end && ++itr != end)
    {
        return std::string(*itr);
    }

    return {};
}

/////////////////////////////////////////////////
/// @brief Vérifier si une option dans les
/// arguments de la ligne de commande existe
/////////////////////////////////////////////////
bool option_exists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

/////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    /////////////////////////////////////////////////
    /// Desactiver les couleurs
    /////////////////////////////////////////////////
    if(option_exists(argv, argv + argc, "-c")) 
    {
        stretch::output::disable_color = true;
    }

    /////////////////////////////////////////////////
    /// Affichage des options
    /////////////////////////////////////////////////
    if(option_exists(argv, argv + argc, "-h") || argc == 1) 
    {
        std::string output = fmt::format("Usage: ");
        output += stretch::output::format(fg(fmt::color::powder_blue), "<stretch> -f <fichier> [options]\n");
        output += stretch::output::format(fg(fmt::color::pale_green) | fmt::emphasis::bold, " -f <fichier>");
        output += fmt::format(": fichier a executer\n"); 
        output += stretch::output::format(fg(fmt::color::pale_green) | fmt::emphasis::bold, " -h");
        output += fmt::format(": affiche cette aide\n");
        output += stretch::output::format(fg(fmt::color::pale_green) | fmt::emphasis::bold, " -v");
        output += fmt::format(": affiche la version\n");
        output += stretch::output::format(fg(fmt::color::pale_green) | fmt::emphasis::bold, " -t");
        output += fmt::format(": affiche l'arbre de la syntaxe abstraite\n");
        output += stretch::output::format(fg(fmt::color::pale_green) | fmt::emphasis::bold, " -s");
        output += fmt::format(": desactive les fonctions standards\n");
        output += stretch::output::format(fg(fmt::color::pale_green) | fmt::emphasis::bold, " -g");
        output += fmt::format(": desactive l'analyse de la grammaire\n");
        output += stretch::output::format(fg(fmt::color::pale_green) | fmt::emphasis::bold, " -c");
        output += fmt::format(": desactiver les couleurs\n"); 

        fmt::print("{}\n", output);

        return 0;
    }

    /////////////////////////////////////////////////
    /// Affichage de la version
    /////////////////////////////////////////////////
    if(option_exists(argv, argv + argc, "-v")) 
    {
        fmt::print("{}\n", stretch::output::format(fg(fmt::color::honey_dew), "Stretch. Version: 0.0.1"));
        return 0;
    }

    /////////////////////////////////////////////////
    /// Précondition, analyse de la grammaire
    /////////////////////////////////////////////////
    if(!option_exists(argv, argv + argc, "-g") && pe::analyze< stretch::grammaire >(1)) 
    {
        fmt::print(stderr, "La grammaire n'est pas valide");
        return 1;
    }

    /////////////////////////////////////////////////
    /// Execution du fichier donné en argument
    /////////////////////////////////////////////////
    std::string filename = { get_option(argv, argv + argc, "-f") };

    if(!filename.empty())
    {
        pe::file_input in(filename); 

        try
        {
            /////////////////////////////////////////////////
            /// Génération de l'arbre de la syntaxe abstraite
            /////////////////////////////////////////////////
            auto root = pe::parse_tree::parse< stretch::grammaire, stretch::selector, pe::nothing, stretch::control >( in );
            
            /////////////////////////////////////////////////
            /// Affichage de l'arbre de la syntaxe abstraite
            /////////////////////////////////////////////////
            if(option_exists(argv, argv + argc, "-t"))
            {
                pe::parse_tree::print_dot(std::cout, *root);
            }
            else
            {
                /////////////////////////////////////////////////
                /// Chargement des fonctions standards si
                /// l'option -s n'est pas donnée en argument
                /////////////////////////////////////////////////
                if(!option_exists(argv, argv + argc, "-s"))
                {
                    stretch::standard::charger();
                }
                
                stretch::Fonction::enregistrer("main", stretch::Fonction(root));
                
                stretch::Scope scope;
                executer(root, scope);
            }
        }
        /////////////////////////////////////////////////
        /// Exception pour quitter le programme
        /////////////////////////////////////////////////
        catch(const stretch::exception::Quitter& e) 
        {            
            return 0;
        }
        /////////////////////////////////////////////////
        /// Exception pour les erreurs de syntaxe
        /////////////////////////////////////////////////
        catch(const pe::parse_error& e)
        {
            stretch::exception::Runtime::print(e, in.line_at(e.positions().front()));
            return 0;
        }
    }
    
    return 0;
}
