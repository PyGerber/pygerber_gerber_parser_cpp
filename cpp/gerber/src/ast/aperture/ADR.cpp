
#include "gerber/ast/aperture/ADR.hpp"

namespace gerber {
    ADR::ADR(
        const std::string_view& apertureId_,
        double                  width_,
        double                  height_,
        std::optional<double>   holeDiameter_
    ) :
        AD(apertureId_),
        width(width_),
        height(height_),
        holeDiameter(holeDiameter_) {}

    std::string ADR::getNodeName() const {
        return "ADR";
    }

    double ADR::getWidth() const {
        return width;
    }

    double ADR::getHeight() const {
        return height;
    }

    std::optional<double> ADR::getHoleDiameter() const {
        return holeDiameter;
    }
} // namespace gerber