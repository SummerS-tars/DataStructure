#pragma once
#include <random>
#include "DungeonMap.h"

struct GenConfig {
    int room_num = 20;
    unsigned int seed = std::random_device{}();
    int monster_min = 0;
    int monster_max = 50;
    int extra_edges = 20; // 在生成主干树后增加的随机边数量
};

class MapGenerator {
public:
    static DungeonMap generate(const GenConfig& cfg);
};
