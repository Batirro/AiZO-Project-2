#pragma once

#include <stdexcept>

// Simple MinHeap node storing vertex and its current cost/distance
struct HeapNode {
    int vertex;
    int cost;

    HeapNode(int v = 0, int c = 0) : vertex(v), cost(c) {}
};

class MinHeap {
private:
    HeapNode* data;
    int capacity;
    int size;

    void resize(int newCapacity);
    void heapifyUp(int index);
    void heapifyDown(int index);
    void swap(HeapNode& a, HeapNode& b);

public:
    MinHeap();
    ~MinHeap();

    void push(int vertex, int cost);
    HeapNode pop();
    bool isEmpty() const;
    void clear();
};
