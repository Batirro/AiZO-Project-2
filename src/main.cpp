#include <iostream>
#include "Parameters.h"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Aizo Project 2 - Graphs" << std::endl;

    // Inicjalizacja parsera parametrów
    Parameters::readParameters(argc, argv);

    std::cout << "\n--- Testowanie Struktur Grafowych ---\n" << std::endl;

    int vertices = 4;
    bool isDirected = true; // Zgodnie z założeniami SP i MF, testujemy graf skierowany

    AdjacencyList adjList(vertices, isDirected);
    IncidenceMatrix incMatrix(vertices, isDirected);

    // Przykładowe dane
    // 0->1 (10), 0->2 (10), 1->3 (5), 2->3 (12), 3->0 (1), 3->2 (7)

    int edges[][3] = {
        {0, 1, 10},
        {0, 2, 10},
        {1, 3, 5},
        {2, 3, 12},
        {3, 0, 1},
        {3, 2, 7}
    };

    for (int i = 0; i < 6; ++i) {
        adjList.addEdge(edges[i][0], edges[i][1], edges[i][2]);
        incMatrix.addEdge(edges[i][0], edges[i][1], edges[i][2]);
    }

    adjList.print();
    std::cout << std::endl;
    incMatrix.print();

    return 0;
}
