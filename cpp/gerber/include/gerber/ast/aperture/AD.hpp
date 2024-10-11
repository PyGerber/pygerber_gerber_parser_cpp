#pragma once
#include "gerber/ast/command.hpp"
#include <string>

namespace gerber {
    class AD : public Command {
      private:
        std::string apertureId;

        AD() = delete;

      public:
        AD(const std::string_view& apertureId);
        std::string getNodeName() const override;
        std::string getApertureId() const;
    };
} // namespace gerber