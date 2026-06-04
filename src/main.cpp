#include <iostream>
#include "Parameters.h"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"
#include "GraphGenerator.hpp"
#include "GraphLoader.hpp"
#include "PrimMST.hpp"
#include "KruskalMST.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Aizo Project 2 - Graphs" << std::endl;

    Parameters::readParameters(argc, argv);

    std::cout << "\n=== Test algorytmow MST ===\n" << std::endl;
    
    std::cout << "--- Generowanie grafu testowego ---" << std::endl;
    GraphGenerator gen(8, 0.4f, false);
    Graph* graph = gen.generate(true);
    std::cout << "Graf: " << graph->getVerticesCount() 
              << " wierzcholkow, " << graph->getEdgesCount() << " krawedzi" << std::endl;
    graph->print();
    
    std::cout << "\n--- Algorytm Prima ---" << std::endl;
    MSTResult primResult = PrimMST::findMST(graph);
    PrimMST::printMST(primResult);
    
    std::cout << "\n--- Algorytm Kruskala ---" << std::endl;
    MSTResult kruskalResult = KruskalMST::findMST(graph);
    KruskalMST::printMST(kruskalResult);
    
    std::cout << "\n--- Porownanie wynikow ---" << std::endl;
    std::cout << "Koszt MST (Prim):    " << primResult.totalCost << std::endl;
    std::cout << "Koszt MST (Kruskal): " << kruskalResult.totalCost << std::endl;
    
    if (primResult.totalCost == kruskalResult.totalCost) {
        std::cout << "SUKCES: Oba algorytmy daly ten sam koszt MST!" << std::endl;
    } else {
        std::cout << "UWAGA: Rozne koszty - sprawdz implementacje" << std::endl;
    }
    
    std::cout << "\n--- Test na macierzy incydencji ---" << std::endl;
    Graph* graphMatrix = gen.generate(false);
    std::cout << "Graf (macierz): " << graphMatrix->getVerticesCount() 
              << " wierzcholkow, " << graphMatrix->getEdgesCount() << " krawedzi" << std::endl;
    
    MSTResult primMatrix = PrimMST::findMST(graphMatrix);
    std::cout << "Prim (macierz) - koszt: " << primMatrix.totalCost << std::endl;
    
    MSTResult kruskalMatrix = KruskalMST::findMST(graphMatrix);
    std::cout << "Kruskal (macierz) - koszt: " << kruskalMatrix.totalCost << std::endl;
    
    delete graph;
    delete graphMatrix;

    return 0;
}
