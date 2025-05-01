# Compiler and Flags
CXX ?= g++
CXXFLAGS = $(WARNINGS) -std=c++17 -O0
WARNINGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2
LDFLAGS = -lboost_program_options -lboost_regex

# Directories
SRC_DIR = src
TEST_DIR := $(SRC_DIR)/tests
BUILD_DIR = build

# Define pseudotargets
.PHONY: all run clean cleanall test

# Source, Header and Object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
HDRS = $(wildcard $(SRC_DIR)/*.hpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Output
TARGET = grcpp

# Defaulttarget
all: $(TARGET)

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDRS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Make the build directory if needed
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Run-Target
run:
	@if [ ! -f $(TARGET) ]; then $(MAKE) $(TARGET); fi
	@./$(TARGET)

# Clean Object files
clean:
	@rm -rfv $(BUILD_DIR)

# Clean out file and objects
cleanall: clean
	@rm -rfv $(TARGET)

# Testing target
test: $(TEST_DIR)/main_test.cpp
	$(CXX) $(CXXFLAGS) -Iexternal/Catch2/src -o $(TEST_DIR)/test_runner $(TEST_DIR)/main_test.cpp
	./$(TEST_DIR)/test_runner