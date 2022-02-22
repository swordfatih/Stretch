#include <iostream>
#include <algorithm>
#include <optional>

#include "../include/Arithmetique.hpp"

namespace pe = tao::pegtl;

// std::string out;
// pe::parse<stretch::grammaire, stretch::action>(in, out);

template <typename T>
void print_tree(std::unique_ptr<T>& root)
{
    pe::parse_tree::print_dot(std::cout, *root);
}

// template <typename T>
// void parse_operateur(const std::string_view& operateur)

template <typename Noeud>
int evaluer(std::unique_ptr<Noeud>& noeud) {
    return noeud->children.empty() ? std::stoi(noeud->string()) : stretch::arithmetique::operation<int>(noeud->type, evaluer(noeud->children[0]), evaluer(noeud->children[1]));
}

int main()
{
    std::string filename = "/home/stretch/text";
    pe::file_input in(filename);

    auto root = pe::parse_tree::parse<stretch::operation_ou, stretch::selector>(in);

    print_tree(root);
    std::cout << evaluer(root->children[0]) << std::endl;

    return 0;
}
