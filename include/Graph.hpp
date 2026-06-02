#pragma once

#include "DynamicArray.hpp"
#include <iostream>

class Graph {
protected:
    int vertices;
    int numEdges;
    bool isDirected;

public:
    Graph(int v, bool directed = false) : vertices(v), numEdges(0), isDirected(directed) {}
    virtual ~Graph() = default;

    virtual void addEdge(int start, int end, int weight) = 0;
    virtual void print() const = 0;
    virtual DynamicArray getEdges() const = 0;
    
    int getVerticesCount() const { return vertices; }
    int getEdgesCount() const { return numEdges; }
    bool getIsDirected() const { return isDirected; }
};
