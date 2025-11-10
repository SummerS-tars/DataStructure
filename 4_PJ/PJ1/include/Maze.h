#pragma once

#include <string>
#include <vector>

class Maze {
private:
    std::vector<std::string> map_data_;
    int width_;
    int height_;
    int start_x_, start_y_;
    int end_x_, end_y_;

public:
    Maze();
    
    // Load maze from file
    bool load(const std::string& filename);
    
    // Check if position is a wall or out of bounds
    bool is_wall(int x, int y) const;
    
    // Get start and end positions
    void get_start_pos(int& x, int& y) const;
    void get_end_pos(int& x, int& y) const;
    
    // Display maze with player position (single player)
    void display(int player_x, int player_y) const;
    
    // Display maze with multiple players (multiplayer mode)
    void display_multiplayer(int p1_x, int p1_y, char p1_symbol, 
                            int p2_x, int p2_y, char p2_symbol) const;
    
    // Getters
    int width() const { return width_; }
    int height() const { return height_; }
};
