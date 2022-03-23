#include <tao/pegtl/demangle.hpp>

#include <functional>
#include <map>
#include <regex>
#include <cstring>
#include <cmath>

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
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return f;
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
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<BigDecimal>(f.get_valeur()) - std::get<BigDecimal>(s.get_valeur()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Reel),
                [](const Variable f, const Variable s) {
                    std::string str = std::get<std::string>(f.get_valeur());
                    BigDecimal n = std::get<BigDecimal>(s.get_valeur());
                    str.erase(str.begin() + str.size() - n.toInt(), str.end());

                    return Variable(str);
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    Variable t_f = Variable::parse(std::get<std::string>(f.get_valeur()));
                    Variable t_s = Variable::parse(std::get<std::string>(s.get_valeur()));

                    if(t_s.est(Nature::Reel)) {
                        int n = std::get<BigDecimal>(t_s.get_valeur()).toInt();

                        std::string str = std::get<std::string>(f.get_valeur());
                        str.erase(str.begin() + str.size() - n, str.end());
                        
                        return Variable(str);
                    } 

                    std::regex r = std::regex(std::get<std::string>(f.get_valeur()));
                    return Variable(std::regex_replace(std::get<std::string>(s.get_valeur()), r, ""));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<BigDecimal>(f.get_valeur()) * -1);
                }
            }
        }
    },

    /////////////////////////////////////////////////
    // Multiplication
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::facteur>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<BigDecimal>(f.get_valeur()) * std::get<BigDecimal>(s.get_valeur()));
                }
            },
            /////////////////////////////////////////////////
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
            /////////////////////////////////////////////////
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
    },

    /////////////////////////////////////////////////
    // Division
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::fraction>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<BigDecimal>(f.get_valeur()) / std::get<BigDecimal>(s.get_valeur()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    Variable t = Variable::parse(std::get<std::string>(s.get_valeur()));

                    if(t.est(Nature::Reel))
                        return Variable(std::get<BigDecimal>(f.get_valeur()) / std::get<BigDecimal>(t.get_valeur()));
                    
                    throw std::runtime_error("Vous ne pouvez pas diviser un réel par une chaîne qui ne contient pas de réel");
                }
            }
        }
    },

    /////////////////////////////////////////////////
    // Reste
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::modulo>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<BigDecimal>(f.get_valeur()) % std::get<BigDecimal>(s.get_valeur()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    Variable t = Variable::parse(std::get<std::string>(s.get_valeur()));

                    if(t.est(Nature::Reel))
                        return Variable(std::get<BigDecimal>(f.get_valeur()) % std::get<BigDecimal>(t.get_valeur()));
                    
                    throw std::runtime_error("Vous ne pouvez pas diviser un réel par une chaîne qui ne contient pas de réel");
                }
            }
        }
    },

    /////////////////////////////////////////////////
    // Egalité
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::egal>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<BigDecimal>(f.get_valeur()) == std::get<BigDecimal>(s.get_valeur()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<std::string>(f.get_valeur()) == std::get<std::string>(s.get_valeur()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<std::string>(f.get_valeur()) == std::get<BigDecimal>(s.get_valeur()).toString());
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<std::string>(f.get_valeur()) == std::get<BigDecimal>(s.get_valeur()).toString());
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Booleen),
                [](const Variable f, const Variable s) {
                    Variable t = Variable::parse(std::get<std::string>(f.get_valeur()));

                    if(t.est(Nature::Booleen))
                        return Variable(std::get<bool>(t.get_valeur()) == std::get<bool>(s.get_valeur()));
                    
                    return Variable(false);
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Booleen, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    Variable t = Variable::parse(std::get<std::string>(s.get_valeur()));
                    
                    if(t.est(Nature::Booleen))
                        return Variable(std::get<bool>(f.get_valeur()) == std::get<bool>(t.get_valeur()));
                    
                    return Variable(false);
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Booleen, Nature::Booleen),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<bool>(f.get_valeur()) == std::get<bool>(s.get_valeur()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Nul),
                [](const Variable f, const Variable s) {                
                    return Variable(false);
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Nul, Nature::Chaine),
                [](const Variable f, const Variable s) {                
                    return Variable(false);
                } 
            }
        } 
    },

    /////////////////////////////////////////////////
    // Différence
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::different>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(!(std::get<BigDecimal>(f.get_valeur()) == std::get<BigDecimal>(s.get_valeur())));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<std::string>(f.get_valeur()) != std::get<std::string>(s.get_valeur()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<std::string>(f.get_valeur()) != std::get<BigDecimal>(s.get_valeur()).toString());
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<std::string>(f.get_valeur()) != std::get<BigDecimal>(s.get_valeur()).toString());
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Booleen),
                [](const Variable f, const Variable s) {
                    Variable t = Variable::parse(std::get<std::string>(f.get_valeur()));

                    if(t.est(Nature::Booleen))
                        return Variable(std::get<bool>(t.get_valeur()) != std::get<bool>(s.get_valeur()));
                    
                    return Variable(true);
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Booleen, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    Variable t = Variable::parse(std::get<std::string>(s.get_valeur()));

                    if(t.est(Nature::Booleen))
                        return Variable(std::get<bool>(f.get_valeur()) != std::get<bool>(t.get_valeur()));
                    
                    return Variable(true);
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Nul),
                [](const Variable f, const Variable s) {                
                    return Variable(true);
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Nul, Nature::Chaine),
                [](const Variable f, const Variable s) {                
                    return Variable(true);
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Booleen, Nature::Booleen),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<bool>(f.get_valeur()) != std::get<bool>(s.get_valeur()));
                }
            }
        }
    }, 

    /////////////////////////////////////////////////
    // Ou
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::ou>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Booleen, Nature::Booleen),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<bool>(f.get_valeur()) || std::get<bool>(s.get_valeur()));
                }
            }
        }
    },   

    /////////////////////////////////////////////////
    // Et
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::et>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Booleen, Nature::Booleen),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<bool>(f.get_valeur()) && std::get<bool>(s.get_valeur()));
                }
            }
        }
    }, 

    /////////////////////////////////////////////////
    // Plus petit que
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::plus_petit_que>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<BigDecimal>(f.get_valeur()) < std::get<BigDecimal>(s.get_valeur()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    return Variable(std::strcmp(std::get<std::string>(f.get_valeur()).c_str(), std::get<std::string>(s.get_valeur()).c_str()) == -1);
                }
            }
        }
    }, 

    /////////////////////////////////////////////////
    // Plus grand que
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::plus_grand_que>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Reel),
                [](const Variable f, const Variable s) {
                    return Variable(std::get<BigDecimal>(f.get_valeur()) > std::get<BigDecimal>(s.get_valeur()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    return Variable(std::strcmp(std::get<std::string>(f.get_valeur()).c_str(), std::get<std::string>(s.get_valeur()).c_str()) == 1);
                }
            }
        }
    },

    /////////////////////////////////////////////////
    // Non
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::non>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Booleen, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(!(std::get<bool>(f.get_valeur())));
                }
            }
        }
    },

    /////////////////////////////////////////////////
    // Indice
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::indice>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Reel),
                [](const Variable f, const Variable s) {
                    auto indice = std::get<BigDecimal>(s.get_valeur());
                    auto chaine = std::get<std::string>(f.get_valeur());

                    if(indice > 0 && indice.toInt() <= chaine.size()) 
                        return Variable(std::string{chaine[indice.toInt() - 1]});

                    throw std::runtime_error("L'indice donné dépasse les bornes de la chaîne");
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Reel),
                [](const Variable f, const Variable s) {
                    auto indice = std::get<BigDecimal>(s.get_valeur());
                    auto reel = std::get<BigDecimal>(f.get_valeur());

                    if(indice < 0) {
                        int entier = std::abs((indice + 1).toInt());

                        if(entier >= reel.getDecPart().size())
                            throw std::runtime_error("L'indice " + std::to_string(entier) + " donné dépasse les bornes de la partie décimale de taille " + std::to_string(reel.getDecPart().size()));

                        return Variable(BigDecimal(reel.getDecPart()[entier] - '0')); 
                    }
                        
                    if(indice > 0 && indice.toInt() >= reel.getIntPart().size())
                        throw std::runtime_error("L'indice " + indice.toString() + " donné dépasse les bornes de la partie réelle de taille " + std::to_string(reel.getIntPart().size()));

                    return Variable(BigDecimal(reel.getIntPart()[reel.getIntPart().size() - indice.toInt()] - '0')); 
                }
            }
        }
    }
};

const Variable operation(const std::string_view& operateur, const Variable first, const Variable second) {
    if(operations.find(operateur) == operations.end()) {
        throw std::runtime_error("L'opération " + static_cast<std::string>(operateur) + " n'existe pas");
        return Variable();
    } 

    if(operations[operateur].find(std::make_pair(first.get_nature(), second.get_nature())) == operations[operateur].end()) {
        throw std::runtime_error("L'opération " + static_cast<std::string>(operateur) + " n'est pas supporté pour les types " + Variable::type_tos(first.get_nature()) + " et " + Variable::type_tos(second.get_nature()));
        return Variable();
    }

    return operations[operateur][std::make_pair(first.get_nature(), second.get_nature())](first, second);
}

} // namespace stretch::arithmetique