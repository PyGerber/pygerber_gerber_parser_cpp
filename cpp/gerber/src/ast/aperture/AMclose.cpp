#include "gerber/ast/aperture/AMclose.hpp"

namespace gerber {
    AMclose::AMclose() {}

    std::string AMclose::getNodeName() const {
        return "AMclose";
    }
} // namespace gerber