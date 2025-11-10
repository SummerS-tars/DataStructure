#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <string>
#include <vector>

class Game;  // Forward declaration

class MenuManager {
public:
    MenuManager();
    
    // Main menu display and handling
    static void show_main_menu();
    
    // Interactive selection menus
    static std::string select_maze_file();
    static int select_save_slot();
    static void generate_maze_menu();
    
    // File scanning utilities
    static std::vector<std::string> scan_maze_files();
    static std::vector<int> scan_save_slots();
    
private:
    static void show_help();
    static bool validate_int_input(int& value);
};

#endif // MENU_MANAGER_H
