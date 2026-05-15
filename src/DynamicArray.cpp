#include "DynamicArray.hpp"

DynamicArray::DynamicArray() : capacity(2), size(0) {
    data = new Edge[capacity];
}

DynamicArray::~DynamicArray() {
    delete[] data;
}

void DynamicArray::resize(int newCapacity) {
    Edge* newData = new Edge[newCapacity];
    for (int i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

void DynamicArray::push_back(const Edge& edge) {
    if (size == capacity) {
        resize(capacity * 2);
    }
    data[size++] = edge;
}

void DynamicArray::pop_back() {
    if (size > 0) {
        size--;
    }
}

Edge& DynamicArray::operator[](int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

const Edge& DynamicArray::operator[](int index) const {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

int DynamicArray::getSize() const {
    return size;
}

void DynamicArray::clear() {
    size = 0;
}
