#pragma once
#include "gerber/ast/enums.hpp"
#include "gerber/ast/extended_command.hpp"
#include <string>

namespace gerber {
    class LP : public ExtendedCommand {
      public:
        Polarity polarity;

      public:
        LP(const char polarity);
        std::string getNodeName() const override;
    };
} // namespace gerber