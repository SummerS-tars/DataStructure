#pragma once

#include "Maze.h"
#include "Player.h"
#include "SaveManager.h"
#include "MazeGenerator.h"
#include <string>

class MenuManager;  // Forward declaration

class Game {
    friend class MenuManager;  // Allow MenuManager to access private members
    
private:
    Maze maze_;
    Player player_;
    SaveManager save_manager_;
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
    
    // Handle save command with slot number
    void handle_save_command(int slot);
    
    // Handle load command with slot number
    void handle_load_command(int slot);

public:
    Game();
    
    // Load maze and initialize game
    bool init(const std::string& maze_file);
    
    // Main game loop
    void run();
};
