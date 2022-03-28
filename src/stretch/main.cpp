/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>
#include <tao/pegtl/contrib/analyze.hpp>

#include "stretch/Execution.hpp"
#include "stretch/Standard.hpp"

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

    auto root = pe::parse_tree::parse< stretch::grammaire, stretch::selector >(in);

    pe::parse_tree::print_dot(std::cout, *root);

    try
    {
        stretch::standard::charger();
        
        auto& bloc = root->children.front();
        stretch::Fonction::enregistrer("main", stretch::Fonction(bloc));
        
        stretch::Scope scope;
        executer(bloc, scope);
    }
    catch(const stretch::exception::Quitter& e) 
    {            
        std::cerr << e.what() << std::endl;                 
        return 0;
    }
    
    return 0;
}
