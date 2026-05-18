#pragma once

#include "Graph.hpp"
#include "LinkedList.hpp"

class AdjacencyList : public Graph {
private:
    LinkedList* list; // Array of LinkedLists

public:
    AdjacencyList(int v, bool directed = false);
    ~AdjacencyList() override;

    void addEdge(int start, int end, int weight) override;
    void print() const override;
    
    // Allows iterating over neighbors for given vertex
    const LinkedList& getNeighbors(int vertex) const;
};
