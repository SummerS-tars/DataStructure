CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
INCLUDES := -Iinclude

SRC_DIR := src
OBJ_DIR := obj
BIN := maze_game

SRCS := $(filter-out $(SRC_DIR)/test_%.cpp, $(wildcard $(SRC_DIR)/*.cpp))
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BIN)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

run: $(BIN)
	./$(BIN)

.PHONY: all clean run
