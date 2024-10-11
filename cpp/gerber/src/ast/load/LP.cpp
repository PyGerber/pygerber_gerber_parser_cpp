#include "gerber/ast/load/LP.hpp"
#include "gerber/ast/enums.hpp"
#include <string>

namespace gerber {
    LP::LP(const char polarity) :
        polarity(Polarity::fromString(polarity)) {}

    std::string LP::getNodeName() const {
        return "LP";
    }
} // namespace gerber