#pragma once

#include "Graph.hpp"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"
#include "MinHeap.hpp"
#include "MSTResult.hpp"

class PrimMST {
public:
    static MSTResult findMST(Graph* graph, int startVertex = 0);
    static void printMST(const MSTResult& result);
};
