#include "KruskalMST.hpp"
#include <iostream>

void KruskalMST::sortEdges(DynamicArray& edges) {
    int n = edges.getSize();
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }
}

MSTResult KruskalMST::findMST(Graph* graph) {
    MSTResult result;
    
    if (graph->getIsDirected()) {
        std::cerr << "Blad: Algorytm Kruskala dziala tylko na grafach nieskierowanych" << std::endl;
        return result;
    }
    
    int vertices = graph->getVerticesCount();
    DynamicArray allEdges = graph->getEdges();
    
    sortEdges(allEdges);
    
    UnionFind uf(vertices);
    result.totalCost = 0;
    int edgesAdded = 0;
    
    for (int i = 0; i < allEdges.getSize() && edgesAdded < vertices - 1; i++) {
        const Edge& edge = allEdges[i];
        
        if (uf.unionSets(edge.start, edge.end)) {
            result.edges.push_back(edge);
            result.totalCost += edge.weight;
            edgesAdded++;
        }
    }
    
    result.success = (edgesAdded == vertices - 1);
    
    return result;
}

void KruskalMST::printMST(const MSTResult& result) {
    if (!result.success) {
        std::cout << "MST nie znalezione lub graf nie jest spojny" << std::endl;
        return;
    }
    
    std::cout << "Minimalne Drzewo Rozpinajace (Algorytm Kruskala):" << std::endl;
    std::cout << "Calkowity koszt: " << result.totalCost << std::endl;
    std::cout << "Krawedzie w MST:" << std::endl;
    
    for (int i = 0; i < result.edges.getSize(); i++) {
        const Edge& edge = result.edges[i];
        std::cout << "  " << edge.start << " -- " << edge.end 
                  << " [waga: " << edge.weight << "]" << std::endl;
    }
}
