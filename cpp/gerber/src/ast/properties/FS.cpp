#include "gerber/ast/properties/FS.hpp"

namespace gerber {
    FS::FS(
        const std::string_view& zeros,
        const std::string_view& coordinate_mode,
        int                     x_integral,
        int                     x_decimal,
        int                     y_integral,
        int                     y_decimal
    ) :
        zeros(Zeros::fromString(zeros)),
        coordinate_mode(CoordinateNotation::fromString(coordinate_mode)),
        x_integral(x_integral),
        x_decimal(x_decimal),
        y_integral(y_integral),
        y_decimal(y_decimal) {}

    std::string FS::getNodeName() const {
        return "FS";
    }
} // namespace gerber