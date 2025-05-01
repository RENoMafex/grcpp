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

struct Grcpp_Options { //The options directly used by grcpp
    bool help = false; //does print_help_message() need to be called?
    bool err = false; //redirect stderr
    bool out = false; //redirect stdout
//  bool pty = false; //use pseutdoterminal
    std::string confname = {}; //configfile name for grcat
    std::string color = "auto"; //colorization of output (auto, on, off)
};

//prints help message
void print_help_msg(std::string_view called_name = "grcpp");
//fills an 'Grcpp_Options' struct with the parsed arguments
void init_program_options(int argc, char* argv[], Grcpp_Options &grcpp_options, std::vector<std::string> &other_options);
//checks if an invalid option for the '--color' option is set
bool invalid_color_arg(Grcpp_Options& check);

#endif