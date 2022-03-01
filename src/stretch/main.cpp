#include <iostream>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>
#include "stretch/Evaluation.hpp"

#include "stretch/Variable.hpp"

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

    std::cout << stretch::Variable("salutt") << std::endl;
    std::cout << stretch::Variable(45) << std::endl;
    std::cout << stretch::Variable(75000.8) << std::endl;
    std::cout << stretch::Variable(stretch::Reel, "45487551215455555555.55555555555555555555555") << std::endl;
    std::cout << stretch::Variable(root->children[0]->children[1]->children[0]) << std::endl;
    std::cout << stretch::Variable::parse("23442.98").get_nature() == stretch::Reel << std::endl;

    return 0;
}
