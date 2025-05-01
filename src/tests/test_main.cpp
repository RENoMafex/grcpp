#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

int main(){
TEST_CASE("Beispieltest") {
    REQUIRE(1 + 1 == 2);
}
}