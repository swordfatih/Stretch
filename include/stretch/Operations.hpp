/////////////////////////////////////////////////
// Headers
/////////////////////////////////////////////////
#include <functional>
#include <map>
#include <regex>
#include <cstring>
#include <cmath>

/////////////////////////////////////////////////
namespace stretch
{

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
        pe::demangle<stretch::addition>(), 
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
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Tableau, Nature::Tableau),
                [](const Variable f, const Variable s) {
                    auto t = std::get<Tableau>(f.get_valeur());
                    auto q = std::get<Tableau>(s.get_valeur());
                    t.insert(t.end(), q.begin(), q.end());
                    
                    return Variable(t);
                }
            }
        }
    },

    /////////////////////////////////////////////////
    // Soustraction
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::soustraction>(), 
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
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Tableau, Nature::Reel),
                [](const Variable f, const Variable s) {
                    auto t_f = std::get<Tableau>(f.get_valeur());
                    auto t_s = std::get<BigDecimal>(s.get_valeur());
                    
                    if(t_s.toInt() > t_f.size()) {
                        throw std::runtime_error("Ce que vous souhaitez retirer du tableau dépasse sa taille");
                    }

                    return Variable(std::vector(t_f.begin(), t_f.end() - t_s.toInt()));
                }
            }

        }
    }, 

    /////////////////////////////////////////////////
    // Multiplication
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::multiplication>(), 
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
        pe::demangle<stretch::division>(), 
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
        pe::demangle<stretch::reste>(), 
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
        pe::demangle<stretch::egalite>(), 
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
                    
                    return Variable(VariantValeur(false));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Booleen, Nature::Chaine),
                [](const Variable f, const Variable s) {
                    Variable t = Variable::parse(std::get<std::string>(s.get_valeur()));
                    
                    if(t.est(Nature::Booleen))
                        return Variable(std::get<bool>(f.get_valeur()) == std::get<bool>(t.get_valeur()));
                    
                    return Variable(VariantValeur(false));
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
                    return Variable(VariantValeur(false));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Nul, Nature::Chaine),
                [](const Variable f, const Variable s) {                
                    return Variable(VariantValeur(false));
                } 
            }
        } 
    },

    /////////////////////////////////////////////////
    // Différence
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::inegalite>(), 
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
    // Inferieur
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::inferieur>(), 
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
    // Supérieur
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::superieur>(), 
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
        pe::demangle<stretch::negation>(), 
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
        pe::demangle<stretch::indexation>(), 
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
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Tableau, Nature::Reel),
                [](const Variable f, const Variable s) {
                    auto indice = std::get<BigDecimal>(s.get_valeur());
                    auto tableau = std::get<Tableau>(f.get_valeur());

                    if(indice > 0 && indice.toInt() <= tableau.size()) 
                        return Variable(tableau[indice.toInt() - 1]);

                    throw std::runtime_error("L'indice donné dépasse les bornes du tableau");
                }
            }
        }
    },

    /////////////////////////////////////////////////
    // Taille
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::taille>(), 
        {
            {
                std::make_pair(Nature::Objet, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(BigDecimal(std::to_string(std::get<Objet>(f.get_valeur()).size())));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Tableau, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(BigDecimal(std::to_string(std::get<Tableau>(f.get_valeur()).size())));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(BigDecimal(std::to_string(f.to_string().size())));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(BigDecimal(std::to_string(f.to_string().size())));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Booleen, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(BigDecimal(std::to_string(f.to_string().size())));
                }
            }
        }
    },
    /////////////////////////////////////////////////
    // Nature
    /////////////////////////////////////////////////
    {
        pe::demangle<stretch::nature>(), 
        {
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Tableau, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(Variable::type_tos(f.get_nature()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Chaine, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(Variable::type_tos(f.get_nature()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Reel, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(Variable::type_tos(f.get_nature()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Booleen, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(Variable::type_tos(f.get_nature()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Objet, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(Variable::type_tos(f.get_nature()));
                }
            },
            /////////////////////////////////////////////////
            {
                std::make_pair(Nature::Nul, Nature::Nul),
                [](const Variable f, const Variable s) {
                    return Variable(Variable::type_tos(f.get_nature()));
                }
            }
        }
    }
};

/////////////////////////////////////////////////
const Variable operer(const std::string_view& operateur, const Variable first, const Variable second) 
{
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

} // namespace stretch