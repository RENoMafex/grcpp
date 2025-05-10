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

#ifndef COLORIZE_HPP
#define COLORIZE_HPP

#include "grcpp.hpp"

//colorize given stream
void colorize(boost::process::ipstream& stream, std::ostream& out, std::string confname);

//some utility functions for colorize
namespace colorize_utilities{

    auto split(std::string str, std::string separator) -> std::vector<std::string>;
    auto split(std::string str, char separator) -> std::vector<std::string>;

    //make string lowercase.
    void tolower(std::string& str);

    //return lowercase string.
    auto tolower(std::string_view str) -> std::string;

    //strip spaces (front and back only) from a string
    void strip_outer_spaces(std::string& str);
}

#endif
