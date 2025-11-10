#pragma once

#include <stack>
#include <vector>
#include <utility>
#include <string>

class Player {
private:
    int x_, y_;
    int player_id_;  // 1 or 2 for multiplayer
    char symbol_;    // Display symbol ('@' for P1, '$' for P2)
    std::string name_;
    std::stack<std::pair<int, int>> move_history_;
    std::vector<char> command_history_;

public:
    Player(int x, int y, int player_id = 1, char symbol = '@', const std::string& name = "Player");
    
    // Move player to new position and record in history
    void move_to(int x, int y, char command);
    
    // Undo last move
    bool undo();
    
    // Get current position
    int x() const { return x_; }
    int y() const { return y_; }
    
    // Get move count
    int move_count() const { return static_cast<int>(move_history_.size()); }
    
    // Get move history for path printing
    std::vector<std::pair<int, int>> get_path() const;
    
    // Get command history for replay
    const std::vector<char>& get_commands() const { return command_history_; }
    
    // Multiplayer support
    int get_id() const { return player_id_; }
    char get_symbol() const { return symbol_; }
    const std::string& get_name() const { return name_; }
};
