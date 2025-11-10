#include "Maze.h"
#include <fstream>
#include <iostream>

Maze::Maze() : width_(0), height_(0), start_x_(-1), start_y_(-1), end_x_(-1), end_y_(-1) {}

bool Maze::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }
    
    map_data_.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            map_data_.push_back(line);
        }
    }
    
    file.close();
    
    if (map_data_.empty()) {
        std::cerr << "Error: Empty maze file" << std::endl;
        return false;
    }
    
    height_ = static_cast<int>(map_data_.size());
    width_ = static_cast<int>(map_data_[0].length());
    
    // Find start 'S' and end 'E' positions
    // Note: Using byte positions (size_t) to handle UTF-8 characters properly
    for (int y = 0; y < height_; y++) {
        for (size_t x = 0; x < map_data_[y].length(); x++) {
            if (map_data_[y][x] == 'S') {
                start_x_ = static_cast<int>(x);
                start_y_ = y;
            } else if (map_data_[y][x] == 'E') {
                end_x_ = static_cast<int>(x);
                end_y_ = y;
            }
        }
    }
    
    if (start_x_ == -1 || start_y_ == -1) {
        std::cerr << "Error: Start position 'S' not found in maze" << std::endl;
        return false;
    }
    
    if (end_x_ == -1 || end_y_ == -1) {
        std::cerr << "Error: End position 'E' not found in maze" << std::endl;
        return false;
    }
    
    return true;
}

bool Maze::is_wall(int x, int y) const {
    // Check bounds
    if (x < 0 || y < 0 || y >= height_ || x >= width_) {
        return true;
    }
    
    if (x >= static_cast<int>(map_data_[y].length())) {
        return true;
    }
    
    // '#' is wall
    return map_data_[y][x] == '#';
}

void Maze::get_start_pos(int& x, int& y) const {
    x = start_x_;
    y = start_y_;
}

void Maze::get_end_pos(int& x, int& y) const {
    x = end_x_;
    y = end_y_;
}

void Maze::display(int player_x, int player_y) const {
    std::cout << "===================================\n";
    std::cout << "Maze Display (@ = Player Position)\n";
    std::cout << "===================================\n";
    
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < static_cast<int>(map_data_[y].length()); x++) {
            if (x == player_x && y == player_y) {
                std::cout << '@';  // Player position
            } else {
                std::cout << map_data_[y][x];
            }
        }
        std::cout << '\n';
    }
    std::cout << "===================================\n";
}
