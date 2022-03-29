#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

/////////////////////////////////////////////////
// Headers
/////////////////////////////////////////////////
#include <tao/pegtl/position.hpp>
#include <exception>

/////////////////////////////////////////////////
namespace stretch::exception {

/////////////////////////////////////////////////
class StretchException : public std::exception
{
public:
    /////////////////////////////////////////////////
    explicit StretchException(const pe::position& position, std::string message = {}, std::string sujet = {}) : m_position(position), m_message(message), m_sujet(sujet) 
    {
        m_output = "[exception]\n";
        m_output += "   - fichier: " + m_position.source + "\n";
        m_output += "   - ligne: " + std::to_string(m_position.line) + "\n";
        m_output += "   - colonne: " + std::to_string(m_position.column) + "\n";
        
        if(!m_sujet.empty())
            m_output += "   - sujet: " + m_sujet + "\n";

        if(!m_message.empty())
            m_output += "   - message: " + m_message + "\n";
    }

    /////////////////////////////////////////////////
    char const* what() const noexcept override 
    { 
        return m_output.data();
    }

protected:
    pe::position m_position;
    std::string m_message;
    std::string m_sujet;
    std::string m_output;
};

/////////////////////////////////////////////////
class Quitter : public StretchException 
{
    using StretchException::StretchException;
};

/////////////////////////////////////////////////
class Boucle : public StretchException 
{
public:
    /////////////////////////////////////////////////
    enum class Type {
        Arreter,
        Continuer
    };

    /////////////////////////////////////////////////
    explicit Boucle(Type type, pe::position position, std::string message = {}) : m_type(type), StretchException(position, message) {}

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
class Runtime : public StretchException
{
    using StretchException::StretchException;
};

} // namespace stretch

#endif // EXCEPTIONS_HPP