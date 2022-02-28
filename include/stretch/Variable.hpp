#include <cstdint>
#include <string>

class Variable {
public:
    ////////////////////////////////////////////////////////////
    /// \brief Constructeur qui attribue une valeur par défaut
    ///
    ////////////////////////////////////////////////////////////
    Variable() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Constructeur qui créer la variable depuis une
    /// chaîne de caractère
    ///
    ////////////////////////////////////////////////////////////
    Variable(const std::string& chaine);

private:
    virtual void parse(const std::string& chaine) = 0;
};

////////////////////////////////////////////////////////////
/// \brief Entier tel que representé dans la mémoire
///
////////////////////////////////////////////////////////////
class Entier : public Variable {
public:
    Entier();
    Entier(const std::string& chaine);

private:
    void parse(const std::string& chaine);

    // std::variant<int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t> valeur;
};