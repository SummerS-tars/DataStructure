#include <cassert>
#include <random>
#include <unordered_set>
#include <iostream>
#include "../include/MapGenerator.h"
#include "../include/PathFinder.h"
#include "../include/BossManager.h"

static void test_boss_move_neighbor() {
    GenConfig cfg; cfg.room_num = 25; cfg.extra_edges = 25; cfg.seed = 123;
    auto map = MapGenerator::generate(cfg);
    int prev = map.boss_room_id();
    std::mt19937 rng{cfg.seed};
    int next = BossManager::move_boss_once(map, rng);
    assert(next != -1);
    // 新 Boss 必须是旧 Boss 的邻居
    const auto& nb = map.neighbors(prev);
    bool ok=false; for (int v: nb) if (v==next) ok=true; assert(ok);
    // 标记唯一性：仅 next 为 Boss
    for (int id : map.room_ids()) {
        const Room* r = map.get_room(id);
        if (!r) continue;
        if (id == next) { assert(r->is_boss); }
        else { assert(!r->is_boss); }
    }
}

static void test_replan_after_move() {
    GenConfig cfg; cfg.room_num = 20; cfg.extra_edges = 15;
    auto map = MapGenerator::generate(cfg);
    auto ids = map.room_ids();
    int start = ids.front();
    int old_goal = map.boss_room_id();
    auto p1 = PathFinder::find_shortest_path(map, start, old_goal);
    std::mt19937 rng{cfg.seed};
    int new_goal = BossManager::move_boss_once(map, rng);
    auto p2 = PathFinder::find_shortest_path(map, start, new_goal);
    assert(!p1.empty());
    assert(!p2.empty());
    // 若目标变更，路径有可能变化（不强制不同，但至少可重新求得）
}

int main(){
    test_boss_move_neighbor();
    test_replan_after_move();
    std::cout << "[TEST PASS] Phase4 tests passed.\n";
    return 0;
}
