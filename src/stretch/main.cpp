#include <iostream>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>
#include "stretch/Evaluation.hpp"

namespace pe = tao::pegtl;

int main()
{
    std::string filename = "main.st";
    pe::file_input in(filename);

    auto root = pe::parse_tree::parse<stretch::grammaire, stretch::selector>(in);

    pe::parse_tree::print_dot(std::cout, *root);
    // stretch::evaluer(root->children[0]);

    return 0;
}
