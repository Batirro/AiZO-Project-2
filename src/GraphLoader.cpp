#include "GraphLoader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Graph* GraphLoader::loadFromFile(const char* filename, bool useAdjacencyList, bool isDirected) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Blad: Nie mozna otworzyc pliku " << filename << std::endl;
        return nullptr;
    }
    
    int vertices = 0;
    int expectedEdges = 0;
    
    std::string line;
    if (!std::getline(file, line)) {
        std::cerr << "Blad: Pusty plik lub nie mozna odczytac pierwszej linii" << std::endl;
        file.close();
        return nullptr;
    }
    
    std::istringstream iss(line);
    if (!(iss >> vertices >> expectedEdges)) {
        std::cerr << "Blad: Nieprawidlowy format pierwszej linii. Oczekiwano: wierzcholki<TAB>krawedzie" << std::endl;
        file.close();
        return nullptr;
    }
    
    if (vertices <= 0 || expectedEdges < 0) {
        std::cerr << "Blad: Nieprawidlowa liczba wierzcholkow lub krawedzi" << std::endl;
        file.close();
        return nullptr;
    }
    
    Graph* graph = nullptr;
    if (useAdjacencyList) {
        graph = new AdjacencyList(vertices, isDirected);
    } else {
        graph = new IncidenceMatrix(vertices, isDirected);
    }
    
    int edgesRead = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        int start, end, weight;
        std::istringstream edgeStream(line);
        
        if (!(edgeStream >> start >> end >> weight)) {
            std::cerr << "Uwaga: Pomijanie nieprawidlowej linii: " << line << std::endl;
            continue;
        }
        
        if (start < 0 || start >= vertices || end < 0 || end >= vertices) {
            std::cerr << "Uwaga: Nieprawidlowe indeksy wierzcholkow (" << start << ", " << end 
                      << "). Pomijanie krawedzi." << std::endl;
            continue;
        }
        
        graph->addEdge(start, end, weight);
        edgesRead++;
    }
    
    file.close();
    
    if (!validateGraph(graph, expectedEdges)) {
        std::cerr << "Uwaga: Niezgodnosc liczby krawedzi. Oczekiwano: " << expectedEdges 
                  << ", Wczytano: " << edgesRead << ", W grafie: " << graph->getEdgesCount() << std::endl;
    }
    
    std::cout << "Pomyslnie wczytano graf: " << vertices << " wierzcholkow, " 
              << graph->getEdgesCount() << " krawedzi" << std::endl;
    
    return graph;
}

bool GraphLoader::validateGraph(Graph* graph, int expectedEdges) {
    return graph->getEdgesCount() == expectedEdges;
}
