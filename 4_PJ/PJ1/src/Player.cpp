#include "Player.h"
#include <algorithm>

Player::Player(int x, int y, int player_id, char symbol, const std::string& name) 
    : x_(x), y_(y), player_id_(player_id), symbol_(symbol), name_(name) {
    // Push initial position to history stack
    move_history_.push({x, y});
}

void Player::move_to(int x, int y, char command) {
    x_ = x;
    y_ = y;
    move_history_.push({x, y});
    command_history_.push_back(command);
}

bool Player::undo() {
    // Can't undo if only at start position
    if (move_history_.size() <= 1) {
        return false;
    }
    
    // Pop current position
    move_history_.pop();
    
    // Get previous position
    auto prev_pos = move_history_.top();
    x_ = prev_pos.first;
    y_ = prev_pos.second;
    
    // Remove last command from history
    if (!command_history_.empty()) {
        command_history_.pop_back();
    }
    
    return true;
}

std::vector<std::pair<int, int>> Player::get_path() const {
    std::vector<std::pair<int, int>> path;
    std::stack<std::pair<int, int>> temp_stack = move_history_;
    
    // Extract all positions from stack (they'll be in reverse order)
    while (!temp_stack.empty()) {
        path.push_back(temp_stack.top());
        temp_stack.pop();
    }
    
    // Reverse to get path from start to current
    std::reverse(path.begin(), path.end());
    
    return path;
}
