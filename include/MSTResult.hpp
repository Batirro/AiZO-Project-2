#pragma once

#include "DynamicArray.hpp"

struct MSTResult {
    DynamicArray edges;
    int totalCost;
    bool success;
    
    MSTResult() : totalCost(0), success(false) {}
};
