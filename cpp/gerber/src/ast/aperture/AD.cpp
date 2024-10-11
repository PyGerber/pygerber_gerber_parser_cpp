#include "gerber/ast/aperture/AD.hpp"

namespace gerber {
    AD::AD(const std::string_view& apertureId_) :
        apertureId(apertureId_) {}

    std::string AD::getNodeName() const {
        return "AD";
    }

    std::string AD::getApertureId() const {
        return apertureId;
    }
} // namespace gerber