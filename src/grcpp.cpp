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

#include "grcpp.hpp"

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
        "        --getconf        check where the used \"grc.conf\" is located\n" <<
        "\n" <<
        "THIS SOFTWARE IS LICENSED UNDER THE GNU GENERAL PUBLIC LICENSE V3.0!\n" <<
        "Copyright (c) Malte Schilling 2025\n" <<
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
            ("color,colour", value<string>(), "Set coloration to \"on\", \"off\" or \"auto\"")
            ("getconf", "check where grc.conf lives");

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
        if (varmap.count("getconf")) grcpp_options.getconf = true;
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
    const bool istty = isatty(STDOUT_FILENO);
    if (check.color == "auto") {
        check.color = (istty) ? "on" : "off";
        return false;
    }
    if (check.color == "on") return false;
    if (check.color == "off") return false;
    if (istty) {
        cout << "\n\033[31m\033[1mERROR:\033[0m invalid argument for option '--color'\n" << endl;
    } else {
        cout << "\nERROR: invalid argument for option '--color'\n" << endl;
    }

    return true;
}
