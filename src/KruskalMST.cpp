#include "KruskalMST.hpp"
#include <iostream>

void KruskalMST::merge(DynamicArray& edges, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Edge* L = new Edge[n1];
    Edge* R = new Edge[n2];

    for (int i = 0; i < n1; i++) L[i] = edges[left + i];
    for (int i = 0; i < n2; i++) R[i] = edges[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
        edges[k++] = (L[i].weight <= R[j].weight) ? L[i++] : R[j++];
    while (i < n1) edges[k++] = L[i++];
    while (j < n2) edges[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void KruskalMST::mergeSort(DynamicArray& edges, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(edges, left, mid);
    mergeSort(edges, mid + 1, right);
    merge(edges, left, mid, right);
}

void KruskalMST::sortEdges(DynamicArray& edges) {
    mergeSort(edges, 0, edges.getSize() - 1);
}

MSTResult KruskalMST::findMST(Graph* graph) {
    MSTResult result;

    if (graph->getIsDirected()) {
        std::cerr << "Blad: Algorytm Kruskala dziala tylko na grafach nieskierowanych" << std::endl;
        return result;
    }

    int vertices = graph->getVerticesCount();
    DynamicArray allEdges = graph->getEdges();

    sortEdges(allEdges);

    UnionFind uf(vertices);
    result.totalCost = 0;
    int edgesAdded = 0;

    for (int i = 0; i < allEdges.getSize() && edgesAdded < vertices - 1; i++) {
        const Edge& edge = allEdges[i];

        if (uf.unionSets(edge.start, edge.end)) {
            result.edges.push_back(edge);
            result.totalCost += edge.weight;
            edgesAdded++;
        }
    }

    result.success = (edgesAdded == vertices - 1);

    return result;
}

void KruskalMST::printMST(const MSTResult& result) {
    if (!result.success) {
        std::cout << "MST nie znalezione lub graf nie jest spojny" << std::endl;
        return;
    }

    std::cout << "Minimalne Drzewo Rozpinajace (Algorytm Kruskala):" << std::endl;
    std::cout << "Calkowity koszt: " << result.totalCost << std::endl;
    std::cout << "Krawedzie w MST:" << std::endl;

    for (int i = 0; i < result.edges.getSize(); i++) {
        const Edge& edge = result.edges[i];
        std::cout << "  " << edge.start << " -- " << edge.end
                  << " [waga: " << edge.weight << "]" << std::endl;
    }
}
