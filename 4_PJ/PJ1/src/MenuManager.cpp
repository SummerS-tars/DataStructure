#include "MenuManager.h"
#include "Game.h"
#include "SaveManager.h"
#include "GameState.h"
#include "MazeGenerator.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <dirent.h>

#ifdef _WIN32
    #include <cstdlib>
    #define CLEAR_COMMAND "cls"
#else
    #include <cstdlib>
    #define CLEAR_COMMAND "clear"
#endif

MenuManager::MenuManager() {}

bool MenuManager::validate_int_input(int& value) {
    std::cin >> value;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n✗ Invalid input! Please enter a valid number.\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return false;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

// Scan data directory for maze files
std::vector<std::string> MenuManager::scan_maze_files() {
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
std::vector<int> MenuManager::scan_save_slots() {
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
std::string MenuManager::select_maze_file() {
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
    if (!validate_int_input(choice)) {
        return "";
    }
    
    if (choice < 1 || choice > static_cast<int>(mazes.size())) {
        return "";
    }
    
    return "data/" + mazes[choice - 1];
}

// Interactive save slot selection
int MenuManager::select_save_slot() {
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
    if (!validate_int_input(choice)) {
        return 0;
    }
    
    // Validate choice
    if (std::find(slots.begin(), slots.end(), choice) == slots.end()) {
        return 0;
    }
    
    return choice;
}

// Generate maze menu
void MenuManager::generate_maze_menu() {
    system(CLEAR_COMMAND);
    std::cout << "\n========================================\n";
    std::cout << "       Maze Generator\n";
    std::cout << "========================================\n";
    std::cout << "  Create a new random maze\n";
    std::cout << "========================================\n\n";
    
    int width, height;
    std::string filename;
    
    std::cout << "Enter maze width (odd number, e.g., 15, 21, 31): ";
    if (!validate_int_input(width)) {
        return;
    }
    
    std::cout << "Enter maze height (odd number, e.g., 15, 21, 31): ";
    if (!validate_int_input(height)) {
        return;
    }
    
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

// Show help text
void MenuManager::show_help() {
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
}

// Main menu
void MenuManager::show_main_menu() {
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
        if (!validate_int_input(choice)) {
            continue;
        }
        
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
            generate_maze_menu();
        } else if (choice == 4) {
            // Show help
            show_help();
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
