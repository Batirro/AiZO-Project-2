#pragma once

#include "Graph.hpp"
#include "UnionFind.hpp"
#include "MSTResult.hpp"

class KruskalMST {
private:
    static void sortEdges(DynamicArray& edges);
    
public:
    static MSTResult findMST(Graph* graph);
    static void printMST(const MSTResult& result);
};
