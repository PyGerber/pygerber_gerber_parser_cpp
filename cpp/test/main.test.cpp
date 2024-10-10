#include "fmt/format.h"
#include "gerber/gerber.hpp"
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
    auto           gerber_source = fmt::format("G{}*G0{}*G00{}*G000{}*", code, code, code, code);
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 4);

    for (const auto& node : nodes) {
        REQUIRE(node->getNodeName() == fmt::format("G{:0>2}", code));
    }
}

TEST_CASE("Parse G04 with content", "[g_codes]") {
    gerber::Parser parser;
    auto           gerber_source = "G04 Hello, world!*";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 1);
    REQUIRE(nodes[0]->getNodeName() == "G04");
}

TEST_CASE("Parse FS", "[properties]") {
    gerber::Parser parser;
    auto           gerber_source = "%FSLAX26Y26*%";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 1);
    auto fs = std::dynamic_pointer_cast<gerber::FS>(nodes[0]);

    REQUIRE(fs->getNodeName() == "FS");

    REQUIRE(fs->zeros == gerber::Zeros::SKIP_LEADING);
    REQUIRE(fs->coordinate_mode == gerber::CoordinateNotation::ABSOLUTE);

    REQUIRE(fs->x_integral == 2);
    REQUIRE(fs->x_decimal == 6);

    REQUIRE(fs->y_integral == 2);
    REQUIRE(fs->y_decimal == 6);
}

TEMPLATE_TEST_CASE_SIG(
    "Parse MO",
    "[properties]",
    ((gerber::UnitMode::Enum enumValue), enumValue),
    (gerber::UnitMode::INCHES),
    (gerber::UnitMode::MILLIMETERS)
) {
    std::string    unitModeString = gerber::UnitMode(enumValue).toString();
    gerber::Parser parser;
    auto           gerber_source = fmt::format("%MO{}*%", unitModeString);
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 1);
    auto mo = std::dynamic_pointer_cast<gerber::MO>(nodes[0]);

    REQUIRE(mo->getNodeName() == "MO");

    REQUIRE(mo->unit_mode == enumValue);
}