# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
RELEASE_FLAGS = -O2 -DNDEBUG
DEBUG_FLAGS = -O0 -DDEBUG

# Target executable name
TARGET = l1-TestDrive
SOURCE = l1-TestDrive.cpp
HEADER = l1-SingleLinkedList.hpp

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(SOURCE) $(HEADER)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Debug build
debug: $(SOURCE) $(HEADER)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -o $(TARGET)_debug $(SOURCE)

# Release build
release: $(SOURCE) $(HEADER)
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -o $(TARGET)_release $(SOURCE)

# Build with TYPE_1 implementation (if you want to add conditional compilation back)
type1: $(SOURCE) $(HEADER)
	$(CXX) $(CXXFLAGS) -DTYPE_1 -o $(TARGET)_type1 $(SOURCE)

# Build with TYPE_2 implementation (if you want to add conditional compilation back)
type2: $(SOURCE) $(HEADER)
	$(CXX) $(CXXFLAGS) -DTYPE_2 -o $(TARGET)_type2 $(SOURCE)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run debug version
run-debug: debug
	./$(TARGET)_debug

# Run release version
run-release: release
	./$(TARGET)_release

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TARGET)_debug $(TARGET)_release $(TARGET)_type1 $(TARGET)_type2
	rm -f *.o *.out core

# Install (optional - copies to /usr/local/bin)
install: release
	sudo cp $(TARGET)_release /usr/local/bin/$(TARGET)

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

# Check for memory leaks with valgrind
valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)_debug

# Format code (requires clang-format)
format:
	clang-format -i $(SOURCE)

# Show help
help:
	@echo "Available targets:"
	@echo "  all       - Build the main executable (default)"
	@echo "  debug     - Build debug version with -g -O0"
	@echo "  release   - Build optimized release version"
	@echo "  type1     - Build with TYPE_1 defined"
	@echo "  type2     - Build with TYPE_2 defined"
	@echo "  run       - Build and run the main executable"
	@echo "  run-debug - Build and run debug version"
	@echo "  run-release - Build and run release version"
	@echo "  clean     - Remove all build artifacts"
	@echo "  install   - Install release version to /usr/local/bin"
	@echo "  uninstall - Remove installed version"
	@echo "  valgrind  - Run debug version with valgrind"
	@echo "  format    - Format source code with clang-format"
	@echo "  help      - Show this help message"

# Phony targets (don't correspond to files)
.PHONY: all debug release type1 type2 run run-debug run-release clean install uninstall valgrind format help
