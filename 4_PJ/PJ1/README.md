# Maze Path Analysis System

A C++ console-based maze navigation game with path tracking and undo functionality.

## Features Implemented ✓

### Basic Features
- ✅ **Maze Loading**: Load maze from text files with validation
- ✅ **Player Movement**: Control player with W/A/S/D keys (Up/Left/Down/Right)
- ✅ **Collision Detection**: Prevent movement through walls and boundaries
- ✅ **Path Recording**: Track complete movement history using `std::stack`
- ✅ **Path Display**: Print full path from start to current position (P command)
- ✅ **Undo Functionality**: Revert to previous position (U command)
- ✅ **Win Detection**: Detect when player reaches the exit
- ✅ **Interactive UI**: Clear display with position and step count

### Extended Features (To Be Implemented)
- ⏳ **Path Replay**: Automatic replay of movement sequence with animation
- ⏳ **Save/Load**: Save and restore game state
- ⏳ **Maze Generator**: Procedurally generate random mazes
- ⏳ **Multi-player Mode**: Competitive two-player racing

## Project Structure

```
4_PJ/PJ1/
├── include/              # Header files
│   ├── Maze.h           # Maze class declaration
│   ├── Player.h         # Player class declaration
│   └── Game.h           # Game controller class
├── src/                 # Source files
│   ├── Maze.cpp         # Maze implementation
│   ├── Player.cpp       # Player implementation
│   ├── Game.cpp         # Game logic implementation
│   └── main.cpp         # Program entry point
├── data/                # Maze data files
│   ├── Maze1.txt        # 15x15 maze (beginner)
│   └── Maze2.txt        # 21x21 maze (intermediate)
├── saves/               # Saved game states
├── obj/                 # Compiled object files
├── Makefile             # Build configuration
├── test.sh              # Automated test script
├── compile_commands.json # Clangd configuration (generated)
└── README.md            # This file
```

## Core Data Structures

| Component | Data Structure | Purpose |
|-----------|---------------|---------|
| Map Storage | `std::vector<std::string>` | Store 2D maze grid |
| Move History | `std::stack<std::pair<int, int>>` | Enable undo functionality (LIFO) |
| Command History | `std::vector<char>` | Record all commands for replay |
| Player State | `struct Player` | Encapsulate position and history |

## Building the Project

### Prerequisites
- C++17 compatible compiler (g++ 7.0+)
- Make
- bear (optional, for clangd support)

### Compilation

```bash
# Simple build
make

# Clean and rebuild
make clean && make

# Build and run
make run

# Generate compile_commands.json for clangd
bear -- make
```

### Testing

```bash
# Run automated tests
./test.sh
```

## Running the Game

```bash
./maze_game
```

## Game Controls

| Key | Action |
|-----|--------|
| `W` | Move Up |
| `A` | Move Left |
| `S` | Move Down |
| `D` | Move Right |
| `P` | Print complete path from start to current position |
| `U` | Undo last move (return to previous position) |
| `Q` | Quit game |

## Maze File Format

Maze files use the following ASCII characters:
- `#` - Wall (impassable)
- ` ` - Path (walkable space)
- `S` - Start position
- `E` - End position (goal)
- `@` - Player position (displayed during game)

Example:

```
###############
#S    #       #
# # # # ##### #
#     #     #E#
###############
```

## Implementation Details

### Maze Class

- Loads maze from file using `std::ifstream`
- Validates presence of start ('S') and end ('E') positions
- Uses ASCII characters for simple and reliable display
- Provides collision detection via `is_wall()` method

### Player Class
- Uses `std::stack` for move history (enables O(1) undo)
- Uses `std::vector` for command history (enables replay)
- Tracks position with (x, y) coordinates

### Game Class
- Main game loop with state management
- Command processing and validation
- Win condition checking
- Cross-platform screen clearing

## Key Design Decisions

1. **ASCII Character Set**: The maze uses simple ASCII characters (spaces for paths) to avoid UTF-8 encoding complexities and ensure consistent display across all terminals.

2. **Stack for Undo**: Using `std::stack` provides perfect LIFO behavior for undo functionality with minimal memory overhead.

3. **Separate History Vectors**: Command history is stored separately from position history to enable future replay functionality.

4. **Modular Design**: Three core classes (Maze, Player, Game) provide clear separation of concerns and easy testing.

## Development Progress

- [x] Phase 1: Basic structure and Maze loading
- [x] Phase 2: Player movement and collision detection  
- [x] Phase 3: Path tracking and undo functionality
- [x] Phase 4: Win detection and complete game loop
- [ ] Phase 5: Extended features (replay, save/load)

## Clangd Configuration

For proper IDE integration, this project includes:
- `compile_commands.json` - Generated via `bear -- make`
- Configured with `-Iinclude` flag for header resolution

To regenerate after file changes:
```bash
bear -- make clean
bear -- make
```

## Author

Developed for Data Structure Course Project 1

## License

Educational use only
