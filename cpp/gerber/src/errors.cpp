#include "gerber/errors.hpp"

namespace gerber {
    SyntaxError::SyntaxError(const std::string& message) :
        std::runtime_error(message) {}
} // namespace gerber