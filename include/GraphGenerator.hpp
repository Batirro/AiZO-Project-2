#pragma once

#include "Graph.hpp"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"

class GraphGenerator {
private:
    int vertices;
    float density;
    bool isDirected;
    int maxWeight;
    
    bool** edgeExists;
    
    void initEdgeTracking();
    void cleanupEdgeTracking();
    
    void generateSpanningTree(Graph* graph, int& currentEdges);
    void addRemainingEdges_LowDensity(Graph* graph, int targetEdges, int currentEdges);
    void addRemainingEdges_HighDensity(Graph* graph, int targetEdges);
    
    int getRandomWeight(int numEdges);
    bool edgeAlreadyExists(int start, int end);
    void markEdgeAsUsed(int start, int end);
    int calculateMaxEdges();
    int calculateTargetEdges();

public:
    GraphGenerator(int v, float d, bool directed);
    ~GraphGenerator();
    
    Graph* generate(bool useAdjacencyList = true);
    void saveToFile(Graph* graph, const char* filename);
};
