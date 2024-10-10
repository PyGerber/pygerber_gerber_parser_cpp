#include "gerber/ast/g_codes/G90.hpp"

namespace gerber {
    std::string G90::getNodeName() const {
        return "G90";
    }
} // namespace gerber