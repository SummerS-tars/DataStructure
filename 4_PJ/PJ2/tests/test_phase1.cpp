#include <cassert>
#include <iostream>
#include <filesystem>
#include "../include/MapGenerator.h"
#include "../include/DungeonMap.h"

static void test_connectivity_and_boss() {
    GenConfig cfg; cfg.room_num = 25; cfg.extra_edges = 25;
    auto map = MapGenerator::generate(cfg);
    // 连通性
    assert(map.room_count() >= 20);
    assert(map.is_connected());
    // Boss 唯一且为最大怪力
    int boss_id = map.boss_room_id();
    assert(boss_id != -1);
    const Room* boss = map.get_room(boss_id);
    assert(boss && boss->is_boss);
    for (int id : map.room_ids()) {
        const Room* r = map.get_room(id);
        if (!r) continue;
        assert(boss->monster_power >= r->monster_power);
        if (id != boss_id) assert(!r->is_boss);
    }
}

static void test_save_and_load_roundtrip() {
    GenConfig cfg; cfg.room_num = 22; cfg.extra_edges = 15;
    auto map = MapGenerator::generate(cfg);
    const char* path = "build/tmp_map.txt";
    std::filesystem::create_directories("build");
    // 保存
    bool ok = map.save(path);
    assert(ok);
    // 加载
    auto loaded = DungeonMap::load(path);
    assert(loaded.room_count() == map.room_count());
    assert(loaded.is_connected());
    assert(loaded.boss_room_id() == map.boss_room_id());
}

int main() {
    test_connectivity_and_boss();
    test_save_and_load_roundtrip();
    std::cout << "[TEST PASS] Phase1 minimal tests passed.\n";
    return 0;
}
