#include "PrimMST.hpp"
#include <iostream>
#include <limits>

MSTResult PrimMST::findMST(Graph* graph, int startVertex) {
    MSTResult result;
    
    if (graph->getIsDirected()) {
        std::cerr << "Blad: Algorytm Prima dziala tylko na grafach nieskierowanych" << std::endl;
        return result;
    }
    
    int vertices = graph->getVerticesCount();
    if (startVertex < 0 || startVertex >= vertices) {
        std::cerr << "Blad: Nieprawidlowy wierzcholek startowy" << std::endl;
        return result;
    }
    
    AdjacencyList* adjList = dynamic_cast<AdjacencyList*>(graph);
    IncidenceMatrix* incMatrix = dynamic_cast<IncidenceMatrix*>(graph);
    
    if (!adjList && !incMatrix) {
        std::cerr << "Blad: Nieznana reprezentacja grafu" << std::endl;
        return result;
    }
    
    bool* inMST = new bool[vertices];
    int* parent = new int[vertices];
    int* key = new int[vertices];
    
    for (int i = 0; i < vertices; i++) {
        inMST[i] = false;
        parent[i] = -1;
        key[i] = std::numeric_limits<int>::max();
    }
    
    MinHeap minHeap;
    key[startVertex] = 0;
    minHeap.push(startVertex, 0);
    
    int edgesAdded = 0;
    result.totalCost = 0;
    
    while (!minHeap.isEmpty() && edgesAdded < vertices - 1) {
        HeapNode current = minHeap.pop();
        int u = current.vertex;
        
        if (inMST[u]) continue;
        
        inMST[u] = true;
        
        if (parent[u] != -1) {
            result.edges.push_back(Edge(parent[u], u, key[u]));
            result.totalCost += key[u];
            edgesAdded++;
        }
        
        if (adjList) {
            const LinkedList& neighbors = adjList->getNeighbors(u);
            for (const auto& edge : neighbors) {
                int v = edge.end;
                int weight = edge.weight;
                
                if (!inMST[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                    minHeap.push(v, weight);
                }
            }
        } else if (incMatrix) {
            DynamicArray edges = incMatrix->getEdges();
            for (int i = 0; i < edges.getSize(); i++) {
                const Edge& edge = edges[i];
                
                if (edge.start == u || edge.end == u) {
                    int v = (edge.start == u) ? edge.end : edge.start;
                    int weight = edge.weight;
                    
                    if (!inMST[v] && weight < key[v]) {
                        key[v] = weight;
                        parent[v] = u;
                        minHeap.push(v, weight);
                    }
                }
            }
        }
    }
    
    result.success = (edgesAdded == vertices - 1);
    
    delete[] inMST;
    delete[] parent;
    delete[] key;
    
    return result;
}

void PrimMST::printMST(const MSTResult& result) {
    if (!result.success) {
        std::cout << "MST nie znalezione lub graf nie jest spojny" << std::endl;
        return;
    }
    
    std::cout << "Minimalne Drzewo Rozpinajace (Algorytm Prima):" << std::endl;
    std::cout << "Calkowity koszt: " << result.totalCost << std::endl;
    std::cout << "Krawedzie w MST:" << std::endl;
    
    for (int i = 0; i < result.edges.getSize(); i++) {
        const Edge& edge = result.edges[i];
        std::cout << "  " << edge.start << " -- " << edge.end 
                  << " [waga: " << edge.weight << "]" << std::endl;
    }
}
