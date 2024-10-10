#include "gerber/ast/enums.hpp"
#include <stdexcept>
#include <string>
#include <string_view>

namespace gerber {

    Zeros::Zeros(Enum value) :
        value(value) {}

    Zeros Zeros::fromString(const std::string_view& str) {
        if (str == "L") {
            return Enum::SKIP_LEADING;
        } else if (str == "T") {
            return Enum::SKIP_TRAILING;
        }
        throw std::invalid_argument("Invalid zeros");
    }

    bool Zeros::operator==(const Zeros& other) const {
        return value == other.value;
    }

    bool Zeros::operator==(const Enum& other) const {
        return value == other;
    }

    CoordinateNotation::CoordinateNotation(Enum value) :
        value(value) {}

    CoordinateNotation CoordinateNotation::fromString(const std::string_view& str) {
        if (str == "A") {
            return Enum::ABSOLUTE;
        } else if (str == "I") {
            return Enum::INCREMENTAL;
        }
        throw std::invalid_argument("Invalid coordinate notation");
    }

    bool CoordinateNotation::operator==(const CoordinateNotation& other) const {
        return value == other.value;
    }

    bool CoordinateNotation::operator==(const Enum& other) const {
        return value == other;
    }

    UnitMode::UnitMode(Enum value) :
        value(value) {}

    UnitMode UnitMode::fromString(const std::string_view& str) {
        if (str == "IN") {
            return Enum::INCHES;
        } else if (str == "MM") {
            return Enum::MILLIMETERS;
        }
        throw std::invalid_argument("Invalid unit mode");
    }

    std::string UnitMode::toString() const {
        return value == Enum::INCHES ? "IN" : "MM";
    }

    bool UnitMode::operator==(const UnitMode& other) const {
        return value == other.value;
    }

    bool UnitMode::operator==(const Enum& other) const {
        return value == other;
    }
} // namespace gerber