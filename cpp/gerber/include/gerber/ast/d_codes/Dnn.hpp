#pragma once
#include "gerber/ast/command.hpp"
#include <string>

namespace gerber {
    class Dnn : public Command {
        std::string aperture_id;

      public:
        Dnn(const std::string_view& aperture_id_);
        std::string getNodeName() const override;
        std::string getApertureId() const;
    };
} // namespace gerber