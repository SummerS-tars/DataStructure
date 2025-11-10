#include "Game.h"
#include <iostream>

int main() {
    std::cout << "===================================\n";
    std::cout << "  Maze Path Analysis System v1.0\n";
    std::cout << "===================================\n\n";
    
    std::cout << "Loading maze...\n";
    
    Game game;
    if (!game.init("data/Maze1.txt")) {
        std::cerr << "Failed to initialize game!\n";
        return 1;
    }
    
    std::cout << "Maze loaded successfully!\n";
    std::cout << "Press Enter to start the game...";
    std::cin.get();
    
    // Run the game
    game.run();
    
    return 0;
}
