#pragma once
#include <vector>
#include <optional>
#include "Item.h"

struct Room {
    int id = -1;
    int monster_power = 0; // 0 表示无怪
    bool is_boss = false;
    // 可选：网格坐标，仅用于 GridMap 可视化与邻接计算（不影响旧逻辑与序列化）
    int x = -1;
    int y = -1;
    std::vector<Item> chest_items; // 简化为多个物品组成的宝箱
};
