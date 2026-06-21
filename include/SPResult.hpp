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

    SPResult(const SPResult& other) : size(other.size), success(other.success) {
        if (other.dist) {
            dist = new int[size];
            for (int i = 0; i < size; ++i) dist[i] = other.dist[i];
        } else {
            dist = nullptr;
        }
        if (other.parent) {
            parent = new int[size];
            for (int i = 0; i < size; ++i) parent[i] = other.parent[i];
        } else {
            parent = nullptr;
        }
    }

    SPResult& operator=(const SPResult& other) {
        if (this != &other) {
            delete[] dist;
            delete[] parent;
            size = other.size;
            success = other.success;
            if (other.dist) {
                dist = new int[size];
                for (int i = 0; i < size; ++i) dist[i] = other.dist[i];
            } else {
                dist = nullptr;
            }
            if (other.parent) {
                parent = new int[size];
                for (int i = 0; i < size; ++i) parent[i] = other.parent[i];
            } else {
                parent = nullptr;
            }
        }
        return *this;
    }

    SPResult(SPResult&& other) noexcept
        : dist(other.dist), parent(other.parent), size(other.size), success(other.success) {
        other.dist = nullptr;
        other.parent = nullptr;
        other.size = 0;
    }

    SPResult& operator=(SPResult&& other) noexcept {
        if (this != &other) {
            delete[] dist;
            delete[] parent;
            dist = other.dist;
            parent = other.parent;
            size = other.size;
            success = other.success;
            other.dist = nullptr;
            other.parent = nullptr;
            other.size = 0;
        }
        return *this;
    }
};
