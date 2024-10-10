#pragma once
#include "gerber/ast/extended_command.hpp"
#include "gerber/ast/enums.hpp"
#include <string>
#include <string_view>

namespace gerber {
    class FS : public ExtendedCommand {
      public:
        Zeros              zeros;
        CoordinateNotation coordinate_mode;

        int x_integral;
        int x_decimal;

        int y_integral;
        int y_decimal;

      public:
        FS(const std::string_view& zeros,
           const std::string_view& coordinate_mode,
           int                     x_integral,
           int                     x_decimal,
           int                     y_integral,
           int                     y_decimal);

        std::string getNodeName() const override;
    };
} // namespace gerber