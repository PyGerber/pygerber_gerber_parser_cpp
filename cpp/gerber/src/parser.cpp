#include "gerber/parser.hpp"
#include "gerber/ast/ast.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <memory>
#include <regex>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace gerber {

    const std::tuple<location_t, location_t>
    get_line_column(const std::string_view& source, const location_t& index) {
        const location_t line_number = std::count(&source[0], &source[index], '\n');
        const location_t line_start  = source.rfind('\n', index) + 1;
        return std::make_tuple(line_number, index - line_start);
    }

    Parser::Parser() :
        commands(0),
        full_source(""),
        global_index(0) {}

    File Parser::parse(const std::string& source) {
        full_source = source;

        // In gerber usually each command is in a new line, so this can be
        // a good guess for our initial size of a vector.
        auto line_count = std::count(full_source.begin(), full_source.end(), '\n');
        commands.clear();
        commands.reserve(line_count);

        global_index = 0;

        while (global_index < (full_source.size() - 1)) {
            global_index += parse_global(full_source, global_index);
        }

        return File(std::move(commands));
    }

    location_t Parser::parse_global(const std::string_view& source, const location_t& index) {
        const std::string_view sub_source{source.begin() + index, source.size() - index};

        if (sub_source.empty()) {
            return 0;
        }

        switch (sub_source[0]) {
            case 'G':
                return parse_g_code(sub_source, index);
                break;

            case ' ':
            case '\n':
            case '\r':
                return 1;
                break;

            case '%':
                return parse_extended_command(sub_source, index);
                break;

            default:
                break;
        }
        throw_syntax_error();
    }

    [[noreturn]] void Parser::throw_syntax_error() {
        const auto [line, column]  = get_line_column(full_source, global_index);
        const auto next_endl_index = full_source.find("\n", global_index);
        const auto next_endl_or_end_index =
            next_endl_index == std::string::npos ? full_source.size() : next_endl_index;

        const auto end_index =
            (next_endl_or_end_index - global_index) > 20 ? global_index + 20 : next_endl_index;
        const auto source_view = full_source.substr(global_index, end_index);

        auto message = fmt::format(
            "Syntax error at index {} (line: {} column: {}): '{}'",
            global_index,
            line,
            column,
            source_view
        );

        throw SyntaxError(message);
    }

    offset_t Parser::parse_g_code(const std::string_view& gerber, const location_t& index) {
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
            g04_regex,
            std::regex_constants::match_continuous
        );
        if (result && match.size() > 1) {
            commands.push_back(std::make_shared<G04>(match[1].str()));
            return match.length();
        }

        throw_syntax_error();
    }

    offset_t
    Parser::parse_extended_command(const std::string_view& source, const location_t& index) {
        // Shortest possible extended command is probably %TD*%, so 5 chars at least.
        if (source.length() < 5) {
            throw_syntax_error();
        }

        switch (source[1]) {
            case 'F':
                return parse_fs_command(source, index);
                break;

            case 'M':
                return parse_mo_command(source, index);
                break;

            default:
                break;
        }

        throw_syntax_error();
    }

    offset_t Parser::parse_fs_command(const std::string_view& source, const location_t& index) {
        if (source.length() < 13) {
            throw_syntax_error();
        }
        std::cmatch match;

        const auto result = std::regex_search(
            source.data(),
            source.data() + source.size(),
            match,
            fs_regex,
            std::regex_constants::match_continuous
        );
        if (result && match.size() == 7) {
            commands.push_back(std::make_shared<FS>(
                match[1].str(),
                match[2].str(),
                std::stoi(match[3].str()),
                std::stoi(match[4].str()),
                std::stoi(match[5].str()),
                std::stoi(match[6].str())
            ));
            return match.length();
        }
        throw_syntax_error();
    }

    offset_t Parser::parse_mo_command(const std::string_view& source, const location_t& index) {
        if (source.length() < 6) {
            throw_syntax_error();
        }
        std::cmatch match;

        const auto result = std::regex_search(
            source.data(),
            source.data() + source.size(),
            match,
            mo_regex,
            std::regex_constants::match_continuous
        );
        if (result && match.size() == 2) {
            commands.push_back(std::make_shared<MO>(match[1].str()));
            return match.length();
        }
        throw_syntax_error();
    }
} // namespace gerber