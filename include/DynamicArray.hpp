#pragma once

#include "Edge.hpp"
#include <stdexcept>

class DynamicArray {
private:
    Edge* data;
    int capacity;
    int size;

    void resize(int newCapacity);

public:
    DynamicArray();
    ~DynamicArray();

    void push_back(const Edge& edge);
    void pop_back();
    Edge& operator[](int index);
    const Edge& operator[](int index) const;
    int getSize() const;
    void clear();
};
