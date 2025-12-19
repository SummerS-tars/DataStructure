#include "../include/MapGenerator.h"
#include "../include/Item.h"
#include "../include/Room.h"
#include <random>
#include <algorithm>
#include <numeric>
#include <queue>

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

static void assign_tiers(DungeonMap& map, int start_id) {
    std::queue<int> q;
    std::unordered_map<int, bool> vis;
    q.push(start_id); vis[start_id] = true;
    map.get_room_mut(start_id)->tier = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        const auto* ru = map.get_room(u);
        int cur_tier = ru ? ru->tier : 0;
        for (int v : map.neighbors(u)) {
            if (!vis[v]) {
                vis[v] = true;
                if (auto* rv = map.get_room_mut(v)) rv->tier = cur_tier + 1;
                q.push(v);
            }
        }
    }
}

DungeonMap MapGenerator::generate(const GenConfig& cfg) {
    std::mt19937 rng(cfg.seed);
    DungeonMap map;

    // --- Step 1: create rooms with base data ---
    for (int i = 0; i < cfg.room_num; ++i) {
        Room r;
        r.id = i;
        std::uniform_int_distribution<int> mdist(cfg.monster_min, cfg.monster_max);
        r.monster_power = mdist(rng);
        std::uniform_int_distribution<int> cdist(0, 3);
        int cnt = cdist(rng);
        for (int k = 0; k < cnt; ++k) r.chest_items.push_back(random_item(rng));
        r.type = RoomType::NORMAL;
        map.add_room(r);
    }

    // --- Step 2: pick backbone nodes (60% approx) ---
    int backbone_len = std::max(2, cfg.room_num * 60 / 100); // ensure start+boss at least
    if (backbone_len > cfg.room_num) backbone_len = cfg.room_num;
    std::vector<int> ids(cfg.room_num); std::iota(ids.begin(), ids.end(), 0);
    std::shuffle(ids.begin(), ids.end(), rng);
    std::vector<int> backbone(ids.begin(), ids.begin() + backbone_len);

    // Ensure start (0) is in backbone; swap if missing
    auto it0 = std::find(backbone.begin(), backbone.end(), 0);
    if (it0 == backbone.end()) backbone[0] = 0;

    // Choose boss as last backbone node (highest tier expected); adjust later for max monster
    // Build backbone chain sequentially
    for (size_t i = 1; i < backbone.size(); ++i) {
        map.add_edge(backbone[i-1], backbone[i]);
    }

    // --- Step 3: attach branches with remaining nodes ---
    std::vector<int> remaining(ids.begin() + backbone_len, ids.end());
    std::uniform_int_distribution<size_t> parent_dist(0, backbone.size() - 1);
    for (int node : remaining) {
        // pick any existing node as parent (prefer backbone for stability)
        size_t idx = parent_dist(rng);
        int parent = backbone[idx];
        map.add_edge(parent, node);
    }

    // --- Step 4: controlled extra edges (local loops) ---
    int extra = std::max(0, cfg.extra_edges);
    std::uniform_int_distribution<int> ndist(0, cfg.room_num - 1);
    for (int e = 0; e < extra; ++e) {
        int u = ndist(rng), v = ndist(rng);
        if (u == v) { --e; continue; }
        // We will only add if tiers are not too far after we compute tiers; for now tentatively add, then prune if needed
        map.add_edge(u, v);
    }

    // --- Step 5: assign tiers via BFS from start (0) ---
    assign_tiers(map, 0);

    // Now prune extra edges that violate tier gap > 2
    // Rebuild adjacency with constraint
    std::vector<std::pair<int,int>> edges;
    for (int u : map.room_ids()) {
        for (int v : map.neighbors(u)) {
            if (u < v) edges.emplace_back(u, v);
        }
    }
    // Clear adj by recreating map edges respecting constraint
    DungeonMap newmap;
    for (int id : map.room_ids()) newmap.add_room(*map.get_room(id));
    for (auto [u,v] : edges) {
        const Room* ru = map.get_room(u);
        const Room* rv = map.get_room(v);
        if (!ru || !rv) continue;
        if (ru->tier >=0 && rv->tier >=0 && std::abs(ru->tier - rv->tier) <= 2) {
            newmap.add_edge(u, v);
        }
    }
    map = std::move(newmap);

    // Re-run tiers after pruning
    assign_tiers(map, 0);

    // --- Step 6: assign room types ---
    if (auto* start = map.get_room_mut(0)) { start->type = RoomType::START; start->label = "Entrance"; start->monster_power = 0; }

    // Boss = max monster; ensure boss on backbone tail if possible
    int boss_id = backbone.empty() ? 0 : backbone.back();
    int max_power = -1;
    for (int id : map.room_ids()) {
        const Room* r = map.get_room(id);
        if (!r) continue;
        if (r->monster_power > max_power) { max_power = r->monster_power; boss_id = id; }
    }
    map.set_boss(boss_id);
    if (auto* boss = map.get_room_mut(boss_id)) {
        boss->type = RoomType::BOSS;
        boss->label = "Boss";
    }

    // Mark treasure rooms with higher probability on leaves (degree 1, not start/boss)
    for (int id : map.room_ids()) {
        Room* r = map.get_room_mut(id);
        if (!r || id == 0 || id == boss_id) continue;
        const auto& nb = map.neighbors(id);
        bool is_leaf = (nb.size() == 1);
        std::bernoulli_distribution treasure_prob(is_leaf ? 0.6 : 0.2);
        if (treasure_prob(rng)) {
            r->type = RoomType::TREASURE;
            if (r->label.empty()) r->label = "Treasure";
        }
    }

    return map;
}
