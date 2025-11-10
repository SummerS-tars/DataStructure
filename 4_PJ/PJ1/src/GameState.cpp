#include "GameState.h"
#include <fstream>
#include <iostream>

bool GameState::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot create save file " << filename << std::endl;
        return false;
    }
    
    // Write player position
    file.write(reinterpret_cast<const char*>(&player_x), sizeof(player_x));
    file.write(reinterpret_cast<const char*>(&player_y), sizeof(player_y));
    
    // Write maze file name length and content
    size_t maze_file_len = maze_file.length();
    file.write(reinterpret_cast<const char*>(&maze_file_len), sizeof(maze_file_len));
    file.write(maze_file.c_str(), maze_file_len);
    
    // Write move history size and content
    size_t history_size = move_history.size();
    file.write(reinterpret_cast<const char*>(&history_size), sizeof(history_size));
    for (const auto& pos : move_history) {
        file.write(reinterpret_cast<const char*>(&pos.first), sizeof(pos.first));
        file.write(reinterpret_cast<const char*>(&pos.second), sizeof(pos.second));
    }
    
    // Write command history size and content
    size_t cmd_size = command_history.size();
    file.write(reinterpret_cast<const char*>(&cmd_size), sizeof(cmd_size));
    file.write(reinterpret_cast<const char*>(command_history.data()), cmd_size);
    
    file.close();
    return true;
}

bool GameState::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open save file " << filename << std::endl;
        return false;
    }
    
    // Read player position
    file.read(reinterpret_cast<char*>(&player_x), sizeof(player_x));
    file.read(reinterpret_cast<char*>(&player_y), sizeof(player_y));
    
    // Read maze file name
    size_t maze_file_len;
    file.read(reinterpret_cast<char*>(&maze_file_len), sizeof(maze_file_len));
    maze_file.resize(maze_file_len);
    file.read(&maze_file[0], maze_file_len);
    
    // Read move history
    size_t history_size;
    file.read(reinterpret_cast<char*>(&history_size), sizeof(history_size));
    move_history.clear();
    for (size_t i = 0; i < history_size; i++) {
        std::pair<int, int> pos;
        file.read(reinterpret_cast<char*>(&pos.first), sizeof(pos.first));
        file.read(reinterpret_cast<char*>(&pos.second), sizeof(pos.second));
        move_history.push_back(pos);
    }
    
    // Read command history
    size_t cmd_size;
    file.read(reinterpret_cast<char*>(&cmd_size), sizeof(cmd_size));
    command_history.resize(cmd_size);
    file.read(reinterpret_cast<char*>(command_history.data()), cmd_size);
    
    file.close();
    return true;
}
