/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>
#include <tao/pegtl/contrib/analyze.hpp>

#include "stretch/Execution.hpp"
#include "stretch/Standard.hpp"
#include "stretch/Erreurs.hpp"

#include <iostream>

/////////////////////////////////////////////////
namespace pe = tao::pegtl;

/////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    if(argc != 2) {
        std::cerr << "No path given as option" << std::endl;
        return 0;
    }

    if(pe::analyze< stretch::grammaire >(1)) {
        return 1;
    }

    std::string filename{argv[1]};
    pe::file_input in(filename); 

    try
    {
        auto root = pe::parse_tree::parse< stretch::grammaire, stretch::selector, pe::nothing, stretch::control >( in );
        pe::parse_tree::print_dot(std::cout, *root);
        
        stretch::standard::charger();
        
        stretch::Fonction::enregistrer("main", stretch::Fonction(root));
        
        stretch::Scope scope;
        executer(root, scope);
    }
    catch(const pe::parse_error& e)
    {
        std::cout << stretch::exception::StretchException::format(e, in.line_at(e.positions().front())) << std::endl;
    }
    catch(const stretch::exception::Quitter& e) 
    {            
        std::cerr << e.what() << std::endl;                 
        return 1;
    }
    
    return 0;
}
