#include "gerber/ast/g_codes/G04.hpp"
#include <string>

namespace gerber {
    G04::G04(std::string comment) :
        comment(comment) {}

    std::string G04::getNodeName() const {
        return "G04";
    }
} // namespace gerber