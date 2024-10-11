
#include "gerber/ast/aperture/ADC.hpp"

namespace gerber {
    ADC::ADC(
        const std::string_view& apertureId_, double diameter_, std::optional<double> holeDiameter_
    ) :
        AD(apertureId_),
        diameter(diameter_),
        holeDiameter(holeDiameter_) {}

    std::string ADC::getNodeName() const {
        return "ADC";
    }

    double ADC::getDiameter() const {
        return diameter;
    }

    std::optional<double> ADC::getHoleDiameter() const {
        return holeDiameter;
    }
} // namespace gerber