#include "gerber/ast/aperture/AM.hpp"

namespace gerber {
    AM::AM(
        std::shared_ptr<AMopen>    amOpen_,
        AM::primitives_container_t primitives_,
        std::shared_ptr<AMclose>   amClose_
    ) :
        amOpen(amOpen_),
        primitives(primitives_),
        amClose(amClose_) {}

    std::string AM::getNodeName() const {
        return "AM";
    }

    std::shared_ptr<AMopen> AM::getAmOpen() const {
        return amOpen;
    }

    AM::primitives_container_t AM::getPrimitives() const {
        return primitives;
    }

    std::shared_ptr<AMclose> AM::getAmClose() const {
        return amClose;
    }
} // namespace gerber