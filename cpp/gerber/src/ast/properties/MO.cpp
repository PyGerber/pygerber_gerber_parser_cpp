#include "gerber/ast/properties/MO.hpp"
#include "gerber/ast/enums.hpp"
#include <string>
#include <string_view>

namespace gerber {
    MO::MO(const std::string_view& unit_mode) :
        unit_mode(UnitMode::fromString(unit_mode)) {}

    std::string MO::getNodeName() const {
        return "MO";
    }
} // namespace gerber