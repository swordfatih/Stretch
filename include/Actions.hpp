#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

#include "Grammaire.hpp"

namespace pe = tao::pegtl;

namespace stretch {

template<typename Rule>
struct action {};

template<>
struct action <repeter> {
    template <typename Input>
    static void apply(const Input& in, std::string& out) {
        
    }
};

template<>
struct action <boucle_nombre> {
    template <typename ActionInput>
    static void apply(const ActionInput& in, std::string& out) {
        const auto root = pe::parse_tree::parse< stretch::boucle_nombre >( pe::string_input(in.string(), "") );

        for(auto it: in) {
            std::cout << it << std::endl;
        }

        for(auto it = in.begin(); it != in.end(); ++it) {
            std::cout << *it << std::endl;
        }

        int nb = std::stoi(root->children[0]->children[2]->string());
        
        for(int i = 0; i < nb; ++i) {
            std::cout << "je repete " << i << std::endl;
        }
    }
};

int last = 0;

template<>
struct action <operande> {
    template <typename ActionInput>
    static void apply(const ActionInput& in, std::string& out) {
        std::cout << "operande " << in.string() << std::endl;

        auto root = pe::parse_tree::parse< stretch::operande >( pe::string_input(in.string(), "") );

        auto& expr = root->children[0]; // 4

        last += std::stoi(expr->string());
    }
};

template<>
struct action <addition> {
    template <typename ActionInput>
    static void apply(const ActionInput& in, std::string& out) {
        std::cout << "addition " << in.string() << std::endl;

        auto root = pe::parse_tree::parse< stretch::addition >( pe::string_input(in.string(), "") );

        auto& expr = root->children[0]; // a + b

        last = std::stoi(expr->children[0]->string()) + last;
    }
};

template<>
struct action <soustraction> {
    template <typename ActionInput>
    static void apply(const ActionInput& in, std::string& out) {
        std::cout << "soustraction" << std::endl;

        auto root = pe::parse_tree::parse< stretch::soustraction >( pe::string_input(in.string(), "") );

        auto& expr = root->children[0]; // a - b

        int a = std::stoi(expr->children[0]->string());
        int b = std::stoi(expr->children[4]->string());

        last = a - b;
    }
};

template<>
struct action <affectation> {
    template <typename ActionInput>
    static void apply(const ActionInput& in, std::string& out) {
        std::cout << "affectation" << std::endl;

        std::cout << in.string() << std::endl;

        const auto root = pe::parse_tree::parse< stretch::affectation >( pe::string_input(in.string(), std::string{}) );

        auto& children = root->children[0];

        auto variable = children->children[0]->string();
        int valeur = std::stoi(children->children[4]->string());

        std::cout << "on affecte " << last << " à la variable " << variable << std::endl; 
    }
};

}

// struct my_grammar : tao::pegtl::seq<tao::pegtl::plus< tao::pegtl::digit>, 
//     tao::pegtl::sor< tao::pegtl::ascii::space >
//     > {};
// coucou
// variable <- expression
// expression = addition | ...

// c <- a + b      c <- 5
// a + b

// Interpreteur.get().setVariable(c, Interpreteur.get().getVariable(a) + Interpreteur.get().getVariable(b));
