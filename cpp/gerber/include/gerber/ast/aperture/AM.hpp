#pragma once
#include "gerber/ast/aperture/AMclose.hpp"
#include "gerber/ast/aperture/AMopen.hpp"
#include "gerber/ast/command.hpp"
#include "gerber/ast/extended_command.hpp"
#include <memory>
#include <string>
#include <vector>

namespace gerber {
    class AM : public ExtendedCommand {
      private:
        std::shared_ptr<AMopen>               amOpen;
        std::vector<std::shared_ptr<Command>> primitives;
        std::shared_ptr<AMclose>              amClose;

      public:
        using primitive_t            = Command;
        using primitives_container_t = std::vector<std::shared_ptr<primitive_t>>;

        AM(std::shared_ptr<AMopen>  amOpen,
           primitives_container_t   primitives,
           std::shared_ptr<AMclose> amClose);

        std::string              getNodeName() const override;
        std::shared_ptr<AMopen>  getAmOpen() const;
        primitives_container_t   getPrimitives() const;
        std::shared_ptr<AMclose> getAmClose() const;
    };
} // namespace gerber