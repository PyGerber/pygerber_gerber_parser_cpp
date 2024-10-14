#pragma once
#include "gerber/ast/node.hpp"
#include <string>

namespace gerber {
    class AMopen: public Node {
      private:
        std::string apertureId;

        AMopen() = delete;

      public:
        AMopen(const std::string_view& apertureId);
        std::string getNodeName() const override;
        std::string getApertureId() const;
    };
} // namespace gerber