#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Room.h"

class DungeonMap {
public:
    DungeonMap() = default;

    // 创建房间与边
    void add_room(const Room& room);
    void add_edge(int u, int v); // 无向边

    // 查询接口
    const Room* get_room(int id) const;
    Room* get_room_mut(int id);
    const std::vector<int>& neighbors(int id) const;

    // Boss 辅助
    int boss_room_id() const { return boss_id_; }
    void set_boss(int id);

    // 连通性检查
    bool is_connected() const;

    // 调试打印
    std::string to_string() const;
    std::string to_json() const;

    // 元信息
    int room_count() const { return (int)rooms_.size(); }
    std::vector<int> room_ids() const;
    const std::unordered_map<int, Room>& rooms() const { return rooms_; }

    // 序列化/反序列化
    bool save(const std::string& path) const;
    static DungeonMap load(const std::string& path);

private:
    std::unordered_map<int, Room> rooms_;
    std::unordered_map<int, std::vector<int>> adj_; // 邻接表
    int boss_id_ = -1;
};
