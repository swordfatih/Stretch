#include <iostream>
#include <algorithm>

#include "../include/Actions.hpp"
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

namespace pe = tao::pegtl;

std::string as_string(const std::string& filename)
{
   pe::file_input in(filename);

   std::string out;
   pe::parse<stretch::grammaire, stretch::action>(in, out);

   // pe::parse_tree::print_dot( std::cout, *root );

   return out;
}

int main(void) 
{
   std::cout << "Hello" << std::endl;
   std::cout << "Test" << std::endl;

   std::cout << as_string("/home/stretch/text") << std::endl;

   return 0;
}
