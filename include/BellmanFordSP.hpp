#pragma once
#include "Graph.hpp"
#include "SPResult.hpp"

class BellmanFordSP {
public:
    static SPResult findSP(Graph* graph, int startVertex);
    static void printSP(const SPResult& result, int startVertex, int endVertex, int numVertices);
};
