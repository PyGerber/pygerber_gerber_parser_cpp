#pragma once
#include "gerber/ast/extended_command.hpp"
#include "gerber/ast/enums.hpp"
#include <string>
#include <string_view>

namespace gerber {
    class MO : public ExtendedCommand {
      public:
        UnitMode unit_mode;

      public:
        MO(const std::string_view& unit_mode);
        std::string getNodeName() const override;
    };
}