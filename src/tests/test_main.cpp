#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../grcpp.hpp"

TEST_CASE("to_escape() and color_map") {
    REQUIRE(to_escape("bold") == "\033[1m");
    REQUIRE(to_escape("red") == "\033[31m");
    REQUIRE(to_escape("anything") == "\033[0m");
    REQUIRE(to_escape("yellow") == "\033[33m");
}

TEST_CASE("colorize_utilities"){
    REQUIRE(colorize_utilities::tolower("HELLO WORLD") == "hello world");

    std::string str = "HELLO WORLD!";
    colorize_utilities::tolower(str);
    REQUIRE(str == "hello world!");
}