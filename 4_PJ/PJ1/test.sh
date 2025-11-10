#!/bin/bash
# Test script for the maze game

echo "====================================="
echo "  Maze Game - Build and Test"
echo "====================================="
echo ""

# Test 1: Clean build
echo "Test 1: Building the project..."
make clean > /dev/null 2>&1
if make > /dev/null 2>&1; then
    echo "✓ Build successful"
else
    echo "✗ Build failed"
    exit 1
fi

# Test 2: Check if executable exists
echo "Test 2: Checking executable..."
if [ -f maze_game ]; then
    echo "✓ Executable created"
else
    echo "✗ Executable not found"
    exit 1
fi

# Test 3: Check if compile_commands.json exists
echo "Test 3: Checking compile_commands.json..."
if [ -f compile_commands.json ]; then
    echo "✓ compile_commands.json exists (clangd support enabled)"
else
    echo "⚠ compile_commands.json not found (run: bear -- make)"
fi

echo ""
echo "====================================="
echo "All tests passed! ✓"
echo "====================================="
echo ""
echo "To run the game manually:"
echo "  ./maze_game"
echo ""
echo "Game controls:"
echo "  W/A/S/D - Move (Up/Left/Down/Right)"
echo "  P - Print complete path"
echo "  U - Undo last move"
echo "  Q - Quit game"
echo ""
