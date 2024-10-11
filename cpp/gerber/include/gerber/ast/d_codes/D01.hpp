#pragma once
#include "gerber/ast/command.hpp"
#include <string>

namespace gerber {
    class D01 : public Command {
      public:
        std::string getNodeName() const override;
    };
} // namespace gerber