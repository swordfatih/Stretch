#include "stretch/Memoire.hpp"

namespace stretch {
            
void Memoire::vider() {
    if(variables.empty())
        variables.clear();
}

// void Memoire::supprimer_variable(const std::string& nom) {
//     variables.erase(nom);
// }

// void Memoire::modifier_memoire(const std::string& cle, std::unique_ptr<Variable> var) {
//     variables[cle] = std::move(var);
// }

// Variable* Memoire::recuperer_variable(const std::string& nom) {
//     return variables[nom].get();
// }

};