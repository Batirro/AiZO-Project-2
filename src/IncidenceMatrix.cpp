#include "IncidenceMatrix.hpp"

IncidenceMatrix::IncidenceMatrix(int v, bool directed) : Graph(v, directed) {
    capacityEdges = 10; // Initial arbitrary capacity
    matrix = new int*[vertices];
    for (int i = 0; i < vertices; ++i) {
        matrix[i] = new int[capacityEdges];
        for (int j = 0; j < capacityEdges; ++j) {
            matrix[i][j] = 0;
        }
    }
}

IncidenceMatrix::~IncidenceMatrix() {
    for (int i = 0; i < vertices; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void IncidenceMatrix::resizeMatrix(int newCapacityEdges) {
    int** newMatrix = new int*[vertices];
    for (int i = 0; i < vertices; ++i) {
        newMatrix[i] = new int[newCapacityEdges];
        for (int j = 0; j < capacityEdges; ++j) {
            newMatrix[i][j] = matrix[i][j];
        }
        for (int j = capacityEdges; j < newCapacityEdges; ++j) {
            newMatrix[i][j] = 0;
        }
    }

    for (int i = 0; i < vertices; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    matrix = newMatrix;
    capacityEdges = newCapacityEdges;
}

void IncidenceMatrix::addEdge(int start, int end, int weight) {
    if (numEdges == capacityEdges) {
        resizeMatrix(capacityEdges * 2);
    }

    int edgeIndex = numEdges;
    
    if (isDirected) {
        matrix[start][edgeIndex] = weight;
        matrix[end][edgeIndex] = -weight;
    } else {
        matrix[start][edgeIndex] = weight;
        matrix[end][edgeIndex] = weight;
    }

    numEdges++;
}

void IncidenceMatrix::print() const {
    std::cout << "Incidence Matrix Representation:" << std::endl;
    // Print edge header
    std::cout << "    ";
    for (int j = 0; j < numEdges; ++j) {
        std::cout << "e" << std::left << std::setw(3) << j;
    }
    std::cout << std::endl;

    for (int i = 0; i < vertices; ++i) {
        std::cout << std::left << std::setw(3) << i << "|";
        for (int j = 0; j < numEdges; ++j) {
            std::cout << std::right << std::setw(3) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
