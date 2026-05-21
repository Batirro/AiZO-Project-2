#include <iostream>
#include "Parameters.h"

int main(int argc, char* argv[]) {
    std::cout << "Aizo Project 2 - Graphs" << std::endl;
    
    // Inicjalizacja parsera parametrów
    Parameters::readParameters(argc, argv);
    
    // Przykładowe wypisanie
    // Parameters::printParameters();
    
    return 0;
}
