#include "../include/MapGenerator.h"
#include "../include/Item.h"
#include "../include/Room.h"
#include <random>
#include <algorithm>
#include <numeric>

static Item random_item(std::mt19937& rng) {
    std::uniform_int_distribution<int> tdist(0, 4);
    std::uniform_int_distribution<int> vdist(1, 50);
    std::uniform_int_distribution<int> pdist(0, 20);
    Item item;
    item.type = static_cast<ItemType>(tdist(rng));
    switch (item.type) {
        case ItemType::Helmet: item.name = "Helmet"; break;
        case ItemType::Armor: item.name = "Armor"; break;
        case ItemType::Boots: item.name = "Boots"; break;
        case ItemType::Weapon: item.name = "Weapon"; break;
        case ItemType::MagicStone: item.name = "MagicStone"; break;
    }
    item.value = vdist(rng);
    item.power_bonus = pdist(rng);
    return item;
}

DungeonMap MapGenerator::generate(const GenConfig& cfg) {
    std::mt19937 rng(cfg.seed);
    DungeonMap map;

    // 创建房间
    for (int i = 0; i < cfg.room_num; ++i) {
        Room r;
        r.id = i;
        std::uniform_int_distribution<int> mdist(cfg.monster_min, cfg.monster_max);
        r.monster_power = mdist(rng);
        // 随机给 0~3 个物品
        std::uniform_int_distribution<int> cdist(0, 3);
        int cnt = cdist(rng);
        for (int k = 0; k < cnt; ++k) r.chest_items.push_back(random_item(rng));
        map.add_room(r);
    }

    // 生成主干树，保证连通：形成随机生成树
    std::vector<int> nodes(cfg.room_num);
    std::iota(nodes.begin(), nodes.end(), 0);
    std::shuffle(nodes.begin(), nodes.end(), rng);
    for (int i = 1; i < cfg.room_num; ++i) {
        int u = nodes[i];
        std::uniform_int_distribution<int> prevdist(0, i - 1);
        int v = nodes[prevdist(rng)];
        map.add_edge(u, v);
    }

    // 额外添加随机边
    std::uniform_int_distribution<int> ndist(0, cfg.room_num - 1);
    for (int e = 0; e < cfg.extra_edges; ++e) {
        int u = ndist(rng), v = ndist(rng);
        if (u != v) map.add_edge(u, v);
    }

    // 设置 Boss：怪物武力值最大者
    int boss_id = 0;
    int max_power = -1;
    for (int i = 0; i < cfg.room_num; ++i) {
        const Room* r = map.get_room(i);
        if (r && r->monster_power > max_power) {
            max_power = r->monster_power;
            boss_id = i;
        }
    }
    map.set_boss(boss_id);

    return map;
}
