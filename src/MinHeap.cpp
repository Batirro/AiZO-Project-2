#include "MinHeap.hpp"

MinHeap::MinHeap() : capacity(10), size(0) {
    data = new HeapNode[capacity];
}

MinHeap::~MinHeap() {
    delete[] data;
}

MinHeap::MinHeap(const MinHeap& other)
    : capacity(other.capacity), size(other.size) {
    data = new HeapNode[capacity];
    for (int i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

MinHeap& MinHeap::operator=(const MinHeap& other) {
    if (this != &other) {
        delete[] data;
        capacity = other.capacity;
        size = other.size;
        data = new HeapNode[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

MinHeap::MinHeap(MinHeap&& other) noexcept
    : data(other.data), capacity(other.capacity), size(other.size) {
    other.data = nullptr;
    other.capacity = 0;
    other.size = 0;
}

MinHeap& MinHeap::operator=(MinHeap&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        capacity = other.capacity;
        size = other.size;
        other.data = nullptr;
        other.capacity = 0;
        other.size = 0;
    }
    return *this;
}

void MinHeap::resize(int newCapacity) {
    HeapNode* newData = new HeapNode[newCapacity];
    for (int i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

void MinHeap::swap(HeapNode& a, HeapNode& b) {
    HeapNode temp = a;
    a = b;
    b = temp;
}

void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (data[index].cost < data[parent].cost) {
            swap(data[index], data[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

void MinHeap::heapifyDown(int index) {
    while (true) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int smallest = index;

        if (leftChild < size && data[leftChild].cost < data[smallest].cost) {
            smallest = leftChild;
        }
        if (rightChild < size && data[rightChild].cost < data[smallest].cost) {
            smallest = rightChild;
        }

        if (smallest != index) {
            swap(data[index], data[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

void MinHeap::push(int vertex, int cost) {
    if (size == capacity) {
        resize(capacity * 2);
    }
    data[size] = HeapNode(vertex, cost);
    heapifyUp(size);
    size++;
}

HeapNode MinHeap::pop() {
    if (isEmpty()) {
        throw std::out_of_range("Heap is empty");
    }
    HeapNode root = data[0];
    data[0] = data[size - 1];
    size--;
    heapifyDown(0);
    return root;
}

bool MinHeap::isEmpty() const {
    return size == 0;
}

void MinHeap::clear() {
    size = 0;
}
