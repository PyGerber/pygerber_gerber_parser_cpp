#pragma once
#include "gerber/ast/command.hpp"
#include <string>

namespace gerber {
    class Coordinate : public Command {
      private:
        std::string value;

        Coordinate() = delete;

      public:
        Coordinate(const std::string_view& value);
        std::string getNodeName() const override;
        std::string getValue() const;
    };
} // namespace gerber