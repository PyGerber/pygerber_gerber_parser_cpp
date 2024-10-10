#pragma once
#include "gerber/ast/node.hpp"
#include <string>

namespace gerber {
    class Command : public Node {
      public:
        std::string getNodeName() const override;
    };
} // namespace gerber