#!/bin/bash

cd /home/thesumst/Data2/development/DataStructure/4_PJ/PJ1

echo "=== Test 1: Testing invalid string input in main menu ==="
echo "Input: 'abc' (should show error and return to menu)"
echo -e "abc\n5" | timeout 5 ./maze_game
echo ""

echo "=== Test 2: Testing invalid character input ==="
echo "Input: 'x' (should show error and return to menu)"
echo -e "x\n5" | timeout 5 ./maze_game
echo ""

echo "=== Test 3: Testing out of range number ==="
echo "Input: '99' (should show 'Invalid choice' message)"
echo -e "99\n5" | timeout 5 ./maze_game
echo ""

echo "=== Test 4: Testing negative number ==="
echo "Input: '-1' (should show 'Invalid choice' message)"
echo -e "-1\n5" | timeout 5 ./maze_game
echo ""

echo "=== Test 5: Testing valid input then exit ==="
echo "Input: '5' (should exit normally)"
echo "5" | timeout 5 ./maze_game
echo ""

echo "=== All tests completed ==="
