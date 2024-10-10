#pragma once
#include "gerber/ast/command.hpp"
#include <string>

namespace gerber {
    class G04 : public Command {
      private:
        std::string comment;

      public:
        G04(std::string comment);

        std::string getNodeName() const override;
    };
} // namespace gerber