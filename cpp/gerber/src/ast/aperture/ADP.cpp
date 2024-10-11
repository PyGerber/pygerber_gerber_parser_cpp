
#include "gerber/ast/aperture/ADP.hpp"

namespace gerber {
    ADP::ADP(
        const std::string_view& apertureId,
        double                  outerDiameter,
        double                  verticesCount,
        std::optional<double>   rotation,
        std::optional<double>   holeDiameter
    ) :
        AD(apertureId),
        outerDiameter(outerDiameter),
        verticesCount(verticesCount),
        rotation(rotation),
        holeDiameter(holeDiameter) {}

    std::string ADP::getNodeName() const {
        return "ADP";
    }

    double ADP::getOuterDiameter() const {
        return outerDiameter;
    }

    double ADP::getVerticesCount() const {
        return verticesCount;
    }

    std::optional<double> ADP::getRotation() const {
        return rotation;
    }

    std::optional<double> ADP::getHoleDiameter() const {
        return holeDiameter;
    }
} // namespace gerber