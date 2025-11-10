#include "Game.h"
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <limits>
#include <string>
#include <sstream>

Game::Game() : player_(0, 0), is_running_(false) {}

bool Game::init(const std::string& maze_file) {
    // Load maze
    if (!maze_.load(maze_file)) {
        return false;
    }
    
    // Initialize player at start position
    int start_x, start_y;
    maze_.get_start_pos(start_x, start_y);
    player_ = Player(start_x, start_y);
    
    is_running_ = true;
    return true;
}

void Game::clear_screen() const {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Game::print_interface() const {
    clear_screen();
    
    std::cout << "===================================\n";
    std::cout << "    Maze Path Analysis System\n";
    std::cout << "===================================\n";
    std::cout << "Position: (" << player_.x() << ", " << player_.y() << ")  ";
    std::cout << "Steps: " << (player_.move_count() - 1) << "\n";
    std::cout << "===================================\n\n";
    
    // Display maze with player position
    maze_.display(player_.x(), player_.y());
    
    std::cout << "\nCommands:\n";
    std::cout << "  W/A/S/D - Move (Up/Left/Down/Right)\n";
    std::cout << "  P - Print path\n";
    std::cout << "  U - Undo\n";
    std::cout << "  Q - Quit\n";
    std::cout << "===================================\n";
}

bool Game::check_win() const {
    int end_x, end_y;
    maze_.get_end_pos(end_x, end_y);
    return (player_.x() == end_x && player_.y() == end_y);
}

bool Game::process_command(char cmd) {
    cmd = toupper(cmd);
    
    int new_x = player_.x();
    int new_y = player_.y();
    
    // Calculate new position based on command
    switch (cmd) {
        case 'W': new_y--; break;  // Up
        case 'S': new_y++; break;  // Down
        case 'A': new_x--; break;  // Left
        case 'D': new_x++; break;  // Right
        case 'P': {
            // Print path
            std::cout << "\n=== Path from Start to Current ===\n";
            auto path = player_.get_path();
            for (size_t i = 0; i < path.size(); i++) {
                std::cout << "(" << path[i].first << ", " << path[i].second << ")";
                if (i < path.size() - 1) {
                    std::cout << " -> ";
                    if ((i + 1) % 5 == 0) std::cout << "\n";
                }
            }
            std::cout << "\n=================================\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            return true;
        }
        case 'U': {
            // Undo
            if (player_.undo()) {
                std::cout << "Undo successful!\n";
            } else {
                std::cout << "Cannot undo - already at start position!\n";
            }
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            return true;
        }
        case 'Q': {
            is_running_ = false;
            return true;
        }
        default:
            std::cout << "Invalid command '" << cmd << "'!\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            return true;
    }
    
    // Check if move is valid (not a wall)
    if (!maze_.is_wall(new_x, new_y)) {
        player_.move_to(new_x, new_y, cmd);
        return true;  // Move successful, continue processing
    } else {
        std::cout << "\n❌ ERROR: Cannot move " << cmd << " - Wall or out of bounds!\n";
        std::cout << "Movement stopped. Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return false;  // Move failed, stop processing remaining commands
    }
}

void Game::run() {
    while (is_running_) {
        print_interface();
        
        // Check win condition
        if (check_win()) {
            std::cout << "\n🎉 CONGRATULATIONS! 🎉\n";
            std::cout << "You reached the exit!\n";
            std::cout << "Total steps: " << (player_.move_count() - 1) << "\n\n";
            
            std::cout << "Your path:\n";
            auto path = player_.get_path();
            for (size_t i = 0; i < path.size(); i++) {
                std::cout << "(" << path[i].first << ", " << path[i].second << ")";
                if (i < path.size() - 1) {
                    std::cout << " -> ";
                    if ((i + 1) % 5 == 0) std::cout << "\n";
                }
            }
            std::cout << "\n\n";
            break;
        }
        
        std::cout << "Enter command(s): ";
        std::string input;
        std::getline(std::cin, input);
        
        // Process each character in the input
        for (size_t i = 0; i < input.length(); i++) {
            char cmd = input[i];
            
            // Skip whitespace
            if (cmd == ' ' || cmd == '\t') {
                continue;
            }
            
            // Process command and check if it succeeded
            bool success = process_command(cmd);
            
            // If command failed (hit wall), stop processing remaining commands
            if (!success) {
                std::cout << "Remaining commands ignored due to collision.\n";
                break;
            }
            
            // If we need to quit, break
            if (!is_running_) {
                break;
            }
            
            // Check for win after each move
            if (check_win()) {
                print_interface();
                std::cout << "\n🎉 CONGRATULATIONS! 🎉\n";
                std::cout << "You reached the exit!\n";
                std::cout << "Total steps: " << (player_.move_count() - 1) << "\n\n";
                
                std::cout << "Your path:\n";
                auto path = player_.get_path();
                for (size_t j = 0; j < path.size(); j++) {
                    std::cout << "(" << path[j].first << ", " << path[j].second << ")";
                    if (j < path.size() - 1) {
                        std::cout << " -> ";
                        if ((j + 1) % 5 == 0) std::cout << "\n";
                    }
                }
                std::cout << "\n\n";
                is_running_ = false;
                break;
            }
        }
    }
    
    std::cout << "Thanks for playing!\n";
}
