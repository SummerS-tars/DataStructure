#include <iostream>
#include <vector>
#include "../include/MapGenerator.h"
#include "../include/Player.h"
#include "../include/PathFinder.h"

int main(int argc, char** argv) {
    GenConfig cfg;
    cfg.room_num = 20;
    if (argc > 1) {
        try { cfg.room_num = std::max(20, std::stoi(argv[1])); } catch (...) {}
    }
    auto map = MapGenerator::generate(cfg);
    std::cout << map.to_string() << std::endl;
    std::cout << (map.is_connected() ? "Connected" : "NotConnected") << std::endl;

    // Phase 2 演示：玩家探索 + 战利品树 + 日志
    Player player(10);
    // 简单路径示例：取 room_ids 的前 3 个房间
    auto ids = map.room_ids();
    std::vector<int> demo_path;
    for (size_t i = 0; i < ids.size() && i < 3; ++i) demo_path.push_back(ids[i]);
    player.explore_path(map, demo_path);

    std::cout << "\n== LootTree ==\n";
    player.loot().print(std::cout, false);
    auto [tv, tp] = player.loot().totals();
    std::cout << "Totals: value=" << tv << " power_bonus=" << tp << " current_power=" << player.power() << "\n";

    std::cout << "\n== Favorites (empty by default) ==\n";
    player.loot().print(std::cout, true);

    std::cout << "\n== Adventure Log ==\n";
    player.log().print(std::cout);

    // Phase 3 演示：最短路径与高价值路径（从最小房间ID到Boss）
    if (!ids.empty() && map.boss_room_id() != -1) {
        int start = ids.front();
        int goal = map.boss_room_id();
        auto sp = PathFinder::find_shortest_path(map, start, goal);
        std::cout << "\n== Shortest Path (" << start << " -> " << goal << ") ==\n";
        for (size_t i=0;i<sp.size();++i) { if (i) std::cout << "->"; std::cout << sp[i]; }
        std::cout << "\n";

        auto all = PathFinder::find_all_paths_sorted(map, start, goal, /*max_paths=*/200);
        std::cout << "\n== Top Value Paths (top 3) ==\n";
        for (size_t i=0;i<all.size() && i<3; ++i) {
            const auto& pi = all[i];
            std::cout << "#" << (i+1) << " val=" << pi.total_value << " pow+=" << pi.total_power_bonus << " path=";
            for (size_t j=0;j<pi.path.size();++j) { if (j) std::cout << "->"; std::cout << pi.path[j]; }
            std::cout << "\n";
        }
    }
    return 0;
}
