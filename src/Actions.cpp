#include <tao/pegtl.hpp>
#include <iostream>

struct my_grammar : tao::pegtl::star<tao::pegtl::any>{};

template<typename Rule>
struct my_action {
    template<typename ActionInput>
    static void apply(const ActionInput& in, std::string& out)
    {
        std::cout << "APPEL DE APPLY" << std::endl;
        out += in.string();
    }
};
