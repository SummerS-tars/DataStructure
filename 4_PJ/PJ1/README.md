# Maze Path Analysis System

A feature-rich C++ console-based maze navigation game with advanced path tracking, replay system, multi-slot save/load functionality, and procedural maze generation.

## 📋 Features Implemented

### ✅ Basic Features (50 points)
1. **Maze Loading & Display** (10 pts)
   - Load maze from text files with validation
   - Detect and validate start (S) and end (E) positions
   - ASCII-based display for cross-platform compatibility
   - Real-time player position rendering with '@' marker

2. **Player Movement** (10 pts)
   - WASD controls (Up/Left/Down/Right)
   - Collision detection with walls and boundaries
   - Batch command processing support (e.g., "WWSSDDA")
   - Invalid move error handling

3. **Path Recording** (10 pts)
   - Complete movement history using `std::stack`
   - Command history tracking with `std::vector`
   - Path display from start to current position (P command)
   - Step counter

4. **Undo Functionality** (10 pts)
   - Revert to previous position (U command)
   - Maintains consistent command and position history
   - Stack-based O(1) undo operation

5. **Win Detection** (10 pts)
   - Automatic detection when reaching exit
   - Victory message with statistics
   - Path summary display

### ✨ Extended Features (15+ points)

6. **Path Replay System** (5 pts)
   - Animated replay of complete movement sequence
   - Step-by-step visualization with 0.5s delay
   - Preserves original game state after replay

7. **Multi-Slot Save/Load System** (5 pts)
   - 5 independent save slots
   - Metadata storage (timestamp, step count, position, maze file)
   - Load confirmation dialog with preview
   - Save file listing with detailed information
   - Automatic saves/ directory creation

8. **Procedural Maze Generator** (5 pts)
   - Random DFS algorithm for maze generation
   - Configurable maze size (5x5 to 101x101)
   - Guaranteed solvable mazes
   - Export to standard maze format

9. **Two-Player Race Mode** (10 pts)
   - Simultaneous two-player competitive gameplay
   - Turn-based movement system (one move per turn)
   - Real-time position display (Player 1: @, Player 2: %)
   - First to reach exit wins
   - Independent path tracking and statistics
   - Victory screen with step comparison

10. **Enhanced Menu System** (Bonus)
    - Interactive main menu with file scanning
    - Game mode selection (Single Player / Two Players)
    - Automatic detection of available mazes in data/
    - Automatic detection of save files
    - Input validation and error handling
    - Context-sensitive help system

### 🏗️ Code Architecture Improvements

10. **Modular Design**
    - Separated menu logic into `MenuManager` class
    - Extracted save/load logic into `SaveManager` class
    - Clear separation of concerns (MVC-like pattern)
    - Reduced Game.cpp from 779 lines to ~450 lines

**Total Score: 50 (Basic) + 25 (Extended) = 75+ points**

## 📁 Project Structure

```text
4_PJ/PJ1/
├── include/                    # Header files
│   ├── Maze.h                 # Maze class - file loading and display
│   ├── Player.h               # Player class - state and history
│   ├── Game.h                 # Game controller - main loop
│   ├── GameState.h            # Game state serialization
│   ├── SaveManager.h          # Save/load management
│   ├── MenuManager.h          # Menu system and UI
│   └── MazeGenerator.h        # Procedural maze generation
├── src/                       # Source files
│   ├── Maze.cpp               # Maze implementation
│   ├── Player.cpp             # Player implementation
│   ├── Game.cpp               # Game logic (~450 lines)
│   ├── GameState.cpp          # Binary file I/O
│   ├── SaveManager.cpp        # Multi-slot save system
│   ├── MenuManager.cpp        # Menu and selection UI
│   ├── MazeGenerator.cpp      # Random DFS maze generation
│   └── main.cpp               # Program entry point
├── data/                      # Maze data files
│   ├── Maze1.txt              # 15x15 maze (beginner)
│   ├── Maze2.txt              # 21x21 maze (intermediate)
│   └── Maze3.txt              # 31x31 maze (advanced)
├── saves/                     # Saved game states (auto-created)
│   └── game1.save ~ game5.save
├── docs/                      # Documentation
│   ├── prepare/               # Project requirements
│   ├── to-fix/                # Issue tracking
│   └── fix/                   # Analysis documents
├── obj/                       # Compiled object files (generated)
├── Makefile                   # Build configuration
├── maze_game                  # Executable (generated)
├── test*.sh                   # Test scripts
├── compile_commands.json      # Clangd configuration (generated)
└── README.md                  # This file
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

## 🎮 Running the Game

### Interactive Menu Mode (Recommended)

```bash
./maze_game
```

The main menu provides:

- **Start New Game**: Choose single-player or two-player mode
  - Single Player: Solo maze solving with full features
  - Two Players: Competitive race mode (turn-based)
- **Load Saved Game**: Continue from any of 5 save slots (single-player only)
- **Generate New Maze**: Create custom random mazes
- **View Help**: In-game command reference
- **Exit**: Quit the application

### Direct Load Mode

```bash
./maze_game data/Maze1.txt
```

Skips the menu and loads a specific maze file directly.

## 🎯 Game Controls

### Movement Commands

| Key | Action | Batch Example |
|-----|--------|---------------|
| `W` | Move Up | `WWW` = Move up 3 times |
| `A` | Move Left | `AAA` = Move left 3 times |
| `S` | Move Down | `SSS` = Move down 3 times |
| `D` | Move Right | `DDD` = Move right 3 times |

### Function Commands

| Command | Action | Description |
|---------|--------|-------------|
| `P` | Print Path | Display complete path from start to current position |
| `U` | Undo | Return to previous position (stack-based) |
| `R` | Replay | Animated replay of all moves with 0.5s delay |
| `SAVE [1-5]` | Save Game | Save to slot (default: slot 1) |
| `LOAD [1-5]` | Load Game | Load from slot with confirmation |
| `SAVES` | List Saves | Show all saved games with details |
| `HELP` | Show Help | Display in-game help |
| `QUIT` / `Q` | Exit Game | Quit to main menu or exit |

### Example Command Sequences

**Single Player Mode:**
```text
WWSSDDA     - Execute multiple moves at once
SAVE 1      - Save to slot 1
LOAD 2      - Load from slot 2
SAVES       - List all saves
```

**Two-Player Mode:**
```text
W           - Player 1 moves up (one move per turn)
[Next turn]
S           - Player 2 moves down (one move per turn)
[Game continues until one player reaches exit]
```

### Two-Player Mode Rules

- **Turn-Based**: Players alternate turns, one move per turn
- **Single Command Only**: Batch commands (e.g., "WWW") not allowed in multiplayer
- **Visual Markers**: 
  - Player 1: `@` (red in some terminals)
  - Player 2: `%` (blue in some terminals)
- **Victory Condition**: First player to reach exit (`E`) wins
- **Statistics**: Game displays step count for both players at the end
- **Limitations**: Save/Load, Undo, and Replay not available in multiplayer mode

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

## 🔧 Implementation Details

### Class Architecture

#### Maze Class
- **File Loading**: `std::ifstream` for text file reading
- **Validation**: Checks for start ('S') and end ('E') positions
- **Display**: ASCII-based rendering with player marker
- **Collision**: `is_wall(x, y)` for boundary and wall detection
- **Data Structure**: `std::vector<std::string>` for 2D grid

#### Player Class
- **Move History**: `std::stack<std::pair<int, int>>` for O(1) undo
- **Command History**: `std::vector<char>` for replay functionality
- **State**: Current position (x, y) and move count
- **Player ID**: Supports multiple player instances (1-2)
- **Display Marker**: Customizable symbol (@ for P1, % for P2)
- **Methods**: `move_to()`, `undo()`, `get_path()`, `get_commands()`, `id()`, `marker()`

#### Game Class
- **Game Loop**: Main state machine with command processing
- **Command Parser**: Multi-character command support
- **Win Detection**: Automatic victory check after each move
- **Integration**: Coordinates Maze, Player, and SaveManager

#### GameState Class
- **Serialization**: Binary file format for save states
- **Metadata**: Timestamp, step count, maze file, position
- **Methods**: `save()`, `load()`, `get_timestamp_str()`, `get_maze_name()`

#### SaveManager Class
- **Multi-Slot**: 5 independent save slots
- **File Management**: Automatic directory creation
- **Info Display**: Formatted save file listing
- **Validation**: Slot number and file existence checks

#### MenuManager Class
- **Main Menu**: Interactive file and save selection
- **File Scanning**: Automatic detection of mazes and saves
- **Input Validation**: Robust error handling for all inputs
- **Generator Interface**: Maze size and filename prompts

#### MazeGenerator Class
- **Algorithm**: Randomized Depth-First Search (DFS)
- **Maze Structure**: Guaranteed solvable paths
- **Size**: Configurable from 5x5 to 101x101
- **Export**: Standard text format compatible with Maze class

### Key Design Decisions

#### 1. ASCII Character Set
**Problem**: UTF-8 characters (·) caused display corruption  
**Solution**: Use simple ASCII spaces for paths  
**Benefit**: Cross-platform compatibility, consistent rendering

#### 2. Stack for Undo
**Choice**: `std::stack<std::pair<int, int>>`  
**Rationale**: Perfect LIFO behavior, O(1) operations  
**Alternative Considered**: Vector with index (rejected: more complex)

#### 3. Separate Command History
**Design**: Commands stored separately from positions  
**Purpose**: Enable replay without recomputing moves  
**Tradeoff**: Small memory overhead for cleaner replay logic

#### 4. Modular Architecture
**Pattern**: MVC-inspired separation  
- **Model**: Maze + Player (game state)
- **View**: Display functions in Game and Maze
- **Controller**: Game (main loop) + MenuManager (UI)

**Benefits**:
- Reduced Game.cpp from 779 to ~450 lines
- Easier testing and maintenance
- Clear responsibility boundaries

#### 5. Binary Save Format
**Format**: Custom binary with metadata header  
**Structure**: `[timestamp][step_count][player_pos][maze_file][history]`  
**Advantage**: Compact, fast, versioning support  
**Alternative Considered**: JSON (rejected: too verbose)

#### 6. Friend Class Pattern
**Usage**: `MenuManager` as friend of `Game`  
**Purpose**: Access private `handle_load_command()`  
**Justification**: Tight coupling between menu and game state management

#### 7. Two-Player Turn-Based System
**Design**: Single command per turn restriction in multiplayer  
**Implementation**: `multiplayer_mode` flag disables batch processing  
**State Management**: Two `Player` instances with independent histories  
**Display**: `Maze::display_multiplayer()` shows both player positions  
**Rationale**: Ensures fair competition, prevents one player from taking multiple moves

## 📊 Data Structures Summary

| Component | Data Structure | Rationale | Complexity |
|-----------|---------------|-----------|------------|
| Maze Grid | `vector<string>` | 2D access, dynamic size | O(1) access |
| Move History | `stack<pair<int,int>>` | LIFO undo | O(1) push/pop |
| Command History | `vector<char>` | Sequential replay | O(1) append |
| Save Slots | File system (5 files) | Persistence, isolation | O(1) per slot |
| Maze Generator | DFS with stack | Guarantee connectivity | O(W×H) |

## 🎯 Development Progress

- [x] **Phase 1**: Basic structure and Maze loading
- [x] **Phase 2**: Player movement and collision detection  
- [x] **Phase 3**: Path tracking and undo functionality
- [x] **Phase 4**: Win detection and complete game loop
- [x] **Phase 5**: Extended features (replay, save/load)
- [x] **Phase 6**: Procedural maze generation
- [x] **Phase 7**: Code refactoring and modularization
- [x] **Phase 8**: Enhanced menu system and UX
- [x] **Phase 9**: Two-player competitive race mode
- [x] **Phase 10**: Documentation and comprehensive testing

## 📈 Project Statistics

- **Total Lines of Code**: ~3000+ lines
- **Number of Classes**: 7 (Game, Maze, Player, GameState, SaveManager, MenuManager, MazeGenerator)
- **Data Structures Used**: Stack, Vector, String, Pair
- **Features Implemented**: 5 Basic + 4 Extended = 9 major features
- **Total Score**: 75+ points (50 basic + 25 extended)

## 📝 Usage Examples

### Example 1: Complete Playthrough

```text
$ ./maze_game

[Main Menu appears]
Select option (1-5): 1

[Maze selection]
[1] Maze1.txt
[2] Maze2.txt
Enter your choice: 1

[Game starts]
Commands: WWSSDDA     # Move through maze
Commands: P           # Print current path
Commands: U           # Undo last move
Commands: SAVE 1      # Save to slot 1
Commands: R           # Replay all moves
Commands: Q           # Quit

[Returns to main menu]
```

### Example 2: Generate and Play Custom Maze

```text
Select option (1-5): 3

Enter maze width: 21
Enter maze height: 21
Enter filename: data/MyMaze.txt

[Maze generated successfully]

Select option (1-5): 1
[1] Maze1.txt
[2] Maze2.txt
[3] MyMaze.txt      ← Your new maze!
```

### Example 3: Save and Resume

```text
[Playing Maze1.txt]
Commands: WWWSSSDDD
Commands: SAVE 2      # Save to slot 2
Commands: Q

[Later session]
Select option (1-5): 2

[2] Maze1.txt - Pos(8,5) - 9 steps - 01-11 14:30

Enter slot number: 2
Load this save? (Y/N): Y

[Game resumes from saved position]
```

### Example 4: Two-Player Race Mode

```text
$ ./maze_game

Select option (1-5): 1

Select game mode:
  [1] Single Player
  [2] Two Players (Race)
Enter your choice: 2

[Select maze file]
[1] Maze1.txt
Enter your choice: 1

========================================
    Two-Player Race Mode
========================================
Player 1 (@): Steps 0
Player 2 (%): Steps 0
========================================

[Maze displayed with both players at start]

[Player 1's Turn]
Commands: W          # Player 1 moves up

[Player 2's Turn]
Commands: S          # Player 2 moves down

[Turns continue...]

========================================
🎉 Player 1 Wins! 🎉
========================================
Player 1: 15 steps
Player 2: 18 steps
========================================
```

## 🛠️ Clangd Configuration

For proper IDE integration (VS Code, Vim, Emacs):

```bash
# Generate compile_commands.json
bear -- make clean
bear -- make
```

This enables:
- Code completion
- Jump to definition
- Real-time error checking
- Symbol navigation

## 🧪 Testing

### Automated Tests

```bash
# Test all features
./test.sh

# Test maze generator
./test_generator.sh

# Test input validation
./test_input_validation.sh
```

### Manual Testing Checklist

**Single-Player Mode:**

- [ ] Load each maze file successfully
- [ ] Move in all four directions (W/A/S/D)
- [ ] Hit walls and verify error message
- [ ] Reach exit and see victory message
- [ ] Use undo (U) to revert moves
- [ ] Print path (P) at various points
- [ ] Replay moves (R) and verify animation
- [ ] Save to all 5 slots
- [ ] Load from each slot with confirmation
- [ ] List saves (SAVES) and verify display
- [ ] Generate mazes of different sizes
- [ ] Play generated mazes
- [ ] Test batch commands (e.g., "WWSSDD")

**Two-Player Mode:**

- [ ] Select two-player mode from menu
- [ ] Verify both players start at 'S'
- [ ] Confirm players take turns (alternating)
- [ ] Verify only one command per turn accepted
- [ ] Check both players display correctly (@ and %)
- [ ] Test collision for both players
- [ ] Verify first player to exit wins
- [ ] Check victory screen shows both step counts
- [ ] Confirm batch commands are rejected
- [ ] Test with different maze sizes

## 🐛 Known Issues and Fixes

All major issues have been resolved:

1. ✅ **UTF-8 Display Corruption** → Converted to ASCII
2. ✅ **Collision Error Handling** → Added early return on wall hit
3. ✅ **Replay Command Bug** → Fixed by copying command vector
4. ✅ **Input Validation Loop** → Added cin.fail() checks
5. ✅ **Table Alignment** → Replaced box drawings with simple separators
6. ✅ **Code Organization** → Refactored into MenuManager and SaveManager

## 🚀 Future Enhancements (Optional)

Possible extensions beyond current implementation:

1. **Multi-player Race Mode**
   - Two players compete to reach exit first
   - Split-screen or turn-based gameplay
   - Leaderboard with best times

2. **Advanced Maze Features**
   - Teleporters
   - Keys and locked doors
   - Moving obstacles
   - Multiple exit points

3. **AI Pathfinding**
   - Auto-solver using A* or Dijkstra
   - Hint system showing optimal next move
   - Path comparison (player vs optimal)

4. **Graphics**
   - ncurses for better terminal UI
   - SDL2 for graphical version
   - Color-coded paths

## 📚 References

- **Data Structures**: Goodrich, Tamassia, Mount - "Data Structures and Algorithms in C++"
- **Maze Generation**: Jamis Buck - "Mazes for Programmers"
- **DFS Algorithm**: Cormen et al. - "Introduction to Algorithms"

## 👤 Author

Developed for Data Structure Course - Project 1

## 📄 License

Educational use only - Data Structure Course 2025
