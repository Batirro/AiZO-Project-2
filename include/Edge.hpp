#pragma once

struct Edge {
    int start;
    int end;
    int weight;

    Edge(int start = 0, int end = 0, int weight = 0) 
        : start(start), end(end), weight(weight) {}
};
