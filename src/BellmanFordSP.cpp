#include "BellmanFordSP.hpp"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"
#include <iostream>
#include <limits>

SPResult BellmanFordSP::findSP(Graph* graph, int startVertex) {
    SPResult result;

    if (!graph->getIsDirected()) {
        std::cerr << "Uwaga: Bellman-Ford dziala rowniez na grafach nieskierowanych" << std::endl;
    }

    int V = graph->getVerticesCount();
    if (startVertex < 0 || startVertex >= V) {
        std::cerr << "Blad: Nieprawidlowy wierzcholek startowy" << std::endl;
        return result;
    }

    AdjacencyList*   adjList   = dynamic_cast<AdjacencyList*>(graph);
    IncidenceMatrix* incMatrix = dynamic_cast<IncidenceMatrix*>(graph);

    if (!adjList && !incMatrix) {
        std::cerr << "Blad: Nieznana reprezentacja grafu" << std::endl;
        return result;
    }

    int* dist   = new int[V];
    int* parent = new int[V];

    for (int i = 0; i < V; i++) {
        dist[i]   = std::numeric_limits<int>::max();
        parent[i] = -1;
    }
    dist[startVertex] = 0;

    for (int iter = 0; iter < V - 1; iter++) {

        if (adjList) {
            for (int u = 0; u < V; u++) {
                if (dist[u] == std::numeric_limits<int>::max()) continue;

                const LinkedList& neighbors = adjList->getNeighbors(u);
                for (const auto& edge : neighbors) {
                    int v      = edge.end;
                    int weight = edge.weight;

                    if (dist[u] + weight < dist[v]) {
                        dist[v]   = dist[u] + weight;
                        parent[v] = u;
                    }
                }
            }
        } else if (incMatrix) {
            DynamicArray edges = incMatrix->getEdges();
            for (int i = 0; i < edges.getSize(); i++) {
                const Edge& edge = edges[i];
                int u      = edge.start;
                int v      = edge.end;
                int weight = edge.weight;

                if (dist[u] != std::numeric_limits<int>::max()
                    && dist[u] + weight < dist[v]) {
                    dist[v]   = dist[u] + weight;
                    parent[v] = u;
                }
            }
        }
    }

    // Wykrywanie ujemnych cykli (dodatkowa iteracja)
    bool negativeCycle = false;
    if (adjList) {
        for (int u = 0; u < V && !negativeCycle; u++) {
            if (dist[u] == std::numeric_limits<int>::max()) continue;
            const LinkedList& neighbors = adjList->getNeighbors(u);
            for (const auto& edge : neighbors) {
                if (dist[u] + edge.weight < dist[edge.end]) {
                    negativeCycle = true;
                    break;
                }
            }
        }
    } else if (incMatrix) {
        DynamicArray edges = incMatrix->getEdges();
        for (int i = 0; i < edges.getSize() && !negativeCycle; i++) {
            const Edge& edge = edges[i];
            if (dist[edge.start] != std::numeric_limits<int>::max()
                && dist[edge.start] + edge.weight < dist[edge.end]) {
                negativeCycle = true;
            }
        }
    }

    if (negativeCycle) {
        std::cerr << "Blad: Graf zawiera ujemny cykl!" << std::endl;
        delete[] dist;
        delete[] parent;
        return result;
    }

    result.dist   = new int[V];
    result.parent = new int[V];
    result.size   = V;
    for (int i = 0; i < V; i++) {
        result.dist[i]   = dist[i];
        result.parent[i] = parent[i];
    }
    result.success = true;

    delete[] dist;
    delete[] parent;

    return result;
}

void BellmanFordSP::printSP(const SPResult& result, int startVertex, int endVertex, int numVertices) {
    if (!result.success) {
        std::cout << "SP nie znalezione (blad lub ujemny cykl)" << std::endl;
        return;
    }

    int cost = result.dist[endVertex];
    if (cost == std::numeric_limits<int>::max()) {
        std::cout << "Brak sciezki z " << startVertex << " do " << endVertex << std::endl;
        return;
    }

    int* path    = new int[numVertices];
    int  pathLen = 0;
    int  cur     = endVertex;

    while (cur != -1) {
        path[pathLen++] = cur;
        cur = result.parent[cur];
    }

    std::cout << "Najkrotsza sciezka (Bellman-Ford): "
              << startVertex << " -> " << endVertex << std::endl;
    std::cout << "Koszt: " << cost << std::endl;
    std::cout << "Sciezka: ";
    for (int i = pathLen - 1; i >= 0; i--) {
        std::cout << path[i];
        if (i > 0) std::cout << " -> ";
    }
    std::cout << std::endl;

    delete[] path;
}
