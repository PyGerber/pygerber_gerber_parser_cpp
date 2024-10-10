#pragma once
#include <string>

namespace gerber {
    class Node {
      public:
        virtual std::string getNodeName() const;
    };
} // namespace gerber