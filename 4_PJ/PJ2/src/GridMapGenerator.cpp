#include "../include/GridMapGenerator.h"
#include "../include/Room.h"
#include "../include/Item.h"

static Item random_item_simple(std::mt19937& rng) {
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

DungeonMap GridMapGenerator::generate(const GridConfig& cfg) {
    std::mt19937 rng(cfg.seed);
    DungeonMap map;

    auto id_at = [&](int x, int y) { return y * cfg.width + x; };

    // Create rooms with coords and random monsters/items
    for (int y = 0; y < cfg.height; ++y) {
        for (int x = 0; x < cfg.width; ++x) {
            Room r;
            r.id = id_at(x, y);
            r.x = x; r.y = y;
            std::uniform_int_distribution<int> mdist(cfg.monster_min, cfg.monster_max);
            r.monster_power = mdist(rng);
            std::uniform_int_distribution<int> cdist(0, cfg.max_items_per_room);
            int cnt = cdist(rng);
            for (int k = 0; k < cnt; ++k) r.chest_items.push_back(random_item_simple(rng));
            map.add_room(r);
        }
    }

    // 4-neighbor edges (up, down, left, right)
    for (int y = 0; y < cfg.height; ++y) {
        for (int x = 0; x < cfg.width; ++x) {
            int u = id_at(x, y);
            if (x + 1 < cfg.width) map.add_edge(u, id_at(x + 1, y));
            if (y + 1 < cfg.height) map.add_edge(u, id_at(x, y + 1));
        }
    }

    // Boss = max monster
    int boss_id = 0;
    int max_power = -1;
    for (int y = 0; y < cfg.height; ++y) {
        for (int x = 0; x < cfg.width; ++x) {
            int id = id_at(x, y);
            const Room* r = map.get_room(id);
            if (r && r->monster_power > max_power) { max_power = r->monster_power; boss_id = id; }
        }
    }
    map.set_boss(boss_id);

    return map;
}
