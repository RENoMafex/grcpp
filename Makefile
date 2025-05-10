# This file is part of grcpp, a C++ rewrite of Radovan Garabiks 'grc'
# Copyright (C) 2025 Malte Schilling schilling.malte@googlemail.com
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
CHDRS = $(patsubst $(SRC_DIR)/%.hpp, $(SRC_DIR)/%.hpp.gch, $(HDRS))
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

# Precompile headers
$(SRC_DIR)/%.hpp.gch: $(SRC_DIR)/%.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -x c++-header $< -o $@

# Link
$(TARGET): $(CHDRS) $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Make the build directory if needed
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile test file
$(TEST_OBJ): $(TEST_FILE) $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $(TEST_OBJ)

# Link test runner
$(TEST_TARGET): $(TEST_OBJ) $(TEST_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Precompile Catch
$(SRC_DIR)/tests/%.hpp.gch: $(SRC_DIR)/tests/%.hpp
	$(CXX) $(CXXFLAGS) -x c++-header $< -o $@

# Run-Target
run:
	@if [ ! -f $(TARGET) ]; then $(MAKE) $(TARGET); fi
	@./$(TARGET)

# Clean Object files and the test runner
clean:
	@rm -rfv $(BUILD_DIR)
	@rm -fv $(TEST_TARGET)
	@rm -fv $(CHDRS)

# Clean out file and objects
cleanall: clean
	@rm -rfv $(TARGET)

# Testing target
test: $(SRC_DIR)/tests/test_main.hpp.gch $(CHDRS) $(TEST_TARGET)
	@echo
	$(TEST_TARGET) --use-colour=yes
