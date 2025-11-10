CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
INCLUDES := -Iinclude
LDFLAGS :=

SRC_DIR := src
OBJ_DIR := obj
BIN := maze_game

# 检测操作系统
ifeq ($(OS),Windows_NT)
    BIN := maze_game.exe
    RM := rm -f
    RMDIR := rm -rf
    MKDIR := mkdir -p
else
    RM := rm -f
    RMDIR := rm -rf
    MKDIR := mkdir -p $(OBJ_DIR)
    # 某些旧版本 GCC (< 9.1) 需要显式链接 stdc++fs
    # 新版本 GCC 和 w64devkit 通常不需要
    # LDFLAGS := -lstdc++fs
endif

SRCS := $(filter-out $(SRC_DIR)/test_%.cpp, $(wildcard $(SRC_DIR)/*.cpp))
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BIN) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)

clean:
	$(RMDIR) $(OBJ_DIR) 2>/dev/null || true
	$(RM) $(BIN) 2>/dev/null || true

run: $(BIN)
	./$(BIN)

.PHONY: all clean run