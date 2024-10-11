#pragma once
#include "gerber/ast/aperture/AD.hpp"
#include <optional>
#include <string>

namespace gerber {
    class ADP : public AD {
      private:
        double                outerDiameter;
        double                verticesCount;
        std::optional<double> rotation;
        std::optional<double> holeDiameter;

      public:
        ADP(const std::string_view& apertureId,
            double                  outerDiameter,
            double                  verticesCount,
            std::optional<double>   rotation,
            std::optional<double>   holeDiameter);

        std::string           getNodeName() const override;
        double                getOuterDiameter() const;
        double                getVerticesCount() const;
        std::optional<double> getRotation() const;
        std::optional<double> getHoleDiameter() const;
    };
} // namespace gerber