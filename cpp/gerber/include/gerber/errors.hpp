#pragma once
#include <stdexcept>
#include <string>

namespace gerber {
    class SyntaxError : public std::runtime_error {
      public:
        explicit SyntaxError(const std::string& message);
    };
} // namespace gerber