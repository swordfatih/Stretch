#include <tao/pegtl.hpp>
#include <iostream>

template<typename Rule>
struct my_action {
    template<typename ActionInput>
    static void apply(const ActionInput& in, std::string& out)
    {
        // std::cout << "APPEL DE APPLY : " << in.string() << std::endl;

        if(in.string() == "repeter") {
             std::cout << "ON REPETE" << std::endl;
        } 

        //out += in.string();
    }
};


// struct my_grammar : tao::pegtl::seq<tao::pegtl::plus< tao::pegtl::digit>, 
//     tao::pegtl::sor< tao::pegtl::ascii::space >
//     > {};