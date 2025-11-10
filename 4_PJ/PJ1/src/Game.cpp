#include "Game.h"
#include "GameState.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>

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

void Game::generate_maze_menu() {
    clear_screen();
    std::cout << "\n========================================\n";
    std::cout << "       Maze Generator\n";
    std::cout << "========================================\n";
    std::cout << "  Create a new random maze\n";
    std::cout << "========================================\n\n";
    
    int width, height;
    std::string filename;
    
    std::cout << "Enter maze width (odd number, e.g., 15, 21, 31): ";
    std::cin >> width;
    
    // Check for input failure
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n✗ Invalid input! Please enter a valid number.\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    std::cout << "Enter maze height (odd number, e.g., 15, 21, 31): ";
    std::cin >> height;
    
    // Check for input failure
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n✗ Invalid input! Please enter a valid number.\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter filename to save (e.g., data/MyMaze.txt): ";
    std::getline(std::cin, filename);
    
    // Validate input
    if (width < 5 || height < 5) {
        std::cout << "\n✗ Error: Maze size too small! Minimum size is 5x5.\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    if (width > 101 || height > 101) {
        std::cout << "\n✗ Error: Maze size too large! Maximum size is 101x101.\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    // Create generator and generate maze
    std::cout << "\n----------------------------------------\n";
    MazeGenerator generator(width, height);
    generator.generate();
    
    // Save to file
    if (generator.save_to_file(filename)) {
        std::cout << "\n========================================\n";
        std::cout << "  ✓ Maze Generated Successfully!\n";
        std::cout << "========================================\n";
        std::cout << "  Size:     " << width << "x" << height << "\n";
        std::cout << "  File:     " << filename << "\n";
        std::cout << "========================================\n";
        std::cout << "\nYou can now play this maze by loading it\n";
        std::cout << "from the main menu.\n";
    } else {
        std::cout << "\n✗ Failed to save maze to file.\n";
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Game::show_main_menu() {
    while (true) {
        // Scan for available files
        std::vector<std::string> mazes = scan_maze_files();
        std::vector<int> saves = scan_save_slots();
        
        std::cout << "\n========================================\n";
        std::cout << "    Maze Path Analysis System\n";
        std::cout << "========================================\n";
        std::cout << "  1. Start New Game";
        if (!mazes.empty()) {
            std::cout << " (" << mazes.size() << " maze" << (mazes.size() > 1 ? "s" : "") << " available)";
        }
        std::cout << "\n";
        
        std::cout << "  2. Load Saved Game";
        if (!saves.empty()) {
            std::cout << " (" << saves.size() << " save" << (saves.size() > 1 ? "s" : "") << " found)";
        }
        std::cout << "\n";
        
        std::cout << "  3. Generate New Maze\n";
        std::cout << "  4. View Help\n";
        std::cout << "  5. Exit\n";
        std::cout << "========================================\n";
        std::cout << "  Select option (1-5): ";
        
        int choice;
        std::cin >> choice;
        
        // Check if input failed (e.g., user entered non-numeric input)
        if (std::cin.fail()) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "\n✗ Invalid input! Please enter a number between 1 and 5.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 1) {
            // Start new game - use interactive selection
            std::string maze_file = select_maze_file();
            
            if (!maze_file.empty()) {
                Game game;
                if (game.init(maze_file)) {
                    game.run();
                } else {
                    std::cout << "Failed to load maze. Press Enter to continue...";
                    std::cin.get();
                }
            }
        } else if (choice == 2) {
            // Load saved game - use interactive selection
            int slot = select_save_slot();
            
            if (slot > 0) {
                // Get maze file from save
                SaveManager manager;
                GameState state;
                if (manager.get_save_info(slot, state)) {
                    Game game;
                    if (game.init(state.maze_file)) {
                        game.handle_load_command(slot);
                        game.run();
                    } else {
                        std::cout << "Failed to load maze from save. Press Enter to continue...";
                        std::cin.get();
                    }
                }
            }
        } else if (choice == 3) {
            // Generate new maze
            Game game;
            game.generate_maze_menu();
        } else if (choice == 4) {
            // Show help
            std::cout << "\n========================================\n";
            std::cout << "            Help\n";
            std::cout << "========================================\n";
            std::cout << "【Movement Commands】\n";
            std::cout << "  W - Move up\n";
            std::cout << "  A - Move left\n";
            std::cout << "  S - Move down\n";
            std::cout << "  D - Move right\n\n";
            std::cout << "【Function Commands】\n";
            std::cout << "  P - Print complete path\n";
            std::cout << "  U - Undo last move\n";
            std::cout << "  R - Replay all moves\n";
            std::cout << "  SAVE [1-5] - Save game\n";
            std::cout << "  LOAD [1-5] - Load game\n";
            std::cout << "  SAVES - List all saves\n";
            std::cout << "  HELP - Show this help\n";
            std::cout << "  QUIT - Exit game\n\n";
            std::cout << "【Game Rules】\n";
            std::cout << "  * Move from Start (S) to End (E)\n";
            std::cout << "  * '#' = Wall (cannot pass)\n";
            std::cout << "  * ' ' = Passage (can pass)\n";
            std::cout << "========================================\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
        } else if (choice == 5) {
            std::cout << "\nThank you for playing!\n";
            break;
        } else {
            std::cout << "\nInvalid choice. Please select 1-5.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
        }
    }
}

// Scan data directory for maze files
std::vector<std::string> Game::scan_maze_files() {
    std::vector<std::string> mazes;
    DIR* dir = opendir("data");
    
    if (dir == nullptr) {
        return mazes;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename = entry->d_name;
        // Check if it's a .txt file
        if (filename.length() > 4 && filename.substr(filename.length() - 4) == ".txt") {
            mazes.push_back(filename);
        }
    }
    closedir(dir);
    
    std::sort(mazes.begin(), mazes.end());
    return mazes;
}

// Scan saves directory for save files
std::vector<int> Game::scan_save_slots() {
    std::vector<int> slots;
    SaveManager manager;
    
    for (int i = 1; i <= 5; i++) {
        if (manager.save_exists(i)) {
            slots.push_back(i);
        }
    }
    
    return slots;
}

// Interactive maze file selection
std::string Game::select_maze_file() {
    std::vector<std::string> mazes = scan_maze_files();
    
    if (mazes.empty()) {
        std::cout << "\n✗ No maze files found in data/ directory!\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return "";
    }
    
    std::cout << "\n========================================\n";
    std::cout << "      Select a Maze File\n";
    std::cout << "========================================\n";
    
    for (size_t i = 0; i < mazes.size(); i++) {
        std::cout << "  [" << (i + 1) << "] " << mazes[i] << "\n";
    }
    std::cout << "  [0] Cancel\n";
    std::cout << "========================================\n";
    std::cout << "Enter your choice: ";
    
    int choice;
    std::cin >> choice;
    
    // Check for input failure
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "✗ Invalid input!\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return "";
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (choice < 1 || choice > static_cast<int>(mazes.size())) {
        return "";
    }
    
    return "data/" + mazes[choice - 1];
}

// Interactive save slot selection
int Game::select_save_slot() {
    std::vector<int> slots = scan_save_slots();
    
    if (slots.empty()) {
        std::cout << "\n✗ No save files found!\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return 0;
    }
    
    SaveManager manager;
    std::cout << "\n========================================\n";
    std::cout << "      Select a Save File\n";
    std::cout << "========================================\n";
    
    for (int slot : slots) {
        GameState state;
        if (manager.get_save_info(slot, state)) {
            std::cout << "  [" << slot << "] " << state.get_maze_name() 
                     << " - Pos(" << state.player_x << "," << state.player_y 
                     << ") - " << state.step_count << " steps"
                     << " - " << state.get_timestamp_str().substr(5, 11) << "\n";
        }
    }
    std::cout << "  [0] Cancel\n";
    std::cout << "========================================\n";
    std::cout << "Enter slot number: ";
    
    int choice;
    std::cin >> choice;
    
    // Check for input failure
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "✗ Invalid input!\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return 0;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Validate choice
    if (std::find(slots.begin(), slots.end(), choice) == slots.end()) {
        return 0;
    }
    
    return choice;
}
