#include "../include/PathFinder.h"
#include <queue>
#include <algorithm>
#include <unordered_set>

std::vector<int> PathFinder::find_shortest_path(const DungeonMap& map, int start, int goal) {
    if (start == goal) return {start};
    std::unordered_map<int, int> parent;
    std::unordered_set<int> vis;
    std::queue<int> q;
    q.push(start); vis.insert(start);
    bool found = false;
    while (!q.empty() && !found) {
        int u = q.front(); q.pop();
        for (int v : map.neighbors(u)) {
            if (!vis.count(v)) {
                vis.insert(v);
                parent[v] = u;
                if (v == goal) { found = true; break; }
                q.push(v);
            }
        }
    }
    if (!found) return {};
    // 回溯
    std::vector<int> path; int cur = goal;
    while (cur != start) { path.push_back(cur); cur = parent[cur]; }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

PathInfo PathFinder::evaluate_path(const DungeonMap& map, const std::vector<int>& path, int initial_power) {
    PathInfo info; info.path = path; info.total_value = 0; info.total_power_bonus = 0; info.feasible = true;
    int cur_power = initial_power;
    for (int id : path) {
        const Room* r = map.get_room(id);
        if (!r) continue;
        // 若启用约束，要求 cur_power > monster_power，否则不可行
        if (initial_power >= 0) {
            if (cur_power <= r->monster_power) { info.feasible = false; }
        }
        for (const auto& it : r->chest_items) {
            info.total_value += it.value;
            info.total_power_bonus += it.power_bonus;
            if (initial_power >= 0) cur_power += it.power_bonus;
        }
    }
    return info;
}

static void dfs_paths_limit(const DungeonMap& map, int u, int goal,
                            std::unordered_set<int>& used,
                            std::vector<int>& cur,
                            std::vector<std::vector<int>>& out,
                            size_t max_paths) {
    if (out.size() >= max_paths) return;
    if (u == goal) { out.push_back(cur); return; }
    for (int v : map.neighbors(u)) {
        if (!used.count(v)) {
            used.insert(v); cur.push_back(v);
            dfs_paths_limit(map, v, goal, used, cur, out, max_paths);
            cur.pop_back(); used.erase(v);
            if (out.size() >= max_paths) return;
        }
    }
}

std::vector<PathInfo> PathFinder::find_all_paths_sorted(const DungeonMap& map, int start, int goal, size_t max_paths, int initial_power) {
    std::vector<std::vector<int>> raw;
    std::unordered_set<int> used; used.insert(start);
    std::vector<int> cur{start};
    dfs_paths_limit(map, start, goal, used, cur, raw, max_paths);

    std::vector<PathInfo> infos; infos.reserve(raw.size());
    for (const auto& p : raw) {
        auto info = evaluate_path(map, p, initial_power);
        if (initial_power >= 0) {
            if (info.feasible) infos.push_back(std::move(info));
        } else {
            infos.push_back(std::move(info));
        }
    }
    std::sort(infos.begin(), infos.end(), [](const PathInfo& a, const PathInfo& b){
        if (a.total_value != b.total_value) return a.total_value > b.total_value;
        return a.path.size() < b.path.size(); // 价值相同则更短优先
    });
    return infos;
}
