import GerberParserCppModule;

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(gerber_parser, m) {
    py::class_<Node>(m, "Node").def(py::init<>());

    py::class_<File>(m, "File").def_property_readonly("nodes", &File::getNodes);

    py::class_<Command>(m, "Command").def(py::init<>());

    py::class_<G01>(m, "G01").def(py::init<>());

    py::class_<GerberParser>(m, "GerberParser")
        .def(py::init<>())
        .def("parse", &GerberParser::parse)
        .def_readonly("commands", &GerberParser::commands);
}