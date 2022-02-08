#include <cstdint>

struct Variable
{
    std::variant<uint64_t, double, std::string, std::bool> variable; //t'es un bon
    //Counting references
    uint16_t nb_references;
};


//variable en elle-même + le reste