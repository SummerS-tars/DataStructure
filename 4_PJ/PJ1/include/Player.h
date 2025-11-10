#pragma once

#include <stack>
#include <vector>
#include <utility>

class Player {
private:
    int x_, y_;
    std::stack<std::pair<int, int>> move_history_;
    std::vector<char> command_history_;

public:
    Player(int x, int y);
    
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
};
