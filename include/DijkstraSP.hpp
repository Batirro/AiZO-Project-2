#pragma once
#include "Graph.hpp"
#include "SPResult.hpp"

class DijkstraSP {
public:
    SPResult findSP(Graph* graph, int startVertex);
    void printSP(const SPResult& result, int startVertex, int endVertex, int numVertices);
};
