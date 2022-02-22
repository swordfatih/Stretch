#include <iostream>

#include "stretch/Evaluation.hpp"

namespace pe = tao::pegtl;

int main()
{
    std::string filename = "/home/stretch/test.txt";
    pe::file_input in(filename);

    auto root = pe::parse_tree::parse<stretch::grammaire, stretch::selector>(in);

    pe::parse_tree::print_dot(std::cout, *root);
    evaluer(root->children[0]);

    return 0;
}
