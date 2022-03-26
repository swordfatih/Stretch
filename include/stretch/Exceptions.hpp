/////////////////////////////////////////////////
// Headers
/////////////////////////////////////////////////
#pragma once
#include <exception>

namespace stretch::exception {

/////////////////////////////////////////////////
class StretchException : public std::exception
{
public:
    explicit StretchException(std::string message) : m_message(message) {}

    char const* what() const noexcept override 
    { 
        return m_message.c_str(); 
    }

protected:
    std::string m_message;
};

/////////////////////////////////////////////////
class QuitterException : public StretchException {

}

/////////////////////////////////////////////////
class BoucleException : public StretchException {
    
}

} // namespace stretch