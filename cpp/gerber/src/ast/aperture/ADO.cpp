
#include "gerber/ast/aperture/ADO.hpp"

namespace gerber {
    ADO::ADO(
        const std::string_view& apertureId_,
        double                  width_,
        double                  height_,
        std::optional<double>   holeDiameter_
    ) :
        AD(apertureId_),
        width(width_),
        height(height_),
        holeDiameter(holeDiameter_) {}

    std::string ADO::getNodeName() const {
        return "ADO";
    }

    double ADO::getWidth() const {
        return width;
    }

    double ADO::getHeight() const {
        return height;
    }

    std::optional<double> ADO::getHoleDiameter() const {
        return holeDiameter;
    }
} // namespace gerber