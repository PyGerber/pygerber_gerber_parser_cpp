#include "fmt/format.h"
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

// Aperture

TEST_CASE("Parse ADC", "[aperture]") {
    gerber::Parser parser;
    auto           gerber_source = "%ADD10C,0.5*%";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 1);
    auto adc = std::dynamic_pointer_cast<gerber::ADC>(nodes[0]);

    REQUIRE(adc->getNodeName() == "ADC");

    REQUIRE(adc->getApertureId() == "10");
    REQUIRE(adc->getDiameter() == 0.5);
    REQUIRE_FALSE(adc->getHoleDiameter().has_value());
}

TEST_CASE("Parse ADC with hole", "[aperture]") {
    gerber::Parser parser;
    auto           gerber_source = "%ADD10C,0.5X0.1*%";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 1);
    auto adc = std::dynamic_pointer_cast<gerber::ADC>(nodes[0]);

    REQUIRE(adc->getNodeName() == "ADC");

    REQUIRE(adc->getApertureId() == "10");
    REQUIRE(adc->getDiameter() == 0.5);
    REQUIRE(adc->getHoleDiameter().has_value());
    REQUIRE(adc->getHoleDiameter().value() == 0.1);
}

TEST_CASE("Parse ADR", "[aperture]") {
    gerber::Parser parser;
    auto           gerber_source = "%ADD10R,0.5X0.5*%";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 1);
    auto adr = std::dynamic_pointer_cast<gerber::ADR>(nodes[0]);

    REQUIRE(adr->getNodeName() == "ADR");

    REQUIRE(adr->getApertureId() == "10");
    REQUIRE(adr->getWidth() == 0.5);
    REQUIRE(adr->getHeight() == 0.5);
    REQUIRE_FALSE(adr->getHoleDiameter().has_value());
}

TEST_CASE("Parse ADR with hole", "[aperture]") {
    gerber::Parser parser;
    auto           gerber_source = "%ADD10R,0.5X0.5X0.1*%";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 1);
    auto adr = std::dynamic_pointer_cast<gerber::ADR>(nodes[0]);

    REQUIRE(adr->getNodeName() == "ADR");

    REQUIRE(adr->getApertureId() == "10");
    REQUIRE(adr->getWidth() == 0.5);
    REQUIRE(adr->getHeight() == 0.5);
    REQUIRE(adr->getHoleDiameter().has_value());
    REQUIRE(adr->getHoleDiameter().value() == 0.1);
}

TEST_CASE("Parse ADO", "[aperture]") {
    gerber::Parser parser;
    auto           gerber_source = "%ADD10O,0.5X0.5*%";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 1);
    auto ado = std::dynamic_pointer_cast<gerber::ADO>(nodes[0]);

    REQUIRE(ado->getNodeName() == "ADO");

    REQUIRE(ado->getApertureId() == "10");
    REQUIRE(ado->getWidth() == 0.5);
    REQUIRE(ado->getHeight() == 0.5);
    REQUIRE_FALSE(ado->getHoleDiameter().has_value());
}

TEST_CASE("Parse ADO with hole", "[aperture]") {
    gerber::Parser parser;
    auto           gerber_source = "%ADD10O,0.5X0.5X0.1*%";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 1);
    auto ado = std::dynamic_pointer_cast<gerber::ADO>(nodes[0]);

    REQUIRE(ado->getNodeName() == "ADO");

    REQUIRE(ado->getApertureId() == "10");
    REQUIRE(ado->getWidth() == 0.5);
    REQUIRE(ado->getHeight() == 0.5);
    REQUIRE(ado->getHoleDiameter().has_value());
    REQUIRE(ado->getHoleDiameter().value() == 0.1);
}

TEST_CASE("Parse ADP", "[aperture]") {
    gerber::Parser parser;
    auto           gerber_source = "%ADD10P,0.5X5*%";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 1);
    auto adp = std::dynamic_pointer_cast<gerber::ADP>(nodes[0]);

    REQUIRE(adp->getNodeName() == "ADP");

    REQUIRE(adp->getApertureId() == "10");
    REQUIRE(adp->getOuterDiameter() == 0.5);
    REQUIRE(adp->getVerticesCount() == 5.0);
    REQUIRE_FALSE(adp->getRotation().has_value());
    REQUIRE_FALSE(adp->getHoleDiameter().has_value());
}

TEST_CASE("Parse ADP with hole", "[aperture]") {
    gerber::Parser parser;
    auto           gerber_source = "%ADD10P,0.5X5X0X0.1*%";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 1);
    auto adp = std::dynamic_pointer_cast<gerber::ADP>(nodes[0]);

    REQUIRE(adp->getNodeName() == "ADP");

    REQUIRE(adp->getApertureId() == "10");
    REQUIRE(adp->getOuterDiameter() == 0.5);
    REQUIRE(adp->getVerticesCount() == 5.0);
    REQUIRE(adp->getRotation().has_value());
    REQUIRE(adp->getRotation().value() == 0.0);
    REQUIRE(adp->getHoleDiameter().has_value());
    REQUIRE(adp->getHoleDiameter().value() == 0.1);
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


// M codes

TEMPLATE_TEST_CASE_SIG(
    "Parse M codes",
    "[m_codes]",
    ((typename T, int code), T, code),
    (gerber::M02, 2)
) {
    gerber::Parser parser;
    auto           gerber_source = fmt::format("M{}*M0{}*M00{}*M000{}*", code, code, code, code);
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 4);

    for (const auto& node : nodes) {
        REQUIRE(node->getNodeName() == fmt::format("M{:0>2}", code));
    }
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
