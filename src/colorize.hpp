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

    //make string lowercase.
    void tolower(std::string& str);

    //return lowercase string.
    std::string tolower(std::string_view str);

    //strip spaces (front and back only) from a string
    void strip_outer_spaces(std::string& str);
}

#endif