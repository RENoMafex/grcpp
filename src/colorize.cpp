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
    std::vector<std::string> expression_list = {};
    std::string conf_line = {};
    std::unordered_map<std::string, std::string> mymap = {{"count", "more"}}; //TODO: rename this var

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
        mymap.insert({keyword, value});
    } //while

    std::string stdout_line = {};
    while (std::getline(input, stdout_line)) {
        //TODO: process input and write to output
        output << stdout_line << "\n";
    }
}

std::vector<std::string> colorize_utilities::split(std::string str, std::string separator){
    return {str, separator};
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
