import GerberParserCppModule;

#include <catch2/catch_template_test_macros.hpp>
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

TEMPLATE_TEST_CASE_SIG(
    "Parse G codes",
    "[g_codes]",
    ((typename T, int code), T, code),
    (gerber::G01, 1),
    (gerber::G02, 2),
    (gerber::G03, 3),
    (gerber::G04, 4),
    (gerber::G36, 36),
    (gerber::G37, 37),
    (gerber::G54, 54),
    (gerber::G55, 55),
    (gerber::G70, 70),
    (gerber::G71, 71),
    (gerber::G74, 74),
    (gerber::G75, 75),
    (gerber::G90, 90),
    (gerber::G91, 91)
) {
    gerber::Parser parser;
    auto           gerber_source = std::format("G{}*G0{}*G00{}*G000{}*", code, code, code, code);
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 4);

    for (const auto& node : nodes) {
        REQUIRE(node->getNodeName() == std::format("G{:0>2}", code));
    }
}