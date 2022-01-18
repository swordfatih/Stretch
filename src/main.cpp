#include <iostream>
#include <algorithm>

#include "Grammaire.cpp"
#include "Actions.cpp"

std::string as_string(const std::string& filename)
{
   tao::pegtl::file_input in(filename);

   std::string out;
   tao::pegtl::parse<grammar, my_action>(in, out);

   return out;
}

int main() 
{
   std::cout << "" << " 🏴‍☠️🏴‍☠️🏴‍☠️" << std::endl;
   std::cout << as_string("text") << std::endl;

   return 0;
}
