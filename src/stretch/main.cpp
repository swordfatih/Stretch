#include <iostream>

#include "stretch/Evaluation.hpp"

namespace pe = tao::pegtl;

int main()
{
    std::string filename = "/home/stretch/text";
    pe::file_input in(filename);

    auto root = pe::parse_tree::parse<stretch::operation_ou, stretch::selector>(in);

    print_tree(root);
    std::cout << evaluer(root->children[0]) << std::endl;

    return 0;
}
