#include <tao/pegtl/demangle.hpp>

#include <functional>
#include <map>

#include "stretch/Variable.hpp"

namespace pe = tao::pegtl;

namespace stretch::arithmetique {

/////////////////////////////////////////////////
static std::map<    
                    std::string_view, 
                    std::map<   
                                std::pair< Nature, Nature >,
                                std::function< Variable(const Variable, const Variable) > 
                            >
                > 
    operations = {

    /////////////////////////////////////////////////
    // Addition
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::plus>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<BigDecimal>(f.get_valeur()) + std::get<BigDecimal>(s.get_valeur()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<std::string>(f.get_valeur()) + std::get<std::string>(s.get_valeur()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<std::string>(f.get_valeur()) + std::get<BigDecimal>(s.get_valeur()).toString());
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    Variable t = Variable::parse(std::get<std::string>(s.get_valeur()));

                    if(t.est(Nature::Reel))
                        return Variable(std::get<BigDecimal>(f.get_valeur()) + std::get<BigDecimal>(t.get_valeur()));
                    
                    return Variable(std::get<BigDecimal>(f.get_valeur()).toString() + std::get<std::string>(s.get_valeur()));
                }
            }
        }
    },

    /////////////////////////////////////////////////
    // Soustraction
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::moins>(), 
        {
            {
                std::make_pair(Nature::Reel, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<BigDecimal>(f.get_valeur()) - std::get<BigDecimal>(s.get_valeur()));
                }
            },
            {
                std::make_pair(Nature::Chaine, Nature::Reel),
                [](const Variable f, const Variable s) {
                    std::string str = std::get<std::string>(f.get_valeur());
                    BigDecimal n = std::get<BigDecimal>(s.get_valeur());
                    str.erase(str.begin() + str.size() - n.toInt(), str.end());

                    return Variable(str);
                }
            },
            {
                std::make_pair(Nature::Chaine, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    Variable t = Variable::parse(std::get<std::string>(s.get_valeur()));

                    if(t.est(Nature::Reel)) {
                        int n = std::get<BigDecimal>(t.get_valeur()).toInt();

                        std::string str = std::get<std::string>(f.get_valeur());
                        str.erase(str.begin() + str.size() - n, str.end());
                        
                        return Variable(str);
                    }
                        
                    throw std::runtime_error("Vous ne pouvez pas soustraire deux chaines.");
                }
            }
        }
    },
    {
        /////////////////////////////////////////////////
        pe::demangle<stretch::facteur>(), 
        {
            {
                std::make_pair(Nature::Reel, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<BigDecimal>(f.get_valeur()) * std::get<BigDecimal>(s.get_valeur()));
                }
            },
            {
                std::make_pair(Nature::Chaine, Nature::Reel),
                [](const Variable f, const Variable s) {
                    std::string value = std::get<std::string>(f.get_valeur());
                    BigDecimal n = std::get<BigDecimal>(s.get_valeur());

                    std::string repeat;
                    for(BigDecimal i = 0; i < n; i += 1)
                        repeat += value;

                    return Variable(repeat);
                }
            },
            {
                std::make_pair(Nature::Reel, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    std::string value = std::get<std::string>(s.get_valeur());
                    BigDecimal n = std::get<BigDecimal>(f.get_valeur());

                    std::string repeat;
                    for(BigDecimal i = 0; i < n; i += 1)
                        repeat += value;

                    return Variable(repeat);
                }
            }
        }
    } 
        
       /* 
        pe::demangle<stretch::plus>(), 
        [](const Variable first, const Variable second) {
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
    }*/
};

const Variable operation(const std::string_view& operateur, const Variable first, const Variable second) {
    if(operations.find(operateur) == operations.end()) {
        std::cout << "L'opération " << operateur << " n'existe pas" << std::endl;
        return Variable();
    }

    if(operations[operateur].find(std::make_pair(first.get_nature(), second.get_nature())) == operations[operateur].end()) {
        std::cout << "L'opération " << operateur << " n'est pas valide pour ces types" << std::endl;
        return Variable();
    }

    return operations[operateur][std::make_pair(first.get_nature(), second.get_nature())](first, second);
}

} // namespace stretch::arithmetique