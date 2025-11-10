#pragma once

#include <vector>
#include <string>
#include <random>

class MazeGenerator {
public:
    MazeGenerator(int width, int height);
    
    // Generate a maze using randomized DFS algorithm
    void generate();
    
    // Save the generated maze to a file
    bool save_to_file(const std::string& filename) const;
    
    // Get the generated maze
    const std::vector<std::string>& get_maze() const { return maze_; }
    
private:
    int width_;   // Width of the maze (must be odd)
    int height_;  // Height of the maze (must be odd)
    std::vector<std::string> maze_;
    std::mt19937 rng_;
    
    // Helper functions
    void initialize_maze();
    void carve_passages_from(int x, int y);
    bool is_valid_cell(int x, int y) const;
    void set_start_and_end();
    
    // Direction arrays for DFS (up, right, down, left)
    const int dx_[4] = {0, 1, 0, -1};
    const int dy_[4] = {-1, 0, 1, 0};
};
