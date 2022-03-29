#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

/////////////////////////////////////////////////
// Headers
/////////////////////////////////////////////////
#include <tao/pegtl.hpp>
#include <exception>

/////////////////////////////////////////////////
namespace stretch::exception {

/////////////////////////////////////////////////
class StretchException : public std::exception
{
public:
    /////////////////////////////////////////////////
    explicit StretchException(const pe::position& position, const std::string& message = {}, const std::string& sujet = {}) : m_position(position), m_message(message), m_sujet(sujet)
    {
        m_output = format(position, message, sujet);
    }

    /////////////////////////////////////////////////
    static std::string format(const pe::position& position, const std::string& message, const std::string& sujet)
    {
        std::stringstream ss;

        ss << "\033[31;1m" << "[erreur d'execution]" << "\033[0m" << "\n";
        ss << "fichier: " << "\033[35m" << position.source << "\033[0m" << "\n";
        ss << tab << "ligne: " << "\033[35m" << std::to_string(position.line) << "\033[0m" << ", colonne: " << "\033[35m" << std::to_string(position.column) << "\033[0m" << "\n";
        if(!message.empty()) ss << "message: " << "\033[35;1m" << message << "\033[0m" << "\n";
        if(!sujet.empty()) ss << "sujet: " << "\033[35;1m" << sujet << "\033[0m" << "\n";

        return ss.str();
    }

    /////////////////////////////////////////////////
    static std::string format(const pe::parse_error& e, const std::string_view& ligne)
    {
        std::stringstream ss;
        auto position = e.positions().front();

        ss << "\033[31;1m" << "[erreur de syntaxe]" << "\033[0m" << "\n";
        ss << "fichier: " << "\033[35m" << position.source << "\033[0m" << "\n";
        ss << tab << "ligne: " << "\033[35m" << std::to_string(position.line) << "\033[0m" << ", colonne: " << "\033[35m" << std::to_string(position.column) << "\033[0m" << "\n";
        ss << "message: " << "\033[35;1m" << e.message() << "\033[0m" << "\n";
        ss << ligne << "\n";
        ss << "\033[31;1m" << std::string(position.column, '-') << std::string(e.positions().size(), '^') << "\033[0m" << "\n";

        return ss.str();
    }

    /////////////////////////////////////////////////
    char const* what() const noexcept override 
    { 
        return m_output.data();
    }

protected:
    /////////////////////////////////////////////////
    pe::position m_position;
    std::string m_message;
    std::string m_sujet;
    std::string m_output;

private:
    /////////////////////////////////////////////////
    inline static const std::string tab = std::string(4, ' ');
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