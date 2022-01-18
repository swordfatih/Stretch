#include <iostream>
#include <algorithm>

#include "Actions.cpp"

template<typename ParseInput>
std::string as_string(ParseInput& in)
{
   std::string out;
   tao::pegtl::parse<my_grammar, my_action>(in, out);

   return out;
}

std::string my_parse(const std::string& filename)
{
   tao::pegtl::file_input in(filename);
   return as_string(in);
}

int main() 
{
   std::cout << "Schtroumpf" << " 🏴‍☠️🏴‍☠️🏴‍☠️" << std::endl;
   std::cout << my_parse("text") << std::endl;

   return 0;
}
