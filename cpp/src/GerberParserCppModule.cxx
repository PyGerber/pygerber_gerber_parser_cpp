module;

#include <algorithm>
#include <cstdint>
#include <format>
#include <memory>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

export module GerberParserCppModule;

export namespace gerber {
    using location_t = uint64_t;
    using offset_t   = uint64_t;

    const std::tuple<location_t, location_t>
    get_line_column(const std::string_view& source, const location_t& index) {
        const location_t line_number = std::count(&source[0], &source[index], '\n');
        const location_t line_start  = source.rfind('\n', index) + 1;
        return std::make_tuple(line_number, index - line_start);
    }

    class Node {
      public:
        virtual std::string getNodeName() const {
            return "Node";
        }
    };

    class File : public Node {
      private:
        std::vector<std::shared_ptr<Node>> nodes;

      public:
        File(File&& other) :
            nodes(std::move(other.nodes)) {}

        File(std::vector<std::shared_ptr<Node>>&& nodes) :
            nodes(std::move(nodes)) {}

        std::vector<std::shared_ptr<Node>>& getNodes() {
            return nodes;
        }

        virtual std::string getNodeName() const {
            return "File";
        }
    };

    class Command : public Node {
      public:
        std::string getNodeName() const override {
            return "Command";
        }
    };

    class G01 : public Command {
      public:
        std::string getNodeName() const override {
            return "G01";
        }
    };

    class G02 : public Command {
      public:
        std::string getNodeName() const override {
            return "G02";
        }
    };

    class G03 : public Command {
      public:
        std::string getNodeName() const override {
            return "G03";
        }
    };

    class G04 : public Command {
      private:
        std::string comment;

      public:
        G04(std::string comment) :
            comment(comment) {}

        std::string getNodeName() const override {
            return "G04";
        }
    };

    class G36 : public Command {
      public:
        std::string getNodeName() const override {
            return "G36";
        }
    };

    class G37 : public Command {
      public:
        std::string getNodeName() const override {
            return "G37";
        }
    };

    class G54 : public Command {
      public:
        std::string getNodeName() const override {
            return "G54";
        }
    };

    class G55 : public Command {
      public:
        std::string getNodeName() const override {
            return "G55";
        }
    };

    class G70 : public Command {
      public:
        std::string getNodeName() const override {
            return "G70";
        }
    };

    class G71 : public Command {
      public:
        std::string getNodeName() const override {
            return "G71";
        }
    };

    class G74 : public Command {
      public:
        std::string getNodeName() const override {
            return "G74";
        }
    };

    class G75 : public Command {
      public:
        std::string getNodeName() const override {
            return "G75";
        }
    };

    class G90 : public Command {
      public:
        std::string getNodeName() const override {
            return "G90";
        }
    };

    class G91 : public Command {
      public:
        std::string getNodeName() const override {
            return "G91";
        }
    };

    class ExtendedCommand : public Node {
      public:
        std::string getNodeName() const override {
            return "ExtendedCommand";
        }
    };

    class SyntaxError : public std::runtime_error {
      public:
        explicit SyntaxError(const std::string& message) :
            std::runtime_error(message) {}
    };

    class Parser {
      public:
        std::vector<std::shared_ptr<Node>> commands;
        std::string_view                   full_source;
        location_t                         global_index;
        // Regular expressions cache
        std::regex                         g_code_regex;
        std::regex                         g04_regex;

        Parser() :
            commands(0),
            full_source(""),
            global_index(0),
            g_code_regex("^[Gg]0*([1-9][0-9]*)\\*"),
            g04_regex("^[Gg]0*4([^%*])\\*") {}

        ~Parser() {}

        File parse(const std::string& source) {
            full_source = source;

            // In gerber usually each command is in a new line, so this can be
            // a good guess for our initial size of a vector.
            auto line_count = std::ranges::count(full_source, '\n');
            commands.clear();
            commands.reserve(line_count);

            global_index = 0;

            while (global_index < (full_source.size() - 1)) {
                global_index += parse_global(source, global_index);
            }

            return File(std::move(commands));
        }

        location_t parse_global(const std::string_view& source, const location_t& index) {
            const std::string_view sub_source{source.begin() + index, source.end()};

            switch (sub_source[0]) {
                case 'G':
                    return parse_g_code(sub_source, index);
                    break;
                default:
                    break;
            }
            throw_syntax_error();
        }

        [[noreturn]] void throw_syntax_error() {
            const auto [line, column] = get_line_column(full_source, global_index);
            auto message              = std::format(
                "Syntax error at index {} (line: {} column: {})", global_index, line, column
            );

            throw SyntaxError(message);
        }

        offset_t parse_g_code(const std::string_view& gerber, const location_t& index) {
            std::cmatch match;

            bool result = std::regex_search(
                gerber.data(),
                gerber.data() + gerber.size(),
                match,
                g_code_regex,
                std::regex_constants::match_continuous
            );
            if (result && match.size() > 1) {
                const auto g_code_value = std::stoi(match[1].str());

                switch (g_code_value) {
                    case 1:
                        commands.push_back(std::make_shared<G01>());
                        return match.length();
                    case 2:
                        commands.push_back(std::make_shared<G02>());
                        return match.length();
                    case 3:
                        commands.push_back(std::make_shared<G03>());
                        return match.length();
                    case 4:
                        commands.push_back(std::make_shared<G04>(""));
                        return match.length();
                    case 36:
                        commands.push_back(std::make_shared<G36>());
                        return match.length();
                    case 37:
                        commands.push_back(std::make_shared<G37>());
                        return match.length();
                    case 54:
                        commands.push_back(std::make_shared<G54>());
                        return match.length();
                    case 55:
                        commands.push_back(std::make_shared<G55>());
                        return match.length();
                    case 70:
                        commands.push_back(std::make_shared<G70>());
                        return match.length();
                    case 71:
                        commands.push_back(std::make_shared<G71>());
                        return match.length();
                    case 74:
                        commands.push_back(std::make_shared<G74>());
                        return match.length();
                    case 75:
                        commands.push_back(std::make_shared<G75>());
                        return match.length();
                    case 90:
                        commands.push_back(std::make_shared<G90>());
                        return match.length();
                    case 91:
                        commands.push_back(std::make_shared<G91>());
                        return match.length();
                }
            }
            result = std::regex_search(
                gerber.data(),
                gerber.data() + gerber.size(),
                match,
                g_code_regex,
                std::regex_constants::match_continuous
            );
            if (result && match.size() > 1) {
                commands.push_back(std::make_shared<G04>(match[1].str()));
                return match.length();
            }

            throw_syntax_error();
        }
    };
} // namespace gerber
