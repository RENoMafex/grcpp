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

#pragma region includes
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
//#include <cstdint> //maybe uncomment, if uintX_t, intX_t or something similar gets used
#pragma endregion

#define GRCPP_VERSION "v0.0.1(pre-alpha)"
#define DLOG(...) std::cout<<__VA_ARGS__<<std::endl;

struct Grcpp_Options { //The options directly used by grcpp
    bool help = false; //does print_help_message() need to be called?
    bool err = false; //redirect stderr
    bool out = false; //redirect stdout
    std::string confname = {}; //configfile name for grcat
    std::string color = "auto"; //colorization of output (auto, on, off)
};

#pragma region function_declarations
//prints help message
void print_help_msg(std::string_view called_name = "grcpp");
//fills an 'Grcpp_Options' struct with the parsed arguments
void init_program_options(int argc, char* argv[], Grcpp_Options &grcpp_options, std::vector<std::string> &other_options);
//checks if all options are valid.
bool invalid_args(Grcpp_Options check);
//checks if an invalid option for the '--color' option is set
bool invalid_color_arg(Grcpp_Options check);
#pragma endregion

#pragma region int main()
int main(int argc, char* argv[]) {
    //initialize needed things
    namespace fs = std::filesystem;
    Grcpp_Options grcpp_options; //holds options for THIS program
    std::vector<std::string> other = {}; //holds options for the called program
    try { //beautify errors from program_options
        init_program_options(argc, argv, grcpp_options, other);
    }catch (const boost::program_options::error &e) {
        std::cout << "\n" << e.what() << "\n" << std::endl;
        print_help_msg(argv[0]);
        return 1;
    }
    
    if (invalid_color_arg(grcpp_options)) {print_help_msg(argv[0]); return 1;}
    if (grcpp_options.help) {print_help_msg(argv[0]); return 0;}
    //from here on we know, that the grcpp_options is properly setup and help isnt needed
    /*if (grcpp_options.confname.empty()) {
        std::string home_location = std::getenv("HOME");
        std::string xdg_conf_location = std::getenv("XDG_CONFIG_HOME");
        if (xdg_conf_location.empty() && !home_location.empty()) {
            xdg_conf_location = home_location + "/.config";
        }
        std::vector<std::string> conf_file_names = {"/etc/grc.conf", "/usr/local/etc/grc.conf"};
        if (!xdg_conf_location.empty()) conf_file_names.emplace_back(xdg_conf_location + "/grc/grc.conf");
        if (!home_location.empty()) conf_file_names.emplace_back(home_location + "/.grc/grc.conf");
        for (auto conf_file : conf_file_names) {
            std::ifstream file(conf_file);
            if (file) {
                std::string line = {};
                while (std::getline(file, line)) {
                    if (line.at(0) == '#' || line.at(0) == '\n') continue; //original code line 98

                }
            }
        }
    }*/

    return 0;
} // int main()

#pragma region function_definitions
void print_help_msg(std::string called_name) {
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
    } catch (const boost::program_options::error &e) {
        std::cerr << "\n" << e.what() << "\n" << std::endl;
        print_help_msg(argv[0]);
        return;
    } catch (...) {
		std::cerr << "\n" <<
			"Unknown Error! Please reach out to <schilling.malte@googlemail.com> or create an issue on <https://github.com/RENoMafex/grcpp>." <<
			"\n" << 
		std::flush;
    }
} // void init_program_options()

bool invalid_args(Grcpp_Options check) {
    return invalid_color_arg(check);
}

bool invalid_color_arg(Grcpp_Options check) {
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
#pragma endregion

#pragma message "WARNING! This software right now is NOT in a working state and should not be used. Please only use this code for educational or development purposes!"
