import GerberParserCppModule;

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace gbr = gerber;

PYBIND11_MODULE(gerber_parser, m) {
    py::class_<gbr::Node>(m, "Node").def(py::init<>());

    py::class_<gbr::File>(m, "File").def_property_readonly("nodes", &gbr::File::getNodes);

    py::class_<gbr::Command>(m, "Command").def(py::init<>());

    py::class_<gbr::G01>(m, "G01").def(py::init<>());
    py::class_<gbr::G02>(m, "G02").def(py::init<>());
    py::class_<gbr::G03>(m, "G03").def(py::init<>());
    py::class_<gbr::G04>(m, "G04").def(py::init<std::string>());
    py::class_<gbr::G36>(m, "G36").def(py::init<>());
    py::class_<gbr::G37>(m, "G37").def(py::init<>());
    py::class_<gbr::G70>(m, "G70").def(py::init<>());
    py::class_<gbr::G71>(m, "G71").def(py::init<>());
    py::class_<gbr::G74>(m, "G74").def(py::init<>());
    py::class_<gbr::G75>(m, "G75").def(py::init<>());
    py::class_<gbr::G90>(m, "G90").def(py::init<>());
    py::class_<gbr::G91>(m, "G91").def(py::init<>());

    py::class_<gbr::Parser>(m, "GerberParser")
        .def(py::init<>())
        .def("parse", &gbr::Parser::parse)
        .def_readonly("commands", &gbr::Parser::commands);
}