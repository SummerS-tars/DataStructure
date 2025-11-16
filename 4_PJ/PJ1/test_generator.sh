#!/bin/bash

# Test maze generator

cd /home/thesumst/Data2/development/DataStructure/4_PJ/PJ1

echo "=== Testing Maze Generator ==="
echo ""

# Test 1: Generate a small maze
echo "Test 1: Generate 15x15 maze"
echo -e "3\n15\n15\ndata/Generated15x15.txt\n5" | ./maze_game

echo ""
echo "Test 2: Check if file was created"
if [ -f "data/Generated15x15.txt" ]; then
    echo "✓ File created successfully!"
    echo "Content preview:"
    head -20 data/Generated15x15.txt
else
    echo "✗ File not created"
fi

echo ""
echo "Test 3: Try to play the generated maze"
echo -e "SSSDDDWWWQ" | ./maze_game data/Generated15x15.txt 2>&1 | head -50

echo ""
echo "=== Tests Complete ==="
