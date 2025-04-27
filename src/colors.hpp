#ifndef COLORS_HPP
#define COLORS_HPP

#include <array>
#include <string_view>
#include <utility> // std::pair


const std::string_view to_escape(std::string_view color) {
    for (const auto& pair : color_map) {
        if (pair.first == color) {
            return pair.second;
        }
    }
}

constexpr std::array<std::pair<std::string_view, std::string_view>, 46> color_map = {{

// Formatting
    {"none",                ""},
    {"default",             "\033[0m"},
    {"bold",                "\033[1m"},
    {"underline",           "\033[4m"},
    {"blink",               "\033[5m"},
    {"reverse",             "\033[7m"},
    {"concealed",           "\033[8m"},

// Colors
    {"black",               "\033[30m"},
    {"red",                 "\033[31m"},
    {"green",               "\033[32m"},
    {"yellow",              "\033[33m"},
    {"blue",                "\033[34m"},
    {"magenta",             "\033[35m"},
    {"cyan",                "\033[36m"},
    {"white",               "\033[37m"},

// Backgrounds
    {"on_black",            "\033[40m"},
    {"on_red",              "\033[41m"},
    {"on_green",            "\033[42m"},
    {"on_yellow",           "\033[43m"},
    {"on_blue",             "\033[44m"},
    {"on_magenta",          "\033[45m"},
    {"on_cyan",             "\033[46m"},
    {"on_white",            "\033[47m"},

// Misc
    {"beep",                "\007"},
    {"previous",            "prev"},
    {"unchanged",           "unchanged"},

// non-standard attributes, supported by some terminals
    {"dark",                "\033[2m"},
    {"italic",              "\033[3m"},
    {"rapidblink",           "\033[6m"},
    {"strikethrough",       "\033[9m"},

// aixterm bright color codes
    {"bright_black",        "\033[30;90m"},
    {"bright_red",          "\033[31;91m"},
    {"bright_green",        "\033[32;92m"},
    {"bright_yellow",       "\033[33;93m"},
    {"bright_blue",         "\033[34;94m"},
    {"bright_magenta",      "\033[35;95m"},
    {"bright_cyan",         "\033[36;96m"},
    {"bright_white",        "\033[37;97m"},

// aixterm bright background
    {"on_bright_black",     "\033[40;100m"},
    {"on_bright_red",       "\033[41;101m"},
    {"on_bright_green",     "\033[42;102m"},
    {"on_bright_yellow",    "\033[43;103m"},
    {"on_bright_blue",      "\033[44;104m"},
    {"on_bright_magenta",   "\033[45;105m"},
    {"on_bright_cyan",      "\033[46;106m"},
    {"on_bright_white",     "\033[47;107m"}
}};

#endif
