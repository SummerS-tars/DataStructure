#pragma once
#include <vector>
#include <optional>
#include "Item.h"

struct Room {
    int id = -1;
    int monster_power = 0; // 0 表示无怪
    bool is_boss = false;
    std::vector<Item> chest_items; // 简化为多个物品组成的宝箱
};
