#pragma once
#include "DungeonMap.h"
#include <random>

class BossManager {
public:
    // 邻接随机移动 Boss 一步；返回新 Boss 房间 ID（失败返回 -1）
    static int move_boss_once(DungeonMap& map, std::mt19937& rng) {
        int cur = map.boss_room_id();
        if (cur == -1) return -1;
        const auto& nb = map.neighbors(cur);
        if (nb.empty()) return -1;
        std::uniform_int_distribution<size_t> dist(0, nb.size()-1);
        int nxt = nb[dist(rng)];
        // 维持“Boss=全图最大怪力”的不变量
        int maxp = 0;
        for (int id : map.room_ids()) {
            const Room* r = map.get_room(id);
            if (r) maxp = std::max(maxp, r->monster_power);
        }
        // old: 降级为普通怪（最多 maxp-1）
        if (auto* oldr = map.get_room_mut(cur)) {
            if (oldr->monster_power >= maxp) oldr->monster_power = std::max(0, maxp - 5);
            oldr->is_boss = false;
        }
        // new: 提升为 Boss，怪力设为 maxp（或 maxp+1 以保持唯一最大）
        if (auto* newr = map.get_room_mut(nxt)) {
            newr->monster_power = maxp + 1;
        }
        map.set_boss(nxt);
        return nxt;
    }
};
