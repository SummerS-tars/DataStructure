#pragma once

#include "GameState.h"
#include "Player.h"
#include <string>

class SaveManager {
private:
    static const int MAX_SLOTS = 5;
    std::string save_directory_;
    
    // Get save filename for a slot
    std::string get_save_filename(int slot) const;
    
public:
    SaveManager();
    
    // Ensure save directory exists
    void ensure_directory() const;
    
    // Check if save file exists
    bool save_exists(int slot) const;
    
    // Save game state to specific slot (1-5)
    bool save_game(int slot, const Player& player, const std::string& maze_file);
    
    // Load game state from specific slot (1-5)
    bool load_game(int slot, GameState& state);
    
    // List all available save files
    void list_saves() const;
    
    // Get save file info without loading
    bool get_save_info(int slot, GameState& state) const;
    
    // Validate slot number
    bool is_valid_slot(int slot) const;
};
