#include "Variable.hpp"

#include <memory>
#include <string>
#include <map>
#include <stdexcept>

namespace stretch {


    class Memoire {
        public:
            void vider();
            void supprimer_variable(const std::string& nom);
            void ajouter_variable(const std::string& cle, std::unique_ptr<Variable> var); //modife la valeur associée à la clé ou ajoute un nouvel élement
            Variable* recuperer_variable(const std::string& nom);

        private:
            int ajouter_memoire(std::unique_ptr<Variable> var);

            std::map<std::string, int> variables;
            std::map<int, std::unique_ptr<Variable>> memoire;

    };
}