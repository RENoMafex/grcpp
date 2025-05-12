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
// - See TODO in line 90
// - Implement grcat (inside colorize() function)

//DONE:
// - Rewrite grc

#include "grcpp.hpp"

namespace bp = boost::process;
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
    } catch (...) {
        std::cerr << "\n" <<
            "Unknown Error! Please create an issue on <https://github.com/RENoMafex/grcpp> or reach out to <schilling.malte@googlemail.com>." <<
            "\n" <<
        std::flush;
    }

    if (invalid_color_arg(grcpp_options)) {print_help_msg(argv[0]); return -1;}
    if (grcpp_options.help || (other.empty() && !grcpp_options.getconf)) {print_help_msg(argv[0]); return 0;}
    //TODO: Rewrite the next 3 lines.
    if (!grcpp_options.err) {grcpp_options.out = true;} //grc lines 67 - 73
    if (grcpp_options.err && !grcpp_options.out) {grcpp_options.err = true; grcpp_options.out = false;} //grc lines 67 - 73
    if (grcpp_options.err && grcpp_options.out) {grcpp_options.err = true; grcpp_options.out = true;} //grc lines 67 - 73
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
                if (grcpp_options.getconf) {
                    std::cout << conf_file << std::endl;
                    return 0;
                }
                std::string line = {};
                while (std::getline(file, line)) {
                    colorize_utilities::strip_outer_spaces(line);
                    if (line.empty() || line.at(0) == '#' || line.at(0) == '\n') continue; // comments and empty lines
                    boost::regex pattern(line);
                    if (boost::regex_search(other.at(0), pattern, boost::regex_constants::match_perl)) {
                        std::getline(file, grcpp_options.confname);
                        break;
                    }
                }
                if (grcpp_options.confname.empty()) {
                    std::cout << argv[0] << ((grcpp_options.color == "on") ? " \033[31m\033[1mERROR:\033[0m" : " ERROR:") <<
                    " Configfile for command \"" << other.at(0) << "\" could not be determined!\n" << std::endl;
                }
                break;
            } // if (file)
        } // for (auto conf_file : conf_file_names)
    } // if (grcpp_options.confname.empty())

    const auto executable = bp::search_path(other.at(0));
    const std::vector<std::string> exe_args(other.begin() + 1, other.end());

    if (!grcpp_options.confname.empty() && grcpp_options.color == "on") { //TODO: check if evaluation is really needed.
        bp::ipstream out_stream;
        bp::ipstream err_stream;

        bp::child child_process(bp::exe = executable, bp::args = exe_args, bp::std_out > out_stream, bp::std_err > err_stream);
        std::vector<std::thread> colorize_threads;
        if (grcpp_options.out) {
            colorize_threads.emplace_back(
                colorize, std::ref(out_stream), std::ref(std::cout), grcpp_options.confname
            );
        }
        if (grcpp_options.err) {
            colorize_threads.emplace_back(
                colorize, std::ref(err_stream), std::ref(std::cerr), grcpp_options.confname
            );
        }
        for (auto& t : colorize_threads) {
            t.join();
        }
        child_process.wait();

        return child_process.exit_code();
    } else {
        bp::child child_process(bp::exe = executable, bp::args = exe_args);

        child_process.wait();
        return child_process.exit_code();
    }

    return 0;
} // int main()

#pragma message "WARNING! This software right now is NOT in a working state and should not be used. Please only use this code for educational or development purposes!"
