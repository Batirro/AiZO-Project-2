#include "AdjacencyList.hpp"

AdjacencyList::AdjacencyList(int v, bool directed) : Graph(v, directed) {
    list = new LinkedList[vertices];
}

AdjacencyList::~AdjacencyList() {
    delete[] list;
}

void AdjacencyList::addEdge(int start, int end, int weight) {
    list[start].push_back(Edge(start, end, weight));
    
    if (!isDirected) {
        // For undirected graphs, add the reverse edge too
        list[end].push_back(Edge(end, start, weight));
    }
    numEdges++;
}

void AdjacencyList::print() const {
    std::cout << "Adjacency List Representation:" << std::endl;
    for (int i = 0; i < vertices; ++i) {
        std::cout << i << " -> ";
        for (const auto& edge : list[i]) {
            std::cout << "[" << edge.end << "|" << edge.weight << "] ";
        }
        std::cout << std::endl;
    }
}

const LinkedList& AdjacencyList::getNeighbors(int vertex) const {
    if (vertex < 0 || vertex >= vertices) {
        throw std::out_of_range("Vertex index out of bounds");
    }
    return list[vertex];
}
