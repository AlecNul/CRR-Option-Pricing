#include <iostream>
#include "Options.h"
#include "BlackScholesPricer.h"

using namespace std;

int main() {

    // On a besoin d'un 'try' car la création PEUT échouer
    try {

        // 1. On crée un objet CONCRET
        EuropeanCallOption myCall(1.0, 100.0); // T=1 an, K=100

        // 2. On l'utilise
        std::cout << "Payoff du Call (S=110): " << myCall.payoff(110.0) << std::endl; // Doit être 10
        std::cout << "Payoff du Call (S=90) : " << myCall.payoff(90.0) << std::endl;  // Doit être 0

        // 3. On teste l'échec (strike négatif)
        std::cout << "On teste une erreur..." << std::endl;
        EuropeanCallOption badCall(1.0, -50.0); // Cette ligne va 'throw'

    }
    // On attrape l'erreur si elle se produit
    catch (const std::exception& e) {
        std::cout << "OK, erreur attrapee comme prevu !" << std::endl;
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}