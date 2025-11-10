#include "Game.h"
#include "GameState.h"
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <limits>
#include <string>

Game::Game() : player_(0, 0), is_running_(false), current_maze_file_("") {}

bool Game::init(const std::string& maze_file) {
    // Load maze
    if (!maze_.load(maze_file)) {
        return false;
    }
    
    // Store current maze file
    current_maze_file_ = maze_file;
    
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
    std::cout << "  P - Print path  |  U - Undo  |  R - Replay\n";
    std::cout << "  SAVE - Save game  |  LOAD - Load game\n";
    std::cout << "  HELP - Show help  |  QUIT - Exit\n";
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
        case 'R': {
            // Replay
            replay_moves();
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
        
        // Convert to uppercase for comparison
        std::string upper_input = input;
        for (char& c : upper_input) {
            c = toupper(c);
        }
        
        // Check for special multi-character commands
        if (upper_input.substr(0, 4) == "SAVE") {
            // Parse slot number (default to 1)
            int slot = 1;
            if (upper_input.length() > 5 && isdigit(upper_input[5])) {
                slot = upper_input[5] - '0';
            }
            handle_save_command(slot);
            continue;
        } else if (upper_input.substr(0, 4) == "LOAD") {
            // Parse slot number
            int slot = 0;
            if (upper_input.length() > 5 && isdigit(upper_input[5])) {
                slot = upper_input[5] - '0';
            }
            
            if (slot == 0) {
                // Show list and prompt
                save_manager_.list_saves();
                std::cout << "\nEnter slot number to load (1-5, or 0 to cancel): ";
                std::cin >> slot;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (slot == 0) {
                    std::cout << "Load cancelled.\n";
                    std::cout << "Press Enter to continue...";
                    std::cin.get();
                    continue;
                }
            }
            
            handle_load_command(slot);
            continue;
        } else if (upper_input == "SAVES") {
            save_manager_.list_saves();
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            continue;
        } else if (upper_input == "QUIT" || upper_input == "EXIT") {
            is_running_ = false;
            break;
        } else if (upper_input == "HELP") {
            std::cout << "\n=== Help ===\n";
            std::cout << "W/A/S/D - Move up/left/down/right\n";
            std::cout << "P - Print path\n";
            std::cout << "U - Undo last move\n";
            std::cout << "R - Replay all moves\n";
            std::cout << "SAVE [1-5] - Save game to slot (default: 1)\n";
            std::cout << "LOAD [1-5] - Load game from slot\n";
            std::cout << "SAVES - List all save files\n";
            std::cout << "QUIT - Exit game\n";
            std::cout << "============\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            continue;
        }
        
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

void Game::replay_moves() {
    // IMPORTANT: Make a COPY of commands, not a reference!
    // We need to copy before resetting the player
    std::vector<char> commands = player_.get_commands();
    
    if (commands.empty()) {
        std::cout << "\nNo moves to replay yet!\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }
    
    std::cout << "\n=== Starting Replay ===\n";
    std::cout << "Total commands: " << commands.size() << "\n";
    std::cout << "Press Enter to start replay...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    
    // Save current player state
    Player temp_player = player_;
    
    // Reset player to start
    int start_x, start_y;
    maze_.get_start_pos(start_x, start_y);
    player_ = Player(start_x, start_y);
    
    // Replay each command with animation
    for (size_t i = 0; i < commands.size(); i++) {
        char cmd = commands[i];
        
        // Display current state
        clear_screen();
        std::cout << "=== Replay Mode ===\n";
        std::cout << "Command " << (i + 1) << "/" << commands.size() << ": " << cmd << "\n";
        std::cout << "===================\n\n";
        
        maze_.display(player_.x(), player_.y());
        
        // Execute move
        int new_x = player_.x();
        int new_y = player_.y();
        
        switch (toupper(cmd)) {
            case 'W': new_y--; break;
            case 'S': new_y++; break;
            case 'A': new_x--; break;
            case 'D': new_x++; break;
        }
        
        if (!maze_.is_wall(new_x, new_y)) {
            player_.move_to(new_x, new_y, cmd);
        }
        
        // Delay for animation effect
        #ifdef _WIN32
            system("timeout /t 1 /nobreak > nul");
        #else
            system("sleep 0.5");
        #endif
    }
    
    // Show final state
    clear_screen();
    std::cout << "=== Replay Complete! ===\n\n";
    maze_.display(player_.x(), player_.y());
    std::cout << "\nTotal moves: " << commands.size() << "\n";
    
    // Restore original player state
    player_ = temp_player;
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Game::handle_save_command(int slot) {
    // Validate slot number
    if (!save_manager_.is_valid_slot(slot)) {
        std::cout << "Invalid slot! Use SAVE 1-5\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    // Use SaveManager to save
    if (save_manager_.save_game(slot, player_, current_maze_file_)) {
        // Success message already printed by SaveManager
    } else {
        std::cout << "✗ Failed to save game!\n";
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

void Game::handle_load_command(int slot) {
    // Validate slot number
    if (!save_manager_.is_valid_slot(slot)) {
        std::cout << "Invalid slot! Use LOAD 1-5\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    // Check if save exists
    if (!save_manager_.save_exists(slot)) {
        std::cout << "✗ Save slot " << slot << " is empty!\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    // Load state to preview
    GameState state;
    if (!save_manager_.load_game(slot, state)) {
        std::cout << "✗ Failed to load game!\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    // Show confirmation dialog
    std::cout << "\n========================================\n";
    std::cout << "       Load Save File?\n";
    std::cout << "========================================\n";
    std::cout << "  Slot:     " << slot << "\n";
    std::cout << "  Maze:     " << state.get_maze_name() << "\n";
    std::cout << "  Position: (" << state.player_x << ", " << state.player_y << ")\n";
    std::cout << "  Steps:    " << state.step_count << "\n";
    std::cout << "  Saved:    " << state.get_timestamp_str() << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << "  Warning: Current progress will be\n";
    std::cout << "           lost if not saved!\n";
    std::cout << "========================================\n";
    std::cout << "  Load this save? (Y/N): ";
    
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (toupper(choice) != 'Y') {
        std::cout << "Load cancelled.\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    // Restore game state
    if (!maze_.load(state.maze_file)) {
        std::cerr << "Error: Cannot load maze file from save: " << state.maze_file << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    current_maze_file_ = state.maze_file;
    
    // Restore player state
    int start_x, start_y;
    maze_.get_start_pos(start_x, start_y);
    player_ = Player(start_x, start_y);
    
    // Replay all moves to restore state
    for (size_t i = 1; i < state.move_history.size() && i - 1 < state.command_history.size(); i++) {
        player_.move_to(state.move_history[i].first, 
                       state.move_history[i].second, 
                       state.command_history[i - 1]);
    }
    
    std::cout << "✓ Game loaded successfully!\n";
    std::cout << "  Restored to position (" << state.player_x << ", " << state.player_y << ") with " << state.step_count << " steps\n";
    std::cout << "Press Enter to continue...";
    std::cin.get();
}
