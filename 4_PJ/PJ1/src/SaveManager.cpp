#include "SaveManager.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

SaveManager::SaveManager() : save_directory_("saves") {}

std::string SaveManager::get_save_filename(int slot) const {
    return save_directory_ + "/game" + std::to_string(slot) + ".save";
}

void SaveManager::ensure_directory() const {
    #ifdef _WIN32
        _mkdir(save_directory_.c_str());
    #else
        mkdir(save_directory_.c_str(), 0755);
    #endif
}

bool SaveManager::is_valid_slot(int slot) const {
    return slot >= 1 && slot <= MAX_SLOTS;
}

bool SaveManager::save_exists(int slot) const {
    if (!is_valid_slot(slot)) return false;
    
    std::ifstream file(get_save_filename(slot));
    return file.good();
}

bool SaveManager::save_game(int slot, const Player& player, const std::string& maze_file) {
    if (!is_valid_slot(slot)) {
        std::cerr << "Error: Invalid save slot " << slot << " (must be 1-" << MAX_SLOTS << ")" << std::endl;
        return false;
    }
    
    ensure_directory();
    
    GameState state;
    state.player_x = player.x();
    state.player_y = player.y();
    state.maze_file = maze_file;
    state.move_history = player.get_path();
    state.command_history = player.get_commands();
    state.timestamp = time(nullptr);
    state.step_count = player.move_count() - 1;
    
    std::string filename = get_save_filename(slot);
    if (state.save(filename)) {
        std::cout << "\n╔════════════════════════════════════╗\n";
        std::cout << "║   ✓ Game Saved to Slot " << slot << "          ║\n";
        std::cout << "╠════════════════════════════════════╣\n";
        std::cout << "║ Maze:  " << std::left << std::setw(26) << state.get_maze_name() << "║\n";
        std::cout << "║ Position: (" << std::setw(2) << state.player_x << ", " << std::setw(2) << state.player_y << ")             ║\n";
        std::cout << "║ Steps: " << std::setw(26) << state.step_count << "║\n";
        std::cout << "║ Time:  " << std::setw(26) << state.get_timestamp_str().substr(11) << "║\n";
        std::cout << "╚════════════════════════════════════╝\n";
        return true;
    }
    
    return false;
}

bool SaveManager::load_game(int slot, GameState& state) {
    if (!is_valid_slot(slot)) {
        std::cerr << "Error: Invalid save slot " << slot << " (must be 1-" << MAX_SLOTS << ")" << std::endl;
        return false;
    }
    
    if (!save_exists(slot)) {
        std::cerr << "Error: No save file found in slot " << slot << std::endl;
        return false;
    }
    
    std::string filename = get_save_filename(slot);
    return state.load(filename);
}

bool SaveManager::get_save_info(int slot, GameState& state) const {
    if (!is_valid_slot(slot) || !save_exists(slot)) {
        return false;
    }
    
    std::string filename = get_save_filename(slot);
    return state.load(filename);
}

void SaveManager::list_saves() const {
    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║               Available Save Files                         ║\n";
    std::cout << "╠══════╦═════════════╦══════════╦═══════╦══════════════════╣\n";
    std::cout << "║ Slot ║ Maze        ║ Position ║ Steps ║ Saved At         ║\n";
    std::cout << "╠══════╬═════════════╬══════════╬═══════╬══════════════════╣\n";
    
    bool has_saves = false;
    for (int slot = 1; slot <= MAX_SLOTS; slot++) {
        if (save_exists(slot)) {
            GameState state;
            if (get_save_info(slot, state)) {
                has_saves = true;
                std::string maze_name = state.get_maze_name();
                if (maze_name.length() > 11) {
                    maze_name = maze_name.substr(0, 8) + "...";
                }
                
                std::string time_str = state.get_timestamp_str().substr(5, 11); // MM-DD HH:MM
                
                std::cout << "║  " << slot << "   ║ " 
                          << std::left << std::setw(11) << maze_name << " ║ "
                          << "(" << std::setw(2) << std::right << state.player_x << "," 
                          << std::setw(2) << state.player_y << ")  ║ "
                          << std::setw(5) << state.step_count << " ║ "
                          << time_str << "     ║\n";
            }
        }
    }
    
    if (!has_saves) {
        std::cout << "║                    No saves found                          ║\n";
    }
    
    std::cout << "╚══════╩═════════════╩══════════╩═══════╩══════════════════╝\n";
}
