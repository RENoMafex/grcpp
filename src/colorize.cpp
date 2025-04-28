#include "colorize.hpp"

void colorize(boost::process::ipstream& stream, std::ostream& out/* , typename config */) {
    std::string line;
    while (std::getline(stream, line)) {
        //TODO: Make the lines accessible.
        out << line << "\n";
    }
}