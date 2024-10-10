#include "gerber/ast/g_codes/G91.hpp"

namespace gerber {
    std::string G91::getNodeName() const {
        return "G91";
    }
} // namespace gerber