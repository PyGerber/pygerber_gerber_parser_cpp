#pragma once
#include "./coordinate.hpp"
#include <string>

namespace gerber {
    class CoordinateX : public Coordinate {
      public:
        using Coordinate::Coordinate;
        std::string getNodeName() const override;
    };
} // namespace gerber