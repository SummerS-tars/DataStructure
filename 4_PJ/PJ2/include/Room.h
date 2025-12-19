#pragma once
#include <vector>
#include <optional>
#include "Item.h"

enum class RoomType { START, NORMAL, BOSS, TREASURE };

struct Room {
    int id = -1;
    int monster_power = 0; // 0 表示无怪
    bool is_boss = false;
    // 可选：网格坐标，仅用于 GridMap 可视化与邻接计算（不影响旧逻辑与序列化）
    int x = -1;
    int y = -1;
    int tier = -1;             // 距离起点的层级/步数
    RoomType type = RoomType::NORMAL;
    std::string label;         // 供前端展示的名称
    std::vector<Item> chest_items; // 简化为多个物品组成的宝箱
};
