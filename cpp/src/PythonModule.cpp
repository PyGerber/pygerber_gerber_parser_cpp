import GerberParserCppModule;

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace gbr = gerber;

PYBIND11_MODULE(gerber_parser, m) {
    py::register_exception<gbr::SyntaxError>(m, "SyntaxError", PyExc_RuntimeError);

    py::class_<gbr::Node, std::shared_ptr<gbr::Node>>(m, "Node").def(py::init<>());

    py::class_<gbr::File>(m, "File").def_property_readonly("nodes", &gbr::File::getNodes);

    py::class_<gbr::Command>(m, "Command").def(py::init<>());

    py::class_<gbr::G01, std::shared_ptr<gbr::G01>>(m, "G01")
        .def(py::init<>())
        .def("__str__", &gbr::G01::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g01")(self);
        });

    py::class_<gbr::G02, std::shared_ptr<gbr::G02>>(m, "G02")
        .def(py::init<>())
        .def("__str__", &gbr::G02::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g02")(self);
        });

    py::class_<gbr::G03, std::shared_ptr<gbr::G03>>(m, "G03")
        .def(py::init<>())
        .def("__str__", &gbr::G03::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g03")(self);
        });

    py::class_<gbr::G04, std::shared_ptr<gbr::G04>>(m, "G04")
        .def(py::init<std::string>())
        .def("__str__", &gbr::G04::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g04")(self);
        });

    py::class_<gbr::G36, std::shared_ptr<gbr::G36>>(m, "G36")
        .def(py::init<>())
        .def("__str__", &gbr::G36::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g36")(self);
        });

    py::class_<gbr::G37, std::shared_ptr<gbr::G37>>(m, "G37")
        .def(py::init<>())
        .def("__str__", &gbr::G37::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g37")(self);
        });

    py::class_<gbr::G54, std::shared_ptr<gbr::G54>>(m, "G54")
        .def(py::init<>())
        .def("__str__", &gbr::G54::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g54")(self);
        });

    py::class_<gbr::G55, std::shared_ptr<gbr::G55>>(m, "G55")
        .def(py::init<>())
        .def("__str__", &gbr::G55::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g55")(self);
        });

    py::class_<gbr::G70, std::shared_ptr<gbr::G70>>(m, "G70")
        .def(py::init<>())
        .def("__str__", &gbr::G70::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g70")(self);
        });

    py::class_<gbr::G71, std::shared_ptr<gbr::G71>>(m, "G71")
        .def(py::init<>())
        .def("__str__", &gbr::G71::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g71")(self);
        });

    py::class_<gbr::G74, std::shared_ptr<gbr::G74>>(m, "G74")
        .def(py::init<>())
        .def("__str__", &gbr::G74::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g74")(self);
        });

    py::class_<gbr::G75, std::shared_ptr<gbr::G75>>(m, "G75")
        .def(py::init<>())
        .def("__str__", &gbr::G75::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g75")(self);
        });

    py::class_<gbr::G90, std::shared_ptr<gbr::G90>>(m, "G90")
        .def(py::init<>())
        .def("__str__", &gbr::G90::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g90")(self);
        });

    py::class_<gbr::G91, std::shared_ptr<gbr::G91>>(m, "G91")
        .def(py::init<>())
        .def("__str__", &gbr::G91::getNodeName)
        .def("visit", [](py::object self, py::object visitor) {
            return visitor.attr("on_g91")(self);
        });

    py::class_<gbr::Parser>(m, "GerberParser").def(py::init<>()).def("parse", &gbr::Parser::parse);
}