#pragma once
#include <string>
#include <vector>
#include <iostream>

struct LogEntry {
    size_t id = 0;
    std::string timestamp; // 简单字符串
    int start_room = -1;
    int end_room = -1;
    std::vector<int> path; // 完整路径
    int final_power = 0;
    int total_value = 0; // 获得战利品价值总量
};

class AdventureLog {
public:
    void append(const LogEntry& e) {
        LogEntry copy = e; copy.id = ++next_id_; entries_.push_back(std::move(copy));
    }
    const std::vector<LogEntry>& entries() const { return entries_; }

    void print(std::ostream& os) const {
        for (const auto& e : entries_) {
            os << "#" << e.id << " time=" << e.timestamp
               << " start=" << e.start_room << " end=" << e.end_room
               << " len=" << e.path.size()
               << " power=" << e.final_power
               << " value=" << e.total_value << "\n";
        }
    }
private:
    size_t next_id_ = 0;
    std::vector<LogEntry> entries_;
};
