/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

#include "stretch/Execution.hpp"

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

    std::string filename{argv[1]};
    pe::file_input in(filename); 

    auto root = pe::parse_tree::parse<stretch::grammaire, stretch::selector>(in);

    pe::parse_tree::print_dot(std::cout, *root);

    try
    {
        stretch::Fonction::enregistrer("main", stretch::Fonction(root));
        stretch::Scope scope;
        stretch::executer(root, scope);
    }
    catch(const stretch::exception::Quitter& e) 
    {            
        std::cerr << e.what() << std::endl;                 
        return 0;
    }
    
    return 0;
}
