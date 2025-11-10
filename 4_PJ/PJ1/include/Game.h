#pragma once

#include "Maze.h"
#include "Player.h"
#include <string>

class Game {
private:
    Maze maze_;
    Player player_;
    bool is_running_;
    std::string current_maze_file_;
    
    // Process single command, returns false if command failed (hit wall)
    bool process_command(char cmd);
    
    // Check if player reached the end
    bool check_win() const;
    
    // Clear screen (cross-platform)
    void clear_screen() const;
    
    // Print game interface
    void print_interface() const;
    
    // Replay all moves with animation
    void replay_moves();
    
    // Save game state
    bool save_game(const std::string& filename);
    
    // Load game state
    bool load_game(const std::string& filename);

public:
    Game();
    
    // Load maze and initialize game
    bool init(const std::string& maze_file);
    
    // Main game loop
    void run();
};
