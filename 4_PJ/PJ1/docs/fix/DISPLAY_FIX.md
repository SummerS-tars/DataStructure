# Maze Display Fix - Summary

## Problem
The maze originally used UTF-8 middle dot characters (`·`) for paths. When trying to display the player position (`@`), the character would appear corrupted (e.g., `�@`) because:

1. UTF-8 `·` character is encoded as 2 bytes (0xC2 0xB7)
2. When iterating byte-by-byte through the string, the player position index could fall in the middle of a multi-byte character
3. Trying to replace a single byte in the middle of a UTF-8 sequence causes corruption

Example of corruption:
```
#·#·#�@#####·#·#   <- Player at position (5, 4)
```

## Solution
**Converted maze files to use ASCII characters instead of UTF-8:**

- `·` (UTF-8 middle dot) → ` ` (ASCII space)
- All other characters remain ASCII (`#`, `S`, `E`, `@`)

### Changes Made

1. **Maze Files** (`data/Maze1.txt`, `data/Maze2.txt`)
   - Converted using: `sed 's/·/ /g'`
   - Created backups with `.backup` extension

2. **Code Simplification** (`src/Maze.cpp`)
   - Reverted to simple character-by-character iteration
   - No UTF-8 handling needed
   - Cleaner, more maintainable code

3. **Documentation** (`README.md`)
   - Updated maze format description
   - Changed design decisions to reflect ASCII usage

## Benefits

✅ **No display corruption** - ASCII characters are always 1 byte
✅ **Simpler code** - No UTF-8 parsing logic needed  
✅ **Better portability** - Works consistently across all terminals
✅ **Easier debugging** - Character positions match byte positions
✅ **Cleaner appearance** - Spaces make paths more intuitive

## Result

Before (UTF-8):
```
###############
#S····#·······#
#·#·#�@#####·#·#  <- Corrupted
###############
```

After (ASCII):
```
###############
#S    #       #
# # #@##### # #  <- Clean!
###############
```

## Files Modified

- `data/Maze1.txt` - Converted to ASCII
- `data/Maze2.txt` - Converted to ASCII
- `src/Maze.cpp` - Simplified display() function
- `README.md` - Updated documentation
- `Makefile` - Fixed to exclude test files

## Backups

Original UTF-8 maze files saved as:
- `data/Maze1.txt.backup`
- `data/Maze2.txt.backup`
