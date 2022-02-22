#include <tao/pegtl/demangle.hpp>

#include <functional>
#include <map>

#include "stretch/Grammaire.hpp"

namespace pe = tao::pegtl;

namespace stretch::arithmetique {

template <typename T>
static std::map<std::string_view, std::function<T(const T, const T)>> operations = {
    {
        pe::demangle<stretch::plus>(), 
        [](const T first, const T second) {
            return first + second;
        }
    },
    {
        pe::demangle<stretch::moins>(), 
        [](const T first, const T second) {
            return first - second;
        }
    },
    {
        pe::demangle<stretch::fraction>(), 
        [](const T first, const T second) {
            return first / second;
        }
    },
    {
        pe::demangle<stretch::facteur>(), 
        [](const T first, const T second) {
            return first * second;
        }
    },
    {
        pe::demangle<stretch::modulo>(), 
        [](const T first, const T second) {
            return first % second;
        }
    }
};

template <typename T>
const T operation(const std::string_view& operateur, const T first, const T second) {
    return operations<T>[operateur](first, second);
}

} // namespace stretch::arithmetique