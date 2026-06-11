#include "DijkstraSP.hpp"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"
#include "MinHeap.hpp"
#include <iostream>
#include <limits>

SPResult DijkstraSP::findSP(Graph* graph, int startVertex) {
    SPResult result;

    if (!graph->getIsDirected()) {
        std::cerr << "Uwaga: Dijkstra dziala rowniez na grafach nieskierowanych" << std::endl;
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

    bool* visited = new bool[V];
    int*  dist    = new int[V];
    int*  parent  = new int[V];

    for (int i = 0; i < V; i++) {
        visited[i] = false;
        dist[i]    = std::numeric_limits<int>::max();
        parent[i]  = -1;
    }

    MinHeap minHeap;
    dist[startVertex] = 0;
    minHeap.push(startVertex, 0);

    while (!minHeap.isEmpty()) {
        HeapNode current = minHeap.pop();
        int u = current.vertex;

        if (visited[u]) continue;
        visited[u] = true;

        if (adjList) {
            const LinkedList& neighbors = adjList->getNeighbors(u);
            for (const auto& edge : neighbors) {
                int v      = edge.end;
                int weight = edge.weight;

                if (!visited[v] && dist[u] != std::numeric_limits<int>::max()
                    && dist[u] + weight < dist[v]) {
                    dist[v]   = dist[u] + weight;
                    parent[v] = u;
                    minHeap.push(v, dist[v]);
                }
            }
        } else if (incMatrix) {
            DynamicArray edges = incMatrix->getEdges();
            for (int i = 0; i < edges.getSize(); i++) {
                const Edge& edge = edges[i];

                // Dla grafu skierowanego: tylko krawędzie wychodzące z u
                if (edge.start == u) {
                    int v      = edge.end;
                    int weight = edge.weight;

                    if (!visited[v] && dist[u] != std::numeric_limits<int>::max()
                        && dist[u] + weight < dist[v]) {
                        dist[v]   = dist[u] + weight;
                        parent[v] = u;
                        minHeap.push(v, dist[v]);
                    }
                }
            }
        }
    }

    result.dist = new int[V];
    result.parent = new int[V];
    result.size = V;
    
    for (int i = 0; i < V; i++) {
        result.dist[i] = dist[i];
        result.parent[i] = parent[i];
    }
    result.success = true;

    delete[] visited;
    delete[] dist;
    delete[] parent;

    return result;
}

void DijkstraSP::printSP(const SPResult& result, int startVertex, int endVertex, int numVertices) {
    if (!result.success) {
        std::cout << "SP nie znalezione" << std::endl;
        return;
    }

    int cost = result.dist[endVertex];
    if (cost == std::numeric_limits<int>::max()) {
        std::cout << "Brak sciezki z " << startVertex << " do " << endVertex << std::endl;
        return;
    }

    // Odtworzenie ścieżki przez cofanie po parent[]
    int* path    = new int[numVertices];
    int  pathLen = 0;
    int  cur     = endVertex;

    while (cur != -1) {
        path[pathLen++] = cur;
        cur = result.parent[cur];
    }

    std::cout << "Najkrotsza sciezka (Dijkstra): "
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
