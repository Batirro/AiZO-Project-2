#include "LinkedList.hpp"
#include <stdexcept>

LinkedList::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

LinkedList::~LinkedList() {
    clear();
}

void LinkedList::push_back(const Edge& edge) {
    Node* newNode = new Node(edge);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

void LinkedList::pop_back() {
    if (!tail) return;

    Node* toDelete = tail;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        tail = tail->prev;
        tail->next = nullptr;
    }
    delete toDelete;
    size--;
}

void LinkedList::push_front(const Edge& edge) {
    Node* newNode = new Node(edge);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    size++;
}

void LinkedList::pop_front() {
    if (!head) return;

    Node* toDelete = head;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        head = head->next;
        head->prev = nullptr;
    }
    delete toDelete;
    size--;
}

Edge& LinkedList::operator[](int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    Node* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

const Edge& LinkedList::operator[](int index) const {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    const Node* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

int LinkedList::getSize() const {
    return size;
}

void LinkedList::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = tail = nullptr;
    size = 0;
}
