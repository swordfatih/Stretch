#include <tao/pegtl/demangle.hpp>

#include <functional>
#include <map>

#include "../include/Grammaire.hpp"

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

const int operation(const std::string_view& operateur, const std::optional<int> first, const std::optional<int> second) {
    if(first == std::nullopt)
        return second.value();
    else if(second == std::nullopt)
        return first.value();

    return operations<int>[operateur](first.value(), second.value());
}

} // namespace stretch::arithmetique