/*
* This file is part of grcpp, a C++ rewrite of Radovan Garabiks 'grc'
* Copyright (C) 2025 Malte Schilling schilling.malte@googlemail.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GRCPP_HPP
#define GRCPP_HPP

#define BOOST_PROCESS_USE_STD_FS // No need for boost.filesystem

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <boost/process.hpp>
#include "colors.hpp"
#include "colorize.hpp"

#define GRCPP_VERSION "v0.0.1(pre-alpha)"
#define DLOG(x) std::cout<<x<<std::endl;

/// @struct Grcpp_Options
/// @brief The options directly used by grcpp.
///
/// This struct holds the options, which determine, what grcpp should do.
/// @param help Boolean, determines, if the help needs to get displayed.
/// @param err Boolean, determines, if stderr should be colorized.
/// @param out Boolean, determines, if stdout should be colorized.
/// @param confname String, configfile name for colourize().
/// @param color String, colorization of output (auto, on, off).
/// @param getconf Boolean, determines, if the location of "grc.conf" should be output.
struct Grcpp_Options {
    bool help = false;
    bool err = false;
    bool out = false;
//  bool pty = false; //use pseutdoterminal
    std::string confname = {};
    std::string color = "auto";
    //options from here on are not available in grc
    bool getconf = false;
};

/// @brief Prints the help message.
/// @param called_name Should be argv[0], defaults to "grcpp".
void print_help_msg(std::string_view called_name = "grcpp");

/// @brief Initializes the options for grcpp.
/// @param argc Just argc.
/// @param argv Just argv[].
/// @param grcpp_options The struct, that holds the options.
/// @param other_options A vector<string> which holds the options for the called program.
void init_program_options(int argc, char* argv[], Grcpp_Options &grcpp_options, std::vector<std::string> &other_options);

/// @brief Checks if the check.color argument is valid.
/// @param check The struct, in which the color argument should be checked.
/// @return true, when invalid. false, when valid.
auto invalid_color_arg(Grcpp_Options& check) -> bool;

#endif
