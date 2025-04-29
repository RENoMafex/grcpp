#include "colorize.hpp"

void colorize(boost::process::ipstream& input, std::ostream& output/* , typename config */) {
    std::string home = std::getenv("HOME");
    std::string xdg_config = std::getenv("XDG_CONFIG_HOME");
    std::string xdg_data = std::getenv("XDG_DATA_HOME");
    std::string conffile;

    if (!home.empty() && xdg_config.empty()) {
        xdg_config = home + "/.config";
    }

    if (!home.empty() && xdg_data.empty()) {
        xdg_data = home + "/.local/share";
    }

    std::vector<std::string> conffilepaths;
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

    std::string line;
    while (std::getline(input, line)) {
        //TODO: Make the lines accessible.
        output << line << "\n";
    }
}