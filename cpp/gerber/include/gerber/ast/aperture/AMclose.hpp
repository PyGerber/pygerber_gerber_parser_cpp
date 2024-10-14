#pragma once
#include "gerber/ast/node.hpp"
#include <string>

namespace gerber {
    class AMclose : public Node {
      public:
        AMclose();
        std::string getNodeName() const override;
    };
} // namespace gerber