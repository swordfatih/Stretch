#include <iostream>
#include <algorithm>

#include "../include/Grammaire.hpp"

namespace pe = tao::pegtl;

// std::string out;
// pe::parse<stretch::grammaire, stretch::action>(in, out);

template <typename T>
void print_tree(std::unique_ptr<T>* root)
{
    pe::parse_tree::print_dot(std::cout, **root);
}

template <typename T>
int evaluer(std::unique_ptr<T>* root, bool first = false) {
    auto& noeud = **root;

    if(noeud.children.size() == 0)
        return noeud.string().size() == 0 ? 0 : std::stoi(noeud.string());
    
    std::string operateur = first ? "+" : noeud.children[0]->string();
    int valeur = std::stoi(first ? noeud.children[0]->string() : noeud.children[1]->string()) * (operateur == "-" ? -1 : 1);
    auto& expression = noeud.children.back();

    return valeur + evaluer(&expression);
}

int main()
{
    std::string filename = "/home/stretch/text";
    pe::file_input in(filename);

    auto root = pe::parse_tree::parse<stretch::expression, stretch::selector>(in);

    print_tree(&root);
    std::cout << evaluer(&root, true) << std::endl;

    return 0;
}
