#pragma once

class UnionFind {
private:
    int* parent;
    int* rank;
    int size;

public:
    UnionFind(int n);
    ~UnionFind();
    
    int find(int x);
    bool unionSets(int x, int y);
};
