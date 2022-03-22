
class StretchException : public std::exception
{
public:
    explicit StretchException(std::string message) : m_message(message) {

    }

    char const* what() const noexcept override 
    { 
        return m_message.c_str(); 
    }

private:
    std::string m_message;
};