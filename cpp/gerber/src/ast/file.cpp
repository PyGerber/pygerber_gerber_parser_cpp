#include "gerber/ast/file.hpp"
#include <memory>
#include <string>
#include <vector>

namespace gerber {
    File::File(File&& other) :
        nodes(std::move(other.nodes)) {}

    File::File(std::vector<std::shared_ptr<Node>>&& nodes) :
        nodes(std::move(nodes)) {}

    std::vector<std::shared_ptr<Node>>& File::getNodes() {
        return nodes;
    }

    std::string File::getNodeName() const {
        return "File";
    }
} // namespace gerber