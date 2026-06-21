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

    DynamicArray(const DynamicArray& other);
    DynamicArray& operator=(const DynamicArray& other);
    DynamicArray(DynamicArray&& other) noexcept;
    DynamicArray& operator=(DynamicArray&& other) noexcept;

    void push_back(const Edge& edge);
    void pop_back();
    Edge& operator[](int index);
    const Edge& operator[](int index) const;
    int getSize() const;
    void clear();
};
