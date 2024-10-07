module;

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

export module GerberParserCppModule;

export class Node {};

export class File : public Node {
  private:
    std::vector<std::unique_ptr<Node>> nodes;

  public:
    File(File&& other) : nodes(std::move(other.nodes)) {}

    File(std::vector<std::unique_ptr<Node>>&& nodes) :
        nodes(std::move(nodes)) {}

    const std::vector<std::unique_ptr<Node>>& getNodes() const {
        return nodes;
    }
};

export class Command : public Node {};

export class G01 : public Command {};

export class ExtendedCommand : public Node {};

export class GerberParser {
  public:
    std::vector<std::unique_ptr<Node>> commands;

    GerberParser() {}

    ~GerberParser() {}

    File parse(const std::string& gerber) {
        // In gerber usually each command is in a new line, so this can be
        // a good guess for our initial size of a vector.
        auto line_count = std::ranges::count(gerber, '\n');
        commands.clear();
        commands.reserve(line_count);
        parse_global(gerber);

        return File(std::move(commands));
    }

    void parse_global(const std::string& gerber) {
        uint64_t index = 0;

        switch (gerber[index]) {
            case 'G':
                parse_g_code(gerber, index);
                break;
            default:
                break;
        }
    }

    void parse_g_code(const std::string& gerber, uint64_t& index) {}
};
