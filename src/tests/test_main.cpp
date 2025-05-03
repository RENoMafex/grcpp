#define CATCH_CONFIG_MAIN
#include "test_main.hpp"

TEST_CASE("to_escape() and color_map") {
    //std::string_view to_escape()
    CHECK(to_escape("bold") + "this" == "\033[1mthis");
    CHECK(to_escape("red") + "is" == "\033[31mis");
    CHECK(to_escape("anything") + "a" == "\033[0ma");
    CHECK(to_escape("yellow") + "Test!" == "\033[33mTest!");
}

TEST_CASE("colorize_utilities"){
    using namespace colorize_utilities;

    // std::string tolower()
    CHECK(tolower("HELLO WORLD") == "hello world");

    // void tolower()
    std::string str = "HELLO WORLD!";
    tolower(str);
    CHECK(str == "hello world!");

    // void strip_outer_spaces()
    str = "   hello world    ";
    strip_outer_spaces(str);
    CHECK(str == "hello world");

    // std::vector<std::string> split()
    std::vector<std::string> vec = split(str, " ");
    CHECK(vec.at(0) == "hello");
    CHECK(vec.at(1) == "world");

    vec = {};
    vec = split("hello,this,is,a,test", ",");
    CHECK(vec.at(0) == "hello");
    CHECK(vec.at(1) == "this");
    CHECK(vec.at(2) == "is");
    CHECK(vec.at(3) == "a");
    CHECK(vec.at(4) == "test");
    CHECK_THROWS(vec.at(5) == "something");

    vec = {};
    vec = split("hello,this,is,a,test", ',');
    CHECK(vec.at(0) == "hello");
    CHECK(vec.at(1) == "this");
    CHECK(vec.at(2) == "is");
    CHECK(vec.at(3) == "a");
    CHECK(vec.at(4) == "test");
    CHECK_THROWS(vec.at(5) == "something");
}

TEST_CASE("grcpp specific functions"){
    SECTION("invalid_color_arg"){
        Grcpp_Options my_opts;
        CHECK_FALSE(invalid_color_arg(my_opts));
        my_opts.color = "funny";
        CHECK(invalid_color_arg(my_opts));
    }
    SECTION("init_program_options"){
        char *argv[] = {"grcpp", "-s", "-e", "lsblk", "-p"};
        int argc = 5;
        Grcpp_Options o;
        std::vector<std::string> other;
        init_program_options(argc, argv, o, other);

        CHECK_FALSE(o.help);
        CHECK(o.err);
        CHECK(o.out);
        CHECK(o.color == "auto");
        // CHECK(o.confname == "conf.lsblk"); //gets set in main, not in init_program_options.
        CHECK(other.at(0) == "lsblk");
        CHECK(other.at(1) == "-p");
    }
}
