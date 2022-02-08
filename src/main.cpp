#include <iostream>
#include <algorithm>
#include <optional>

#include "../include/Grammaire.hpp"

#include <tao/pegtl/demangle.hpp>

namespace pe = tao::pegtl;

// std::string out;
// pe::parse<stretch::grammaire, stretch::action>(in, out);

template <typename T>
void print_tree(std::unique_ptr<T>* root)
{
    pe::parse_tree::print_dot(std::cout, **root);
}

// template <typename T>
// void parse_operateur(const std::string_view& operateur)

const int operation(const std::string_view& operateur, const std::optional<int> first, const std::optional<int> second) {
    if(first == std::nullopt)
        return second.value();
    else if(second == std::nullopt)
        return first.value();

    // operateur.operation(first, second);

    // if(operateur == pe::demangle<stretch::plus>()) {
    //    return stretch::plus::operation<int>(first.value(), second.value());
    // }
    // else if(operateur == pe::demangle<stretch::moins>()) {
    //    return stretch::moins::operation<int>(first.value(), second.value());
    // }
    // else if(operateur == pe::demangle<stretch::facteur>()) {
    //     return stretch::facteur::operation<int>(first.value(), second.value());
    // }
    // else if(operateur == pe::demangle<stretch::fraction>()) {
    //     return stretch::fraction::operation<int>(first.value(), second.value());
    // }
    // else if(operateur == pe::demangle<stretch::modulo>()) {
    //     return stretch::modulo::operation<int>(first.value(), second.value());
    // }

    std::cout << operateur << std::endl;
    return stretch::operations<int>[operateur](first.value(), second.value());
}

template <typename T>
std::optional<int> evaluer(std::unique_ptr<T>* root, bool first = false) {
    auto& noeud = **root;

    if(noeud.children.size() == 0)
        return noeud.string().size() == 0 ? std::nullopt : std::optional<int>{std::stoi(noeud.string())};

    std::string_view operateur = noeud.children.back()->children.size() != 0 ? noeud.children.back()->children[0]->type : pe::demangle<stretch::plus>();

    int valeur = std::stoi(first ? noeud.children[0]->string() : noeud.children[1]->string());
    auto& expression = noeud.children.back();

    return operation(operateur, std::optional<int>{valeur}, evaluer(&expression));
}

int main()
{
    std::string filename = "/home/stretch/text";
    pe::file_input in(filename);

    auto root = pe::parse_tree::parse<stretch::expression, stretch::selector>(in);

    print_tree(&root);
    std::cout << evaluer(&root, true).value() << std::endl;

    return 0;
}
