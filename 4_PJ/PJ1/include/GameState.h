#pragma once

#include <string>
#include <vector>
#include <utility>
#include <ctime>

struct GameState {
    // Player position
    int player_x;
    int player_y;
    
    // Move history (stored as vector for serialization)
    std::vector<std::pair<int, int>> move_history;
    
    // Command history
    std::vector<char> command_history;
    
    // Current maze file
    std::string maze_file;
    
    // Metadata
    time_t timestamp;
    int step_count;
    
    // Save to file
    bool save(const std::string& filename) const;
    
    // Load from file
    bool load(const std::string& filename);
    
    // Get formatted timestamp string
    std::string get_timestamp_str() const;
    
    // Get short maze name (without path)
    std::string get_maze_name() const;
};
