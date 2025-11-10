#include "Maze.h"
#include "Player.h"
#include <iostream>

int main() {
    std::cout << "Testing Maze Display with UTF-8 characters\n";
    std::cout << "==========================================\n\n";
    
    Maze maze;
    if (!maze.load("data/Maze1.txt")) {
        std::cerr << "Failed to load maze!\n";
        return 1;
    }
    
    int start_x, start_y;
    maze.get_start_pos(start_x, start_y);
    
    std::cout << "Test 1: Display player at start position\n";
    maze.display(start_x, start_y);
    
    std::cout << "\nTest 2: Display player at various positions\n";
    
    // Test at position (5, 4) - where the corruption was seen
    std::cout << "\nPlayer at (5, 4):\n";
    maze.display(5, 4);
    
    // Test at position (3, 3)
    std::cout << "\nPlayer at (3, 3):\n";
    maze.display(3, 3);
    
    // Test at end position
    int end_x, end_y;
    maze.get_end_pos(end_x, end_y);
    std::cout << "\nPlayer at end position (" << end_x << ", " << end_y << "):\n";
    maze.display(end_x, end_y);
    
    std::cout << "\n✓ All display tests completed!\n";
    
    return 0;
}
