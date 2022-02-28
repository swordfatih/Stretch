#include <vector>
#include <variant>
#include <iostream>
#include "StretchFonction.hpp"
#include "Variable.hpp"

struct BlocInstuction {
    public:
        std::vector<Variable>& getParametres(){ 
            return parametres; 
        }
        
        void setParametres(std::vector<Variable> parametres){ 
            this->parametres = parametres; 
        }

        const std::ostream &operator<<(std::ostream &os) {
            return os << "Information sur la fonction : " << fonction_pointeur.string() << std::endl
                    << "Liste des paramètres " << parametres_string() << std::endl
                    <

        }
    private:
        StretchFonction fonction_pointeur;
        std::vector<Variable> parametres; //paramètres de la fonction
        std::vector<Variable> variables; //variables crées dans la fonction

        std::string parametres_string(std::ostream &os) {
            for(const Variable& param : parametres) {
                os << param;
            }
        } 
    
};