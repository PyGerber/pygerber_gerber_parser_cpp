#pragma once
#include "gerber/ast/node.hpp"
#include <string>

namespace gerber {
    class ExtendedCommand : public Node {
      public:
        std::string getNodeName() const override;
    };
} // namespace gerber