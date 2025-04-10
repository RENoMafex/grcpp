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

CC = g++
CWARNINGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2
CFLAGS = -std=c++17 $(CWARNINGS)
LDFLAGS = -lboost_program_options -lboost_regex
RUNFLAGS =

.PHONY: all build clean cleanall run

all: run

build:grcpp

run: grcpp
	./grcpp $(RUNFLAGS)

grcpp: src/grcpp.cpp
	$(CC) $(CFLAGS) src/grcpp.cpp -o grcpp $(LDFLAGS)

clean:
	rm -fv build

cleanall: clean
	rm -fv grcpp
