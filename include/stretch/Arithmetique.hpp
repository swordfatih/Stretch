#include <tao/pegtl/demangle.hpp>

#include <functional>
#include <map>

#include "stretch/Variable.hpp"

namespace pe = tao::pegtl;

namespace stretch::arithmetique {

template <typename... N>
static void types_non_supportes(std::string operation, Variable first, Variable second, N... types) 
{
    if(first.est(types...) || second.est(types...)) 
    {
        throw std::runtime_error("Impossible d'effectuer l'operation " + operation + " entre un " 
            + type_tos(first.get_nature()) + " et un " + type_tos(second.get_nature()));
    }   
}

static void types_differents(std::string operation, Variable first, Variable second) 
{
    if(first.get_nature() != second.get_nature()) 
    {
        throw std::runtime_error("Impossible d'effectuer l'operation " + operation + " entre un " 
            + type_tos(first.get_nature()) + " et un " + type_tos(second.get_nature()));
    }   
}

static std::map<std::string_view, std::function<Variable(const Variable, const Variable)>> operations = {
    {
        pe::demangle<stretch::plus>(), 
        [](const Variable first, const Variable second) {
            types_non_supportes("addition", first, second, Nature::Booleen, Nature::Nul);
            types_differents("addition", first, second);
                
            // if(first.est())
        }
    },
    {
        pe::demangle<stretch::moins>(), 
        [](const Variable first, const Variable second) {
            return first - second;
        }
    },
    {
        pe::demangle<stretch::fraction>(), 
        [](const Variable first, const Variable second) {
            return first / second;
        }
    },
    {
        pe::demangle<stretch::facteur>(), 
        [](const Variable first, const Variable second) {
            return first * second;
        }
    },
    {
        pe::demangle<stretch::modulo>(), 
        [](const Variable first, const Variable second) {
            return first % second;
        }
    },
    {
        pe::demangle<stretch::egal>(),
        [](const Variable first, const Variable second) {
            return first == second;
        }
    }
};

template <typename T>
const T operation(const std::string_view& operateur, const T first, const T second) {
    return operations<T>[operateur](first, second);
}

} // namespace stretch::arithmetique