#ifndef COLORIZE_HPP
#define COLORIZE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/process.hpp>
#include "colors.hpp"

//colorize given stream
void colorize(boost::process::ipstream& stream, std::ostream& out, std::string confname);

#endif