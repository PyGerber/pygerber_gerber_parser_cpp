#include "gerber/gerber.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Basic Gerber #0", "[multi_node]") {
    gerber::Parser parser;
    auto           gerber_source = R"(
        %FSLAX24Y24*%
        %MOIN*%
        %ADD10C,0.5*%
        D10*
        X100000Y100000D02*
        X200000Y200000D01*
        M02*
    )";
    auto           result        = parser.parse(gerber_source);
    const auto&    nodes         = result.getNodes();

    REQUIRE(nodes.size() == 11);
    // Additional checks can be added here based on the expected nodes
}