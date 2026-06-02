#pragma once

#include "Graph.hpp"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"

class GraphLoader {
public:
    static Graph* loadFromFile(const char* filename, bool useAdjacencyList = true, bool isDirected = false);
    
private:
    static bool validateGraph(Graph* graph, int expectedEdges);
};
