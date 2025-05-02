#ifndef COLORIZE_HPP
#define COLORIZE_HPP

#include "grcpp.hpp"

//colorize given stream
void colorize(boost::process::ipstream& stream, std::ostream& out, std::string confname);

//some utility functions for colorize
namespace colorize_utilities{

    std::vector<std::string> split(std::string str, std::string separator);
    std::vector<std::string> split(std::string str, char separator);

    //make string lowercase.
    void tolower(std::string& str);

    //return lowercase string.
    std::string tolower(std::string_view str);

    //strip spaces (front and back only) from a string
    void strip_outer_spaces(std::string& str);
}

#endif
