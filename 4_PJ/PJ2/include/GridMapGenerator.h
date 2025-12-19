#pragma once
#include "DungeonMap.h"
#include <random>

struct GridConfig {
    int width = 5;
    int height = 4;
    unsigned int seed = std::random_device{}();
    int monster_min = 0;
    int monster_max = 50;
    int max_items_per_room = 2;
};

class GridMapGenerator {
public:
    static DungeonMap generate(const GridConfig& cfg);
};
