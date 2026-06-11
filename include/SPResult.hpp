#pragma once

struct SPResult {
    int* dist;
    int* parent;
    int size;
    bool success;
    
    SPResult() : dist(nullptr), parent(nullptr), size(0), success(false) {}
    ~SPResult() {
        delete[] dist;
        delete[] parent;
    }
};
