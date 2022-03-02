#include <tao/pegtl/demangle.hpp>

#include <functional>
#include <map>

#include "stretch/Variable.hpp"

namespace pe = tao::pegtl;

namespace stretch::arithmetique {

/////////////////////////////////////////////////
template <typename... N>
static void types_non_supportes(std::string operation, Variable first, Variable second, N... types) 
{
    for (const auto& type : {types...})
    {
        if(first.est(type) || second.est(type))
            throw std::runtime_error("Impossible d'effectuer l'operation " + operation + " entre un " 
                + Variable::type_tos(first.get_nature()) + " et un " + Variable::type_tos(second.get_nature()));        
    }
}

/////////////////////////////////////////////////
static void types_differents(std::string operation, Variable first, Variable second) 
{
    if(first.get_nature() != second.get_nature()) 
    {
        throw std::runtime_error("Impossible d'effectuer l'operation " + operation + " entre un " 
            + Variable::type_tos(first.get_nature()) + " et un " + Variable::type_tos(second.get_nature()));
    }   
}

/////////////////////////////////////////////////
static std::map<std::string_view, std::function<Variable(const Variable, const Variable)>> operations = {
    {
        /////////////////////////////////////////////////
        pe::demangle<stretch::plus>(), 
        [](const Variable first, const Variable second) {
            types_non_supportes("addition", first, second, Nature::Booleen, Nature::Nul);
                
            // "5" + 5 = "55"
            // 5 + "5" = 10

            if(first.est(Nature::Reel) && second.est(Nature::Reel)) {
                return Variable(std::get<BigDecimal>(first.get_valeur()) + std::get<BigDecimal>(second.get_valeur()));
            } 
            else if(first.est(Nature::Chaine) && second.est(Nature::Chaine)) {
                return Variable(std::get<std::string>(first.get_valeur()) + std::get<std::string>(second.get_valeur()));
            } 
            else if(first.est(Nature::Chaine) && second.est(Nature::Reel)) {
                return Variable(std::get<std::string>(first.get_valeur()) + std::get<BigDecimal>(second.get_valeur()).toString());
            } 
            else if(first.est(Nature::Reel) && second.est(Nature::Chaine)) {
                Variable second_var = Variable::parse(std::get<std::string>(second.get_valeur()));

                if(second_var.get_nature() == Nature::Reel)
                    return Variable(std::get<BigDecimal>(first.get_valeur()) + std::get<BigDecimal>(second_var.get_valeur()));
                else
                    return Variable(std::get<BigDecimal>(first.get_valeur()).toString() + std::get<std::string>(second.get_valeur()));
            } 
        
            return Variable();
        }
    },
    {
        /////////////////////////////////////////////////
        pe::demangle<stretch::moins>(), 
        [](const Variable first, const Variable second) {
            types_non_supportes("soustraction", first, second, Nature::Booleen, Nature::Nul);

            // "salut" - 10 = "sal"
            // 10 - "salut" = erreur
            // 10 - "2" = 8

            return first;// - second;
        }
    },
    {
        /////////////////////////////////////////////////
        pe::demangle<stretch::fraction>(), 
        [](const Variable first, const Variable second) {
            types_non_supportes("division", first, second, Nature::Booleen, Nature::Nul);

            // 2 / "2" = 1
            // "salut" / 2 = erreur

            return first;// / second;
        }
    },
    {
        /////////////////////////////////////////////////
        pe::demangle<stretch::facteur>(), 
        [](const Variable first, const Variable second) {
            types_non_supportes("multiplication", first, second, Nature::Booleen, Nature::Nul);

            // "salut" * 2 = "salutsalut"
            // 2 * "5" = 10

            return first;// * second;
        }
    },
    {
        /////////////////////////////////////////////////
        pe::demangle<stretch::modulo>(), 
        [](const Variable first, const Variable second) {
            types_non_supportes("modulo", first, second, Nature::Booleen, Nature::Nul);
        
            // 2 % "10" = 0
            // "salut" % 2 = erreur
           
            return first;// % second;
        }
    },
    {
        /////////////////////////////////////////////////
        pe::demangle<stretch::egal>(),
        [](const Variable first, const Variable second) {

            // 10 == "10" : true
            // "vrai" == vrai : true 
            // "nul" == nul : false

            return Variable(true);// == second;
        }
    },
    {
        /////////////////////////////////////////////////
        pe::demangle<stretch::different>(),
        [](const Variable first, const Variable second) {

            // 10 != "10" : true
            // "vrai" != vrai : true 
            // "nul" != nul : false

            return first;// == second;
        }
    },
    {
        /////////////////////////////////////////////////
        pe::demangle<stretch::et>(),
        [](const Variable first, const Variable second) {
            types_non_supportes("et", first, second, Nature::Nul, Nature::Reel, Nature::Chaine);
            

            return first;// == second;
        }
    },
    {
        /////////////////////////////////////////////////
        pe::demangle<stretch::ou>(),
        [](const Variable first, const Variable second) {
            types_non_supportes("ou", first, second, Nature::Nul, Nature::Reel, Nature::Chaine);
            

            return first;// == second;
        }
    },
    {
        /////////////////////////////////////////////////
        pe::demangle<stretch::plus_grand_que>(),
        [](const Variable first, const Variable second) {
            types_non_supportes("plus grand que", first, second, Nature::Nul, Nature::Booleen);
            
            // "salut" > "abc" 

            return first;// == second;
        }
    },
    {
        /////////////////////////////////////////////////
        pe::demangle<stretch::plus_petit_que>(),
        [](const Variable first, const Variable second) {
            types_non_supportes("plus petit que", first, second, Nature::Nul, Nature::Booleen);
            
            // "salut" < "abc"

            return first;// == second;
        }
    }
};

const Variable operation(const std::string_view& operateur, const Variable first, const Variable second) {
    return operations[operateur](first, second);
}

} // namespace stretch::arithmetique