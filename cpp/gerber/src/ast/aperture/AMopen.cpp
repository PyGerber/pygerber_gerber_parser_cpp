#include "gerber/ast/aperture/AMopen.hpp"

namespace gerber {
    AMopen::AMopen(const std::string_view& apertureId_) :
        apertureId(apertureId_) {}

    std::string AMopen::getNodeName() const {
        return "AMopen";
    }

    std::string AMopen::getApertureId() const {
        return apertureId;
    }
} // namespace gerber