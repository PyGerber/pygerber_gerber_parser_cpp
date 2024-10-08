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

    class Node {};

    class File : public Node {
      private:
        std::vector<std::unique_ptr<Node>> nodes;

      public:
        File(File&& other) :
            nodes(std::move(other.nodes)) {}

        File(std::vector<std::unique_ptr<Node>>&& nodes) :
            nodes(std::move(nodes)) {}

        const std::vector<std::unique_ptr<Node>>& getNodes() const {
            return nodes;
        }
    };

    class Command : public Node {};

    class G01 : public Command {};

    class G02 : public Command {};

    class G03 : public Command {};

    class G04 : public Command {
      private:
        std::string comment;

      public:
        G04(std::string comment) :
            comment(comment) {}
    };

    class G36 : public Command {};

    class G37 : public Command {};

    class G70 : public Command {};

    class G71 : public Command {};

    class G74 : public Command {};

    class G75 : public Command {};

    class G90 : public Command {};

    class G91 : public Command {};

    class ExtendedCommand : public Node {};

    class SyntaxError : public std::runtime_error {
      public:
        explicit SyntaxError(const std::string& message) :
            std::runtime_error(message) {}
    };

    class Parser {
      public:
        std::vector<std::unique_ptr<Node>> commands;
        std::string_view                   full_source;
        location_t                         global_index;
        // Regular expressions cache
        std::regex                         g_code;

        Parser() :
            g_code("^[Gg]0*([1-9][0-9]*)\\*") {}

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
                g_code,
                std::regex_constants::match_continuous
            );
            if (result && match.size() > 1) {
                const auto g_code_value = std::stoi(match[1].str());

                switch (g_code_value) {
                    case 1:
                        commands.push_back(std::make_unique<G01>());
                        return match.length();

                    case 2:
                        commands.push_back(std::make_unique<G02>());
                        return match.length();

                    default:
                        break;
                }
            }

            throw_syntax_error();
        }
    };
} // namespace gerber
