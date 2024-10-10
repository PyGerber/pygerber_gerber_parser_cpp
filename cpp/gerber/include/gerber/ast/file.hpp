#pragma once
#include "gerber/ast/node.hpp"
#include <memory>
#include <string>
#include <vector>

namespace gerber {
    class File : public Node {
      private:
        std::vector<std::shared_ptr<Node>> nodes;

      public:
        File(File&& other);
        File(std::vector<std::shared_ptr<Node>>&& nodes);
        std::vector<std::shared_ptr<Node>>& getNodes();
        virtual std::string                 getNodeName() const;
    };
} // namespace gerber