#include "fmt/format.h"
#include "gerber/ast/d_codes/Dnn.hpp"
#include "gerber/ast/other/coordinate.hpp"
#include "gerber/gerber.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <tuple>

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

// D codes

TEMPLATE_TEST_CASE_SIG(
    "Parse D codes",
    "[d_codes]",
    ((typename T, int code), T, code),
    (gerber::D01, 1),
    (gerber::D02, 2),
    (gerber::D03, 3)
) {
    gerber::Parser parser;
    auto           gerber_source = fmt::format("D{}*D0{}*D00{}*D000{}*", code, code, code, code);
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 4);

    for (const auto& node : nodes) {
        REQUIRE(node->getNodeName() == fmt::format("D{:0>2}", code));
    }
}

TEST_CASE("Parse Dnn codes", "[d_codes]") {
    gerber::Parser parser;
    auto           gerber_source = "D4*D32*D99*D999*";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 4);
    auto d0 = std::dynamic_pointer_cast<gerber::Dnn>(nodes[0]);
    auto d1 = std::dynamic_pointer_cast<gerber::Dnn>(nodes[1]);
    auto d2 = std::dynamic_pointer_cast<gerber::Dnn>(nodes[2]);
    auto d3 = std::dynamic_pointer_cast<gerber::Dnn>(nodes[3]);

    REQUIRE(d0->getNodeName() == "Dnn");
    REQUIRE(d0->getApertureId() == "4");

    REQUIRE(d1->getNodeName() == "Dnn");
    REQUIRE(d1->getApertureId() == "32");

    REQUIRE(d2->getNodeName() == "Dnn");
    REQUIRE(d2->getApertureId() == "99");

    REQUIRE(d3->getNodeName() == "Dnn");
    REQUIRE(d3->getApertureId() == "999");
}

// G codes

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

// Load

TEMPLATE_TEST_CASE_SIG(
    "Parse LP",
    "[load]",
    ((gerber::Polarity::Enum enumValue), enumValue),
    (gerber::Polarity::DARK),
    (gerber::Polarity::CLEAR)
) {
    std::string    unitModeString = gerber::Polarity(enumValue).toString();
    gerber::Parser parser;
    auto           gerber_source = fmt::format("%LP{}*%", unitModeString);
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 1);
    auto node = std::dynamic_pointer_cast<gerber::LP>(nodes[0]);

    REQUIRE(node->getNodeName() == "LP");

    REQUIRE(node->polarity == enumValue);
}

// Other

TEST_CASE("Parse single coordinate", "[other]", ) {
    using tuple_t = std::tuple<const char*, const char*, const char*>;

    gerber::Parser parser;
    auto           params = GENERATE(
        tuple_t{"X01000000", "X", "01000000"},
        tuple_t{"Y01000002", "Y", "01000002"},
        tuple_t{"I01003200", "I", "01003200"},
        tuple_t{"J32000000", "J", "32000000"}
    );
    auto        result = parser.parse(std::get<0>(params));
    const auto& nodes  = result.getNodes();

    REQUIRE(nodes.size() == 1);
    auto node = std::dynamic_pointer_cast<gerber::Coordinate>(nodes[0]);

    REQUIRE(node->getNodeName() == std::get<1>(params));
    REQUIRE(node->getValue() == std::get<2>(params));
}

TEST_CASE("Two coordinates one after another", "[other]") {
    gerber::Parser parser;
    auto           gerber_source = "X01000000Y01000002";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 2);

    auto x = std::dynamic_pointer_cast<gerber::CoordinateX>(nodes[0]);
    auto y = std::dynamic_pointer_cast<gerber::CoordinateY>(nodes[1]);

    REQUIRE(x->getNodeName() == "X");
    REQUIRE(x->getValue() == "01000000");

    REQUIRE(y->getNodeName() == "Y");
    REQUIRE(y->getValue() == "01000002");
}

// Properties

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
