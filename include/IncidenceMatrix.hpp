#pragma once

#include "Graph.hpp"
#include "Edge.hpp"
#include <iomanip>

class IncidenceMatrix : public Graph {
private:
    int** matrix; // 2D array: vertices x edges
    Edge* edgeInfo; // Array to store edge information
    int capacityEdges;

    void resizeMatrix(int newCapacityEdges);

public:
    IncidenceMatrix(int v, bool directed = false);
    ~IncidenceMatrix() override;

    void addEdge(int start, int end, int weight) override;
    void print() const override;
    DynamicArray getEdges() const override;
};
