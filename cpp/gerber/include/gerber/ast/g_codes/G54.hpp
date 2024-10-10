#pragma once
#include "gerber/ast/command.hpp"
#include <string>

namespace gerber {
    class G54 : public Command {
      public:
        std::string getNodeName() const override;
    };
} // namespace gerber