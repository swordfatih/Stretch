#include <iostream>
#include <algorithm>

#include "../include/Grammaire.hpp"

namespace pe = tao::pegtl;

auto get_tree_root(const std::string& filename)
{
   pe::file_input in(filename);

   // std::string out;
   // pe::parse<stretch::grammaire, stretch::action>(in, out);

   auto root = pe::parse_tree::parse<stretch::expression, stretch::selector>( in );
   pe::parse_tree::print_dot( std::cout, *root );

   return root;
}

int main() 
{
   auto root = get_tree_root("/home/stretch/text");

   for(auto& node : root->children) {
      std::cout << node.get() << std::endl;
      std::cout << node->has_content() << std::endl;
      auto s = node->string();
      printf("%s", s);
   }

   return 0;
}
