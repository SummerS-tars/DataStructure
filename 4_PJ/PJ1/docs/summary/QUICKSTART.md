# Quick Start Guide - Maze Path Analysis System

## Installation & Setup

### 1. Navigate to Project Directory
```bash
cd /home/thesumst/Data2/development/DataStructure/4_PJ/PJ1
```

### 2. Compile the Project
```bash
make clean && make
```

### 3. Run the Game
```bash
./maze_game
```

---

## How to Play

### Basic Movement
- **W** - Move up
- **A** - Move left  
- **S** - Move down
- **D** - Move right

You can enter multiple commands at once: `WWWDDDSS`

### Special Commands
- **P** - Show your complete path
- **U** - Undo last move
- **R** - Replay all your moves (with animation!)
- **SAVE** - Save your progress
- **LOAD** - Load saved game
- **HELP** - Show all commands
- **QUIT** - Exit game

### Game Objective
Navigate from **S** (start) to **E** (exit) while avoiding **#** (walls).

---

## Example Session

```
Enter command(s): WWDDD
Enter command(s): P          # View path
Enter command(s): U          # Undo last move
Enter command(s): SAVE       # Save progress
Enter command(s): R          # Watch replay
Enter command(s): LOAD       # Restore save
```

---

## Tips

1. **Try multiple commands**: Enter "WWWDDDSS" to queue several moves
2. **Hit a wall?**: The game stops and shows an error - no worries!
3. **Made a mistake?**: Press 'U' to undo your last move
4. **Want to see your journey?**: Press 'P' to view your complete path
5. **Take a break?**: Use 'SAVE' before quitting, 'LOAD' when you return

---

## Troubleshooting

**Q: Game won't compile?**  
A: Make sure you have g++ with C++17 support:
```bash
g++ --version  # Should be 7.0+
```

**Q: Save file not found?**  
A: The `saves/` directory is created automatically. Use 'SAVE' first!

**Q: Replay is too fast/slow?**  
A: Currently fixed at 0.5 seconds per move.

---

## For Developers

### Generate IDE Support
```bash
bear -- make clean && bear -- make
```

This creates `compile_commands.json` for clangd/LSP support.

### Project Structure
- `include/` - Header files
- `src/` - Implementation
- `data/` - Maze files (Maze1.txt, Maze2.txt)
- `saves/` - Save game files

### Adding New Mazes
Create a text file in `data/` with:
- `#` for walls
- ` ` (space) for paths
- `S` for start
- `E` for exit

Example:
```
#####
#S  #
# # #
#  E#
#####
```

---

## Have Fun! 🎮

Enjoy navigating the maze and exploring all the features!
