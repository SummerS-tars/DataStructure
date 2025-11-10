#pragma once

#include <string>
#include <vector>
#include <stack>
#include <utility>

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
    
    // Save to file
    bool save(const std::string& filename) const;
    
    // Load from file
    bool load(const std::string& filename);
};
