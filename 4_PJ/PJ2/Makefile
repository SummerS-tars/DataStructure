# PJ2 Makefile

CXX ?= g++
CXXFLAGS ?= -std=c++23 -Wall
DBGFLAGS ?= -g -O0
RELFLAGS ?= -O2 -DNDEBUG
INCLUDES := -Iinclude

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
TARGET := $(BIN_DIR)/pj2

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
MAIN_OBJ := $(BUILD_DIR)/main.o
LIB_OBJS := $(filter-out $(MAIN_OBJ), $(OBJS))

.PHONY: all debug release run test clean help test-bin

all: debug

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# Debug build
debug: CXXFLAGS += $(DBGFLAGS)
debug: $(TARGET)

# Release build
release: CXXFLAGS += $(RELFLAGS)
release: $(TARGET)

# Run with arguments: make run ARGS="30"
run: debug
	$(TARGET) $(ARGS)

# Minimal test script
test: debug
	$(MAKE) -s test-bin
	bash scripts/test_sample.sh

test-bin: $(TARGET)
	@mkdir -p $(BIN_DIR) $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DBGFLAGS) $(INCLUDES) tests/test_phase1.cpp $(LIB_OBJS) -o $(BIN_DIR)/test_phase1
	$(CXX) $(CXXFLAGS) $(DBGFLAGS) $(INCLUDES) tests/test_phase2.cpp $(LIB_OBJS) -o $(BIN_DIR)/test_phase2
	$(CXX) $(CXXFLAGS) $(DBGFLAGS) $(INCLUDES) tests/test_phase3.cpp $(LIB_OBJS) -o $(BIN_DIR)/test_phase3
	$(CXX) $(CXXFLAGS) $(DBGFLAGS) $(INCLUDES) tests/test_phase4.cpp $(LIB_OBJS) -o $(BIN_DIR)/test_phase4

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

help:
	@echo "Targets:"
	@echo "  make [all|debug]   - Build debug binary"
	@echo "  make release       - Build release binary"
	@echo "  make run ARGS=...  - Run with arguments"
	@echo "  make test          - Run minimal test script"
	@echo "  make clean         - Remove build outputs" 
