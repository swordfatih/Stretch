#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

/////////////////////////////////////////////////
// Headers
/////////////////////////////////////////////////
#include <exception>
#include <tao/pegtl.hpp>

#include <fmt/color.h>

/////////////////////////////////////////////////
namespace stretch::exception {

/////////////////////////////////////////////////
class Runtime
{
public:
    /////////////////////////////////////////////////
    Runtime(const pe::position& position, const std::string& message = {}, const std::string& sujet = {}) : m_position(position), m_message(message), m_sujet(sujet)
    {
        
    }

    /////////////////////////////////////////////////
    void print() const
    {
        std::string output = fmt::format(fg(fmt::color::crimson) | fmt::emphasis::bold, "[erreur d'execution]\n");
        output += fmt::format("fichier: ");
        output += fmt::format(fg(fmt::color::pink), "{}\n", m_position.source);
        output += fmt::format("    ligne: ");
        output += fmt::format(fg(fmt::color::pink), "{}", m_position.line);
        output += fmt::format(", colonne: ");
        output += fmt::format(fg(fmt::color::pink), "{}\n", m_position.column);
        
        if(!m_message.empty()) 
        {
            output += fmt::format("message: ");
            output += fmt::format(fg(fmt::color::pink) | fmt::emphasis::bold, "{}\n", m_message);
        } 

        if(!m_sujet.empty()) 
        {
            output += fmt::format("sujet: ");
            output += fmt::format(fg(fmt::color::pink) | fmt::emphasis::bold, "{}\n", m_sujet);
        }

        fmt::print(stderr, "{}", output);
    }

    /////////////////////////////////////////////////
    static void print(const pe::parse_error& e, const std::string_view& ligne)
    {
        auto position = e.positions().front();

        std::string output = fmt::format(fg(fmt::color::crimson) | fmt::emphasis::bold, "[erreur de syntaxe]\n");
        output += fmt::format("fichier: ");
        output += fmt::format(fg(fmt::color::pink), "{}\n", position.source);
        output += fmt::format("    ligne: ");
        output += fmt::format(fg(fmt::color::pink), "{}", position.line);
        output += fmt::format(", colonne: ");
        output += fmt::format(fg(fmt::color::pink), "{}\n", position.column);  
        output += fmt::format("message: ");
        output += fmt::format(fg(fmt::color::pink) | fmt::emphasis::bold, "{}\n", e.message());
        output += fmt::format("{}\n", ligne);
        output += fmt::format(fg(fmt::color::crimson) | fmt::emphasis::bold, "{}{}\n", std::string(position.column, '-'), "^");

        fmt::print(stderr, "{}", output);
    }

private:
    /////////////////////////////////////////////////
    pe::position m_position;
    std::string m_message;
    std::string m_sujet;
    std::string m_output;
};

/////////////////////////////////////////////////
class Quitter 
{
    /////////////////////////////////////////////////
    Boucle()
    {

    }
};

/////////////////////////////////////////////////
class Boucle
{
public:
    /////////////////////////////////////////////////
    enum class Type {
        Arreter,
        Continuer
    };

    /////////////////////////////////////////////////
    Boucle(Type type) : m_type(type)
    {

    }

    /////////////////////////////////////////////////
    Type get_type() const 
    {
        return m_type;
    }

private:
    /////////////////////////////////////////////////
    Type m_type;
};

/////////////////////////////////////////////////
class Retour
{
public:
    /////////////////////////////////////////////////
    Retour(const Tableau& retour) : m_retour(retour)
    {

    }

    /////////////////////////////////////////////////
    Tableau& get_retour() 
    {
        return m_retour;
    }

private:
    /////////////////////////////////////////////////
    Tableau m_retour;
};

} // namespace stretch

#endif // EXCEPTIONS_HPP