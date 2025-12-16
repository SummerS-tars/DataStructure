#pragma once
#include <vector>
#include <optional>
#include "DungeonMap.h"

struct PathInfo {
    std::vector<int> path;
    int total_value = 0;
    int total_power_bonus = 0;
    bool feasible = true; // 若提供 initial_power，则表示是否可通过（玩家当前武力与沿途加成可击败怪物）
};

class PathFinder {
public:
    // 最少探索次数（最少房间数）的路径；若不可达返回空
    static std::vector<int> find_shortest_path(const DungeonMap& map, int start, int goal);

    // 枚举所有无环路径，按战利品价值降序返回；
    // 若 initial_power >= 0，则仅返回可行路径（玩家武力约束）；否则不做约束。
    static std::vector<PathInfo> find_all_paths_sorted(const DungeonMap& map, int start, int goal, size_t max_paths = 1000, int initial_power = -1);

    // 工具函数：计算路径价值与武力加成、可行性
    static PathInfo evaluate_path(const DungeonMap& map, const std::vector<int>& path, int initial_power = -1);
};
