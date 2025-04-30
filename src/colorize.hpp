#ifndef COLORIZE_HPP
#define COLORIZE_HPP

#include <string>
#include <cctype>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/process.hpp>
#include "colors.hpp"

//colorize given stream
void colorize(boost::process::ipstream& stream, std::ostream& out, std::string confname);

//some utility functions for colorize
namespace colorize_utilities{
    void tolower(std::string& str);
    std::string tolower(std::string_view str);
}

#endif