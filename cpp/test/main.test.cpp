import GerberParserCppModule;

#include <catch2/catch_test_macros.hpp>

TEST_CASE("SyntaxError", "[exceptions]") {
    REQUIRE_THROWS_AS(
        []() {
            throw gerber::SyntaxError("Syntax error at index 0 (line: 0 column: 0)");
        }(),
        gerber::SyntaxError
    );
}

TEST_CASE("SyntaxError from parser", "[exceptions]") {
    gerber::Parser parser;
    REQUIRE_THROWS_AS(
        [&parser]() {
            parser.parse("lol");
        }(),
        gerber::SyntaxError
    );
}

TEST_CASE("Parse G01", "[g_codes]") {
    gerber::Parser parser;
    auto           result = parser.parse("G1*G01*G001*G0001*");

    REQUIRE(result.getNodes().size() == 4);
}

TEST_CASE("Parse G02", "[g_codes]") {
    gerber::Parser parser;
    auto           result = parser.parse("G2*G02*G002*G0002*");

    REQUIRE(result.getNodes().size() == 4);
}
