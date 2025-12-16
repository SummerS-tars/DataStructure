#pragma once
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "LootTree.h"
#include "AdventureLog.h"
#include "DungeonMap.h"

class Player {
public:
    explicit Player(int base_power = 10) : power_(base_power) {}

    int power() const { return power_; }
    int total_value() const { return total_value_; }
    const LootTree& loot() const { return loot_; }
    LootTree& loot_mut() { return loot_; }
    const AdventureLog& log() const { return log_; }

    // 从房间收集宝箱物品，并清空房间宝箱
    void collect_room_loot(DungeonMap& map, int room_id) {
        Room* r = map.get_room_mut(room_id);
        if (!r) return;
        for (const auto& it : r->chest_items) {
            loot_.add_item(it);
            power_ += it.power_bonus;
            total_value_ += it.value;
        }
        r->chest_items.clear();
    }

    // 按给定路径探索（先不处理战斗成功与否），自动收集战利品并记录日志
    void explore_path(DungeonMap& map, const std::vector<int>& path) {
        if (path.empty()) return;
        for (int id : path) collect_room_loot(map, id);
        LogEntry e;
        e.timestamp = now_string();
        e.start_room = path.front();
        e.end_room = path.back();
        e.path = path;
        e.final_power = power_;
        e.total_value = total_value_;
        log_.append(e);
    }

private:
    int power_ = 10;
    int total_value_ = 0;
    LootTree loot_;
    AdventureLog log_;

    static std::string now_string() {
        std::time_t t = std::time(nullptr);
        std::tm tm{}; 
        localtime_r(&t, &tm);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};
