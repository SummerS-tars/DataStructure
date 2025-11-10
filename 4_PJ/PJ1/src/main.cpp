#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // If a maze file is provided as argument, skip menu and start directly
    if (argc > 1) {
        Game game;
        if (!game.init(argv[1])) {
            std::cerr << "Failed to load maze: " << argv[1] << std::endl;
            return 1;
        }
        game.run();
    } else {
        // Show main menu
        Game::show_main_menu();
    }
    
    return 0;
}
