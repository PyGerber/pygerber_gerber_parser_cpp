#include "gerber/ast/d_codes/Dnn.hpp"

namespace gerber {
    Dnn::Dnn(const std::string_view& aperture_id_) :
        aperture_id(aperture_id_) {}

    std::string Dnn::getNodeName() const {
        return "Dnn";
    }

    std::string Dnn::getApertureId() const {
        return aperture_id;
    }
} // namespace gerber