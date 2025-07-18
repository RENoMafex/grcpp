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

#include "colorize.hpp"

void colorize(boost::process::ipstream& input, std::ostream& output, std::string confname) {
    using namespace colorize_utilities;
    std::string home = std::getenv("HOME");
    std::string xdg_config = std::getenv("XDG_CONFIG_HOME");
    std::string xdg_data = std::getenv("XDG_DATA_HOME");

    if (!home.empty() && xdg_config.empty()) {
        xdg_config = home + "/.config";
    }

    if (!home.empty() && xdg_data.empty()) {
        xdg_data = home + "/.local/share";
    }

    std::vector<std::string> conffilepaths = {};
    if (!xdg_data.empty()) {
        conffilepaths.emplace_back(xdg_data + "/grc/");
        conffilepaths.emplace_back(xdg_data + "/grcpp/");
    }
    if (!xdg_config.empty()) {
        conffilepaths.emplace_back(xdg_config + "/grc/");
        conffilepaths.emplace_back(xdg_config + "/grcpp/");
    }
    if (!home.empty()) {
        conffilepaths.emplace_back(home + "/grc/");
        conffilepaths.emplace_back(home + "/grcpp/");
    }
    conffilepaths.emplace_back("/usr/local/share/grc/");
    conffilepaths.emplace_back("/usr/share/grc/");

    std::string conffile = {};
    for (const auto &cf : conffilepaths) {
        std::ifstream file(cf + confname);
        if (file) {
            conffile = cf + confname;
            break;
        }
    }

    if (conffile.empty()) {
        output << "\033[31m\033[1mERROR:\033[0m configfile " << confname << " not found!" << std::endl;
        return;
    }

    std::ifstream file_conffile(conffile);
    std::string conf_line = {};
    std::unordered_map<std::string, std::string> my_map = {{"count", "more"}}; // named 'll' in grcat

    while (std::getline(file_conffile, conf_line)) {
        strip_outer_spaces(conf_line);
        if (conf_line.empty() || conf_line.at(0) == '#' || conf_line.at(0) == '\n') continue; // comments and empty lines
        std::array<std::string, 2> fields = {conf_line.substr(0, conf_line.find("=")), conf_line.substr(conf_line.find("=") + 1)};
        auto keyword = tolower(std::as_const(fields.at(0)));
        auto value = fields.at(1);
        {
            auto& KW = keyword;
            if (KW=="color"||KW=="colour"||KW=="colors"||KW=="colours") KW = "color";
            if (!(KW=="regexp"||KW=="color"||KW=="count"||KW=="command"||KW=="skip"||KW=="replace"||KW=="concat")) continue;
        }
        my_map.insert({keyword, value});
    } //while

    std::vector<std::string> color_strings = {}; // namend 'colstrings' in grcat
    for (auto color_grp : split(my_map["color"], ",")) {
        auto color_list = split(color_grp, " ");
        std::string c = {};
        for (auto i : color_list) {
            c = c + to_escape(i);
        }
        color_strings.emplace_back(c);
    }
    std::vector<decltype(my_map)> expression_list = {};
    if (my_map.find("regexp") != my_map.end()) {
        //TODO: Make my_map["regexp"] callable (grcat line 199)
        expression_list.emplace_back(my_map);
    }

    std::string prev_color = to_escape("default");
    std::string prev_count = "more";

    std::string stdout_line = {};
    while (std::getline(input, stdout_line)) {
        std::vector<std::string> c_list = {};
        bool skip = false,
        blockflag = false;
        for (auto pattern : expression_list) {
            int pos = 0;
            auto curr_count = pattern["count"];
            bool was_replace = false;
            while (true) {
                auto m = pattern["regexp"];
                if (!m.empty()) { // grcat line 220
                    boost::regex line(m); // i dont think its right this way?
                    if (pattern.find("replace") != pattern.end()) {
                        if (was_replace) break;
                    }
                }
            }
        }

        output << stdout_line << "\n";
    }
} // void colorize();

std::vector<std::string> colorize_utilities::split(std::string str, std::string separator){
    std::vector<std::string> result = {};
    while (str.find(separator) != std::string::npos) {
        result.emplace_back(str.substr(0, str.find(separator)));
        str = str.substr(str.find(separator) + 1);
    }
    result.emplace_back(str);
    return result;
}

std::vector<std::string> colorize_utilities::split(std::string str, char separator) {
    return split(str, std::string(1, separator));
}

void colorize_utilities::tolower(std::string& str) {
    for (auto& c : str) { // Ensures defined tolower behavior
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
}

std::string colorize_utilities::tolower(std::string_view str) {
    std::string output = {};
    output.reserve(str.size());
    for (const auto c : str) { // Ensures defined tolower behavior
        output.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }
    return output;
}

void colorize_utilities::strip_outer_spaces(std::string& str) {
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
