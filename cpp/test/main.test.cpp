import GerberParserCppModule;

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Parser G01", "[g_codes]") {
    GerberParser parser;
    auto result = parser.parse("G1* G01* G001* G0001*");
}
