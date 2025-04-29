#include "colorize.hpp"

void colorize(boost::process::ipstream& input, std::ostream& output, std::string confname) {
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

    std::ifstream file(conffile);

    std::string line;
    while (std::getline(input, line)) {
        //TODO: Make the lines accessible.
        output << line << "\n";
    }
}