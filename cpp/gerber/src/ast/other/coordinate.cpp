#include "gerber/ast/other/coordinate.hpp"

namespace gerber {
    Coordinate::Coordinate(const std::string_view& value) :
        value(value) {}

    std::string Coordinate::getNodeName() const {
        return "Coordinate";
    }

    std::string Coordinate::getValue() const {
        return value;
    }
} // namespace gerber