#include <pybind11/pybind11.h>
#include <string>
#include <vector>
#include <algorithm>


class BaseCommand {};

class Command: public BaseCommand {};


class ExtendedCommand: public BaseCommand {
};


class GerberParser {
public:
    std::vector<std::unique_ptr<BaseCommand>> commands;
    GerberParser() {}
    ~GerberParser() {}

    void parse(const std::string &gerber) {
        // In gerber usually each command is in a new line, so this can be
        // a good guess for our initial size of a vector.
        auto line_count = std::ranges::count(gerber, '\n');
        commands.clear();
        commands.reserve(line_count);
        // Parse the gerber code
    }
};


struct Pet {
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};

namespace py = pybind11;

PYBIND11_MODULE(pygerber_gerber_parser_cpp, m) {
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName);
}