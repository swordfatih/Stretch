#include <cstdint>
#include <variant>
#include <string>

struct Variable
{
    std::variant<uint64_t, double, std::string, bool> variable; 
    //Counting references
    uint16_t nb_references;
};


//variable en elle-même + le reste