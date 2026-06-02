#include "GraphGenerator.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iostream>

GraphGenerator::GraphGenerator(int v, float d, bool directed) 
    : vertices(v), density(d), isDirected(directed), edgeExists(nullptr) {
    
    if (density < 0.0f || density > 1.0f) {
        std::cerr << "Uwaga: gestosc powinna byc w zakresie [0, 1]. Korygowanie wartosci." << std::endl;
        if (density < 0.0f) density = 0.0f;
        if (density > 1.0f) density = 1.0f;
    }
    
    srand(static_cast<unsigned int>(time(nullptr)));
}

GraphGenerator::~GraphGenerator() {
    cleanupEdgeTracking();
}

void GraphGenerator::initEdgeTracking() {
    edgeExists = new bool*[vertices];
    for (int i = 0; i < vertices; i++) {
        edgeExists[i] = new bool[vertices];
        for (int j = 0; j < vertices; j++) {
            edgeExists[i][j] = false;
        }
    }
}

void GraphGenerator::cleanupEdgeTracking() {
    if (edgeExists) {
        for (int i = 0; i < vertices; i++) {
            delete[] edgeExists[i];
        }
        delete[] edgeExists;
        edgeExists = nullptr;
    }
}

int GraphGenerator::calculateMaxEdges() {
    if (isDirected) {
        return vertices * (vertices - 1);
    } else {
        return (vertices * (vertices - 1)) / 2;
    }
}

int GraphGenerator::calculateTargetEdges() {
    int maxEdges = calculateMaxEdges();
    return static_cast<int>(round(density * maxEdges));
}

int GraphGenerator::getRandomWeight(int numEdges) {
    int maxW = (numEdges * 4) / 5;
    if (maxW < 1) maxW = 1;
    return (rand() % maxW) + 1;
}

bool GraphGenerator::edgeAlreadyExists(int start, int end) {
    return edgeExists[start][end];
}

void GraphGenerator::markEdgeAsUsed(int start, int end) {
    edgeExists[start][end] = true;
    if (!isDirected) {
        edgeExists[end][start] = true;
    }
}

void GraphGenerator::generateSpanningTree(Graph* graph, int& currentEdges) {
    bool* visited = new bool[vertices];
    for (int i = 0; i < vertices; i++) {
        visited[i] = false;
    }
    
    visited[0] = true;
    currentEdges = 0;
    
    int targetEdges = calculateTargetEdges();
    
    for (int i = 1; i < vertices; i++) {
        int source = -1;
        int dest = -1;
        
        do {
            source = rand() % i;
            int visitedCount = 0;
            for (int j = 0; j < vertices && visitedCount <= source; j++) {
                if (visited[j]) {
                    if (visitedCount == source) {
                        source = j;
                        break;
                    }
                    visitedCount++;
                }
            }
            
            dest = rand() % (vertices - i) + i;
            int unvisitedCount = 0;
            for (int j = 0; j < vertices && unvisitedCount < dest - i + 1; j++) {
                if (!visited[j]) {
                    if (unvisitedCount == dest - i) {
                        dest = j;
                        break;
                    }
                    unvisitedCount++;
                }
            }
            
        } while (edgeAlreadyExists(source, dest));
        
        int weight = getRandomWeight(targetEdges);
        graph->addEdge(source, dest, weight);
        markEdgeAsUsed(source, dest);
        visited[dest] = true;
        currentEdges++;
    }
    
    delete[] visited;
}

void GraphGenerator::addRemainingEdges_LowDensity(Graph* graph, int targetEdges, int currentEdges) {
    int remaining = targetEdges - currentEdges;
    
    while (remaining > 0) {
        int source = rand() % vertices;
        int dest = rand() % vertices;
        
        if (source == dest) continue;
        if (edgeAlreadyExists(source, dest)) continue;
        
        int weight = getRandomWeight(targetEdges);
        graph->addEdge(source, dest, weight);
        markEdgeAsUsed(source, dest);
        remaining--;
    }
}

void GraphGenerator::addRemainingEdges_HighDensity(Graph* graph, int targetEdges) {
    struct EdgePair {
        int start;
        int end;
        bool used;
        
        EdgePair() : start(0), end(0), used(false) {}
        EdgePair(int s, int e) : start(s), end(e), used(false) {}
    };
    
    int maxEdges = calculateMaxEdges();
    EdgePair* allEdges = new EdgePair[maxEdges];
    int edgeCount = 0;
    
    for (int i = 0; i < vertices; i++) {
        int jStart = isDirected ? 0 : i + 1;
        for (int j = jStart; j < vertices; j++) {
            if (i != j) {
                allEdges[edgeCount] = EdgePair(i, j);
                if (edgeAlreadyExists(i, j)) {
                    allEdges[edgeCount].used = true;
                }
                edgeCount++;
            }
        }
    }
    
    int toRemove = maxEdges - targetEdges;
    int removed = 0;
    
    while (removed < toRemove) {
        int idx = rand() % edgeCount;
        
        if (!allEdges[idx].used) {
            allEdges[idx].used = true;
            removed++;
        }
    }
    
    for (int i = 0; i < edgeCount; i++) {
        if (!allEdges[i].used) {
            int weight = getRandomWeight(targetEdges);
            graph->addEdge(allEdges[i].start, allEdges[i].end, weight);
        }
    }
    
    delete[] allEdges;
}

Graph* GraphGenerator::generate(bool useAdjacencyList) {
    initEdgeTracking();
    
    Graph* graph = nullptr;
    if (useAdjacencyList) {
        graph = new AdjacencyList(vertices, isDirected);
    } else {
        graph = new IncidenceMatrix(vertices, isDirected);
    }
    
    int targetEdges = calculateTargetEdges();
    
    if (targetEdges < vertices - 1) {
        targetEdges = vertices - 1;
    }
    
    int currentEdges = 0;
    generateSpanningTree(graph, currentEdges);
    
    if (currentEdges < targetEdges) {
        if (density <= 0.5f) {
            addRemainingEdges_LowDensity(graph, targetEdges, currentEdges);
        } else {
            addRemainingEdges_HighDensity(graph, targetEdges);
        }
    }
    
    cleanupEdgeTracking();
    return graph;
}

void GraphGenerator::saveToFile(Graph* graph, const char* filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Blad: Nie mozna otworzyc pliku " << filename << " do zapisu." << std::endl;
        return;
    }
    
    file << graph->getVerticesCount() << "\t" << graph->getEdgesCount() << std::endl;
    
    DynamicArray edges = graph->getEdges();
    for (int i = 0; i < edges.getSize(); i++) {
        const Edge& edge = edges[i];
        file << edge.start << "\t" << edge.end << "\t" << edge.weight << std::endl;
    }
    
    file.close();
    std::cout << "Graf zapisany do pliku: " << filename << std::endl;
    std::cout << "Wierzcholki: " << graph->getVerticesCount() << ", Krawedzie: " << graph->getEdgesCount() << std::endl;
}
