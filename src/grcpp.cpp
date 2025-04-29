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


//TODO:
// - Implement catch_signal() from grc
// - Implement line 106 (needs catch_signal() )
// - Fix that if the called program takes one of the options,
//   grcpp also takes, that it gets forwarded right. (is it even an issue?)
// - Add option -getconfig to display which grc.conf is used
// - See TODO in line 90
// - Implement grcat (inside colorize() function)

//DONE:
// - Rewrite grc



#define BOOST_PROCESS_USE_STD_FS // No need for boost.filesystem

#pragma region includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <boost/process.hpp>
#include "colors.hpp"
#include "colorize.hpp"
// #include <cstdint> //maybe uncomment, if uintX_t, intX_t or something similar gets used
#pragma endregion

#define GRCPP_VERSION "v0.0.1(pre-alpha)"
#define DLOG(x) std::cout<<x<<std::endl;

namespace bp = boost::process;

struct Grcpp_Options { //The options directly used by grcpp
    bool help = false; //does print_help_message() need to be called?
    bool err = false; //redirect stderr
    bool out = false; //redirect stdout
//  bool pty = false; //use pseutdoterminal
    std::string confname = {}; //configfile name for grcat
    std::string color = "auto"; //colorization of output (auto, on, off)
};

#pragma region function_declarations
//prints help message
void print_help_msg(std::string_view called_name = "grcpp");
//fills an 'Grcpp_Options' struct with the parsed arguments
void init_program_options(int argc, char* argv[], Grcpp_Options &grcpp_options, std::vector<std::string> &other_options);
//checks if an invalid option for the '--color' option is set
bool invalid_color_arg(Grcpp_Options& check);
//strip spaces (front and back only) from a string
void strip_outer_spaces(std::string& str);
#pragma endregion

#pragma region int main()
int main(int argc, char* argv[]) {
    //initialize needed things
    Grcpp_Options grcpp_options; //holds options for THIS program
    std::vector<std::string> other = {}; //holds options for the called program
    try { //beautify errors from program_options
        init_program_options(argc, argv, grcpp_options, other);
    } catch (const boost::program_options::error &error) {
        std::cout << "\n" << error.what() << "\n" << std::endl;
        print_help_msg(argv[0]);
        return -1;
    }

    if (invalid_color_arg(grcpp_options)) {print_help_msg(argv[0]); return -1;}
    if (grcpp_options.help || other.empty()) {print_help_msg(argv[0]); return 0;}
    //TODO: Rewrite the next 3 lines.
    if (!grcpp_options.err) {grcpp_options.out = true;} //lines 67 - 73
    if (grcpp_options.err && !grcpp_options.out) {grcpp_options.err = true; grcpp_options.out = false;} //lines 67 - 73
    if (grcpp_options.err && grcpp_options.out) {grcpp_options.err = true; grcpp_options.out = true;} //lines 67 - 73
    //from here on we know, that the grcpp_options is properly setup, other isnt empty and help isnt needed
    if (grcpp_options.confname.empty()) {
        //TODO: Make this not os dependant.
        std::string home_location = std::getenv("HOME");
        std::string xdg_conf_location = std::getenv("XDG_CONFIG_HOME");
        if (xdg_conf_location.empty() && !home_location.empty()) {
            xdg_conf_location = home_location + "/.config";
        }
        std::vector<std::string> conf_file_names = {"/etc/grc.conf", "/usr/local/etc/grc.conf"};
        if (!xdg_conf_location.empty()) conf_file_names.emplace_back(xdg_conf_location + "/grc/grc.conf");
        if (!home_location.empty()) conf_file_names.emplace_back(home_location + "/.grc/grc.conf");
        for (auto conf_file : conf_file_names) {
            std::ifstream file(conf_file); //file should be one of the "grc.conf"s
            if (file) {
                std::string line = {};
                while (std::getline(file, line)) {
                    strip_outer_spaces(line);
                    if (line.empty() || line.at(0) == '#' || line.at(0) == '\n') continue; // comments and empty lines
                    boost::regex pattern(line);
                    if (boost::regex_search(other.at(0), pattern, boost::regex_constants::match_perl)) {
                        std::getline(file, grcpp_options.confname);
                        break;
                    }
                }
                if (grcpp_options.confname.empty()) {
                    std::cout << "ERROR: Configfile for command \"" << other.at(0) << "\" not found!\n" << std::endl;
                    print_help_msg(argv[0]);
                    return -1;
                }
                break;
            } // if (file)
        } // for (auto conf_file : conf_file_names)
    } // if (grcpp_options.confname.empty())

    const auto executable = bp::search_path(other.at(0));
    const std::vector<std::string> exe_args(other.begin() + 1, other.end());

    bp::ipstream out_stream;
    bp::ipstream err_stream;

    if (!grcpp_options.confname.empty() && grcpp_options.color == "on") { //TODO: check if evaluation is really needed.
        bp::child child_process(bp::exe = executable, bp::args = exe_args, bp::std_out > out_stream, bp::std_err > err_stream);

        std::vector<std::thread> colorize_threads;

        if (grcpp_options.out) {
            colorize_threads.emplace_back(colorize, std::ref(out_stream), std::ref(std::cout));
        }
        if (grcpp_options.err) {
            colorize_threads.emplace_back(colorize, std::ref(err_stream), std::ref(std::cerr));
        }

        for (auto& t : colorize_threads) {
            t.join();
        }

        child_process.wait();

        return child_process.exit_code();
    } else {
        bp::child child_process(bp::exe = executable, bp::args = exe_args, bp::std_out > out_stream, bp::std_err > err_stream);

        child_process.wait();

        return child_process.exit_code();
    }

    return 0;
} // int main()

#pragma region function_definitions

void print_help_msg(std::string_view called_name) {
    std::cout <<
        "Generic Colorizer (C++) " << GRCPP_VERSION << " compiled on " << __DATE__ << "\n" << 
        called_name << " [options] command [args]\n" <<
        "Options:\n" <<
        "-h      --help           Show this help.\n" <<
        "-e      --stderr         Redirect stderr. If this option is selected,\n" <<
        "                         do not automatically redirect stdout\n" <<
        "-s      --stdout         Redirect stdout, even if -e is selected\n" <<
        "-c name --config=name    Use name as configuration file for grcat\n" <<
        "        --color=word     Word is one of: on, off, auto\n" <<
        "        --colour=word    Same as color, for compatibility reasons\n" <<
        "\n" <<
        "THIS SOFTWARE IS LICENSED UNDER THE GNU GENERAL PUBLIC LICENSE V3.0!\n" <<
        "Copyright (c) Malte Schilling 2025" <<
    std::flush;
}

void init_program_options(int argc, char* argv[], Grcpp_Options &grcpp_options, std::vector<std::string> &other_options) {
    try {
        using namespace boost::program_options;
        using std::string;

        options_description grcpp_opts("grcpp options");
        grcpp_opts.add_options()
            ("help,h", "Show help.")
            ("stderr,e", "Redirect stderr.")
            ("stdout,s", "Redirect stdout.")
            ("config,c", value<string>(), "Config file name for grcat")
            ("color,colour", value<string>(), "Set coloration to \"on\", \"off\" or \"auto\"");

        parsed_options parsed = command_line_parser(argc, argv).
            options(grcpp_opts).
            allow_unregistered().
            run();

        variables_map varmap;
        store(parsed, varmap);
        notify(varmap);

        other_options = collect_unrecognized(parsed.options, include_positional);

        if (varmap.count("help")) grcpp_options.help = true;
        if (varmap.count("stderr")) grcpp_options.err = true;
        if (varmap.count("stdout")) grcpp_options.out = true;
        if (varmap.count("config")) grcpp_options.confname = varmap["config"].as<string>();
        if (varmap.count("color")) grcpp_options.color = varmap["color"].as<string>();
    } catch (const boost::program_options::error &error) {
        std::cerr << "\n" << error.what() << "\n" << std::endl;
        print_help_msg(argv[0]);
        return;
    } catch (...) {
        std::cerr << "\n" <<
            "Unknown Error! Please create an issue on <https://github.com/RENoMafex/grcpp> or reach out to <schilling.malte@googlemail.com>." <<
            "\n" <<
        std::flush;
    }
} // void init_program_options()

bool invalid_color_arg(Grcpp_Options& check) {
    using std::cout, std::endl;
    if (check.color == "auto") {
        check.color = (isatty(STDOUT_FILENO)) ? "on" : "off";
        return false;
    }
    if (check.color == "on") return false;
    if (check.color == "off") return false;

    cout << "\ninvalid argument for option '--color'\n" << endl;

    return true;
}

void strip_outer_spaces(std::string& str) {
    size_t start = 0;
    while (start < str.size() && std::isspace(str[start])) {
        ++start;
    }

    size_t end = str.size();
    while (end > start && std::isspace(str[end - 1])) {
        --end;
    }

    str = str.substr(start, end - start);
}

#pragma endregion

#pragma message "WARNING! This software right now is NOT in a working state and should not be used. Please only use this code for educational or development purposes!"
