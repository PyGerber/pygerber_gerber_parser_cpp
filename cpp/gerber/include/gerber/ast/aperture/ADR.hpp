#pragma once
#include "gerber/ast/aperture/AD.hpp"
#include <optional>
#include <string>

namespace gerber {
    class ADR : public AD {
      private:
        double                width;
        double                height;
        std::optional<double> holeDiameter;

      public:
        ADR(const std::string_view& apertureId,
            double                  width,
            double                  height,
            std::optional<double>   holeDiameter);

        std::string           getNodeName() const override;
        double                getWidth() const;
        double                getHeight() const;
        std::optional<double> getHoleDiameter() const;
    };
} // namespace gerber