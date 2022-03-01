#include <iostream>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>
#include "stretch/Evaluation.hpp"

namespace pe = tao::pegtl;

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
    // stretch::evaluer(root->children[0]);

    return 0;
}
