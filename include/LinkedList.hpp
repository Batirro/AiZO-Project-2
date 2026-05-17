#pragma once

#include "Edge.hpp"

class LinkedList {
private:
    struct Node {
        Edge data;
        Node* next;
        Node* prev;

        Node(const Edge& e) : data(e), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    LinkedList();
    ~LinkedList();

    void push_back(const Edge& edge);
    void pop_back();
    void push_front(const Edge& edge);
    void pop_front();

    // Iterator-like access is useful, but simple array-like indexing is easier for basic needs
    // Note: O(n) access time. For O(1) we would need iterators, but this is sufficient for graph representation iteration
    Edge& operator[](int index);
    const Edge& operator[](int index) const;

    int getSize() const;
    void clear();
    
    // Iterator support for range-based for loops
    class Iterator {
    private:
        Node* current;
    public:
        Iterator(Node* node) : current(node) {}
        Edge& operator*() { return current->data; }
        Iterator& operator++() { current = current->next; return *this; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
    };
    
    class ConstIterator {
    private:
        const Node* current;
    public:
        ConstIterator(const Node* node) : current(node) {}
        const Edge& operator*() const { return current->data; }
        ConstIterator& operator++() { current = current->next; return *this; }
        bool operator!=(const ConstIterator& other) const { return current != other.current; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
    ConstIterator begin() const { return ConstIterator(head); }
    ConstIterator end() const { return ConstIterator(nullptr); }
};
