# Compiler and Flags
CXX ?= g++
CXXFLAGS = $(WARNINGS) -std=c++17 -O0
WARNINGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2
LDFLAGS = -lboost_program_options -lboost_regex

# Directories
SRC_DIR = src
BUILD_DIR = build

# Define pseudotargets
.PHONY: all run clean cleanall test

# Source, Header and Object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
HDRS = $(wildcard $(SRC_DIR)/*.hpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TEST_OBJS = $(filter-out $(BUILD_DIR)/main.o, $(OBJS))

TEST_FILE = $(SRC_DIR)/tests/test_main.cpp

# Output
TARGET = grcpp
TEST_OBJ = $(BUILD_DIR)/test_runner.o
TEST_TARGET = $(SRC_DIR)/tests/test_runner

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

# Clean Object files and the test runner
clean:
	@rm -rfv $(BUILD_DIR)
	@rm -fv $(TEST_TARGET)

# Clean out file and objects
cleanall: clean
	@rm -rfv $(TARGET)

# Compile test file
$(TEST_OBJ): $(TEST_FILE) $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $(TEST_OBJ)

# Link test runner
$(TEST_TARGET): $(TEST_OBJ) $(TEST_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Testing target
test: $(TEST_TARGET)
	@echo
	$(TEST_TARGET) --use-colour=yes
