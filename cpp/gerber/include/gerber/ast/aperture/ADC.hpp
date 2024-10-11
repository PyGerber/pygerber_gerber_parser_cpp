#pragma once
#include "gerber/ast/aperture/AD.hpp"
#include <optional>
#include <string>

namespace gerber {
    class ADC : public AD {
      private:
        double                diameter;
        std::optional<double> holeDiameter;

      public:
        ADC(const std::string_view& apertureId, double diameter, std::optional<double> holeDiameter
        );
        std::string           getNodeName() const override;
        double                getDiameter() const;
        std::optional<double> getHoleDiameter() const;
    };
} // namespace gerber