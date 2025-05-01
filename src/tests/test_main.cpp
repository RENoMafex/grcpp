#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../grcpp.hpp"

TEST_CASE("to_escape() and color_map") {
    //std::string_view to_escape()
    REQUIRE(to_escape("bold") == "\033[1m");
    REQUIRE(to_escape("red") == "\033[31m");
    REQUIRE(to_escape("anything") == "\033[0m");
    REQUIRE(to_escape("yellow") == "\033[33m");
}

TEST_CASE("colorize_utilities"){
    // std::string tolower()
    REQUIRE(colorize_utilities::tolower("HELLO WORLD") == "hello world");

    // void tolower()
    std::string str = "HELLO WORLD!";
    colorize_utilities::tolower(str);
    REQUIRE(str == "hello world!");

    // void strip_outer_spaces()
    str = "   hello world    ";
    colorize_utilities::strip_outer_spaces(str);
    REQUIRE(str == "hello world");
}