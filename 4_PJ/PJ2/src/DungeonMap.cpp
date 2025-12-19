#include "../include/DungeonMap.h"
#include "../include/Room.h"
#include "../include/Item.h"
#include <queue>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iomanip>

// Helper to escape JSON strings (minimal)
static std::string json_escape(const std::string& s) {
    std::ostringstream os;
    for (char c : s) {
        switch (c) {
            case '\\': os << "\\\\"; break;
            case '"': os << "\\\""; break;
            case '\n': os << "\\n"; break;
            case '\r': os << "\\r"; break;
            case '\t': os << "\\t"; break;
            default: os << c; break;
        }
    }
    return os.str();
}

void DungeonMap::add_room(const Room& room) {
    rooms_[room.id] = room;
    if (!adj_.count(room.id)) adj_[room.id] = {};
}

void DungeonMap::add_edge(int u, int v) {
    if (u == v) return;
    adj_[u].push_back(v);
    adj_[v].push_back(u);
    // 去重
    auto dedup = [](std::vector<int>& xs) {
        std::sort(xs.begin(), xs.end());
        xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
    };
    dedup(adj_[u]);
    dedup(adj_[v]);
}

const Room* DungeonMap::get_room(int id) const {
    auto it = rooms_.find(id);
    if (it == rooms_.end()) return nullptr;
    return &it->second;
}

Room* DungeonMap::get_room_mut(int id) {
    auto it = rooms_.find(id);
    if (it == rooms_.end()) return nullptr;
    return &it->second;
}

const std::vector<int>& DungeonMap::neighbors(int id) const {
    static const std::vector<int> empty;
    auto it = adj_.find(id);
    if (it == adj_.end()) return empty;
    return it->second;
}

void DungeonMap::set_boss(int id) {
    if (boss_id_ != -1) {
        if (auto* prev = get_room_mut(boss_id_)) prev->is_boss = false;
    }
    boss_id_ = id;
    if (auto* r = get_room_mut(id)) r->is_boss = true;
}

bool DungeonMap::is_connected() const {
    if (rooms_.empty()) return true;
    std::unordered_map<int, bool> vis;
    for (auto& kv : rooms_) vis[kv.first] = false;

    int start = rooms_.begin()->first;
    std::queue<int> q;
    q.push(start);
    vis[start] = true;
    int cnt = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        cnt++;
        auto it = adj_.find(u);
        if (it == adj_.end()) continue;
        for (int v : it->second) {
            if (!vis[v]) { vis[v] = true; q.push(v); }
        }
    }
    return cnt == (int)rooms_.size();
}

std::string DungeonMap::to_string() const {
    std::ostringstream os;
    os << "Rooms: " << rooms_.size() << "\n";
    for (const auto& kv : rooms_) {
        const auto& r = kv.second;
        os << "Room " << r.id << " [monster=" << r.monster_power
           << (r.is_boss ? ", BOSS" : "") << "] chest_items=" << r.chest_items.size() << "\n";
        os << "  ->";
        auto it = adj_.find(r.id);
        if (it != adj_.end()) {
            for (int v : it->second) os << " " << v;
        }
        os << "\n";
    }
    os << "BossRoom: " << boss_id_ << "\n";
    return os.str();
}

static std::string room_type_to_str(RoomType t) {
    switch (t) {
        case RoomType::START: return "START";
        case RoomType::NORMAL: return "NORMAL";
        case RoomType::BOSS: return "BOSS";
        case RoomType::TREASURE: return "TREASURE";
    }
    return "NORMAL";
}

std::string DungeonMap::to_json() const {
    std::ostringstream os;
    os << "{\n  \"rooms\": [\n";
    auto ids = room_ids();
    for (size_t i = 0; i < ids.size(); ++i) {
        const Room& r = rooms_.at(ids[i]);
        os << "    {\"id\": " << r.id
           << ", \"type\": \"" << room_type_to_str(r.type) << "\""
           << ", \"tier\": " << r.tier
           << ", \"power\": " << r.monster_power
           << ", \"label\": \"" << json_escape(r.label) << "\""
           << ", \"items\": [";
        for (size_t k = 0; k < r.chest_items.size(); ++k) {
            const auto& it = r.chest_items[k];
            os << "{";
            os << "\"name\": \"" << json_escape(it.name) << "\"";
            os << ", \"type\": " << item_type_to_int(it.type);
            os << ", \"val\": " << it.value;
            os << ", \"power\": " << it.power_bonus;
            os << "}";
            if (k + 1 < r.chest_items.size()) os << ",";
        }
        os << "]";
        os << "}";
        if (i + 1 < ids.size()) os << ",";
        os << "\n";
    }
    os << "  ],\n  \"edges\": [\n";
    bool first_edge = true;
    for (const auto& kv : adj_) {
        int u = kv.first;
        for (int v : kv.second) if (u < v) {
            if (!first_edge) os << ",\n"; else first_edge = false;
            os << "    {\"source\": " << u << ", \"target\": " << v << "}";
        }
    }
    os << "\n  ],\n  \"bossId\": " << boss_id_
       << ",\n  \"connected\": " << (is_connected() ? "true" : "false")
       << "\n}";
    return os.str();
}

std::vector<int> DungeonMap::room_ids() const {
    std::vector<int> ids;
    ids.reserve(rooms_.size());
    for (const auto& kv : rooms_) ids.push_back(kv.first);
    std::sort(ids.begin(), ids.end());
    return ids;
}

bool DungeonMap::save(const std::string& path) const {
    std::ofstream ofs(path);
    if (!ofs) return false;
    auto ids = room_ids();
    ofs << rooms_.size() << "\n";
    for (int id : ids) {
        const Room& r = rooms_.at(id);
        ofs << r.id << ' ' << r.monster_power << ' ' << (r.is_boss ? 1 : 0) << ' ' << r.chest_items.size();
        for (const auto& it : r.chest_items) {
            ofs << ' ' << item_type_to_int(it.type) << ' ' << it.value << ' ' << it.power_bonus << ' ' << it.name;
        }
        ofs << "\n";
    }
    // edges
    size_t m = 0;
    for (const auto& kv : adj_) m += kv.second.size();
    // each undirected edge counted twice; dedup by only u<v
    std::vector<std::pair<int,int>> edges;
    for (const auto& kv : adj_) {
        int u = kv.first;
        for (int v : kv.second) if (u < v) edges.emplace_back(u, v);
    }
    ofs << edges.size() << "\n";
    for (auto [u, v] : edges) ofs << u << ' ' << v << "\n";
    ofs << boss_id_ << "\n";
    return true;
}

DungeonMap DungeonMap::load(const std::string& path) {
    std::ifstream ifs(path);
    DungeonMap map;
    if (!ifs) return map;
    int n = 0; ifs >> n; 
    for (int i = 0; i < n; ++i) {
        Room r; int is_boss_int = 0; size_t cnt = 0;
        ifs >> r.id >> r.monster_power >> is_boss_int >> cnt;
        r.is_boss = (is_boss_int != 0);
        for (size_t k = 0; k < cnt; ++k) {
            int t, val, pb; std::string name;
            ifs >> t >> val >> pb >> name;
            Item it{ name, item_type_from_int(t), val, pb };
            r.chest_items.push_back(it);
        }
        map.add_room(r);
    }
    size_t m = 0; ifs >> m;
    for (size_t e = 0; e < m; ++e) { int u, v; ifs >> u >> v; map.add_edge(u, v); }
    int boss_id = -1; ifs >> boss_id; if (boss_id != -1) map.set_boss(boss_id);
    return map;
}
