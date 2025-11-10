#include "MazeGenerator.h"
#include <algorithm>
#include <stack>
#include <fstream>
#include <iostream>
#include <ctime>

MazeGenerator::MazeGenerator(int width, int height) 
    : width_(width), height_(height), rng_(std::random_device{}()) {
    // Ensure dimensions are odd for proper maze generation
    if (width_ % 2 == 0) width_++;
    if (height_ % 2 == 0) height_++;
    
    maze_.resize(height_, std::string(width_, '#'));
}

void MazeGenerator::initialize_maze() {
    // Fill entire maze with walls
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            maze_[y][x] = '#';
        }
    }
}

bool MazeGenerator::is_valid_cell(int x, int y) const {
    return x > 0 && x < width_ - 1 && y > 0 && y < height_ - 1;
}

void MazeGenerator::carve_passages_from(int x, int y) {
    // Mark current cell as passage
    maze_[y][x] = ' ';
    
    // Create array of directions and shuffle them
    std::vector<int> directions = {0, 1, 2, 3};
    std::shuffle(directions.begin(), directions.end(), rng_);
    
    // Try each direction
    for (int dir : directions) {
        // Calculate neighbor cell (2 steps away)
        int nx = x + dx_[dir] * 2;
        int ny = y + dy_[dir] * 2;
        
        // If neighbor is valid and still a wall
        if (is_valid_cell(nx, ny) && maze_[ny][nx] == '#') {
            // Carve passage to neighbor (remove wall between)
            maze_[y + dy_[dir]][x + dx_[dir]] = ' ';
            
            // Recursively carve from neighbor
            carve_passages_from(nx, ny);
        }
    }
}

void MazeGenerator::set_start_and_end() {
    // Set start point near top-left
    // Find first available passage cell
    for (int y = 1; y < height_ - 1; y++) {
        for (int x = 1; x < width_ - 1; x++) {
            if (maze_[y][x] == ' ') {
                maze_[y][x] = 'S';
                goto found_start;
            }
        }
    }
found_start:
    
    // Set end point near bottom-right
    // Find last available passage cell
    for (int y = height_ - 2; y > 0; y--) {
        for (int x = width_ - 2; x > 0; x--) {
            if (maze_[y][x] == ' ') {
                maze_[y][x] = 'E';
                goto found_end;
            }
        }
    }
found_end:
    ;
}

void MazeGenerator::generate() {
    std::cout << "Generating " << width_ << "x" << height_ << " maze...\n";
    
    // Step 1: Initialize with all walls
    initialize_maze();
    
    // Step 2: Start carving from center position (must be odd coordinates)
    int start_x = 1;
    int start_y = 1;
    carve_passages_from(start_x, start_y);
    
    // Step 3: Set start and end points
    set_start_and_end();
    
    std::cout << "Maze generation complete!\n";
}

bool MazeGenerator::save_to_file(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot create file " << filename << std::endl;
        return false;
    }
    
    // Write maze to file
    for (const auto& row : maze_) {
        file << row << '\n';
    }
    
    file.close();
    std::cout << "Maze saved to " << filename << std::endl;
    return true;
}
