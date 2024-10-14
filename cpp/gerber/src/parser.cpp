#include "gerber/parser.hpp"
#include "gerber/ast/ast.hpp"
#include "gerber/ast/command.hpp"
#include "gerber/ast/m_codes/M02.hpp"
#include <algorithm>
#include <cassert>
#include <fmt/format.h>
#include <memory>
#include <optional>
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
            global_index += parse_global(full_source.substr(global_index), global_index);
        }

        return File(std::move(commands));
    }

    location_t Parser::parse_global(const std::string_view& source, const location_t& index) {
        if (source.empty()) {
            return 0;
        }

        switch (source[0]) {
            case ' ':
            case '\n':
            case '\r':
                return 1;
                break;

            case 'G':
                return parse_g_code(source, index);
                break;

            case 'D':
                return parse_d_code(source, index);
                break;

            case 'X':
                return parse_coordinate<CoordinateX>(source, index);
            case 'Y':
                return parse_coordinate<CoordinateY>(source, index);
            case 'I':
                return parse_coordinate<CoordinateI>(source, index);
            case 'J':
                return parse_coordinate<CoordinateJ>(source, index);

            case '%':
                return parse_extended_command(source, index);
                break;

            case 'M':
                return parse_m_code(source);
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

    offset_t Parser::parse_aperture(const std::string_view& source) {
        // Shortest possible aperture related node is %AB*%, so 5 chars at least.
        if (source.length() < 5) {
            throw_syntax_error();
        }

        switch (source[2]) {
            case 'D':
                return parse_aperture_definition(source);
                break;

            case 'M':
                return parse_aperture_macro(source);
                break;

            default:
                break;
        }

        throw_syntax_error();
    }

    offset_t Parser::parse_aperture_definition(const std::string_view& source) {
        std::cmatch match;

        const auto result = std::regex_search(
            source.data(),
            source.data() + source.size(),
            match,
            ad_header_regex,
            std::regex_constants::match_continuous
        );

        auto offset = 0;

        if (result && match.size() == 3) {
            const auto aperture_id   = match[1].str();
            const auto template_name = match[2].str();
            offset += match.length();

            if (template_name.length() == 0) {
                throw_syntax_error();
            }
            if (template_name.length() == 1) {
                assert(source.length() > match.length());

                char             standard_template_symbol = template_name[0];
                std::string_view rest                     = source.substr(match.length());

                if (rest.empty()) {
                    throw_syntax_error();
                }

                switch (standard_template_symbol) {
                    case 'C':
                        return offset + parse_standard_aperture_c_tail(rest, aperture_id);
                    case 'R':
                        return offset + parse_standard_aperture_r_like_tail<ADR>(rest, aperture_id);
                    case 'O':
                        return offset + parse_standard_aperture_r_like_tail<ADO>(rest, aperture_id);
                    case 'P':
                        return offset + parse_standard_aperture_p_tail(rest, aperture_id);
                    default:
                        throw_syntax_error();
                }
            }
            // Handling of macros not implemented.
            throw_syntax_error();
        }
        throw_syntax_error();
    }

    offset_t Parser::parse_standard_aperture_c_tail(
        const std::string_view& source, const std::string& aperture_id
    ) {
        std::string_view rest   = source;
        offset_t         offset = 0;

        auto diameter = consume_float(rest, offset);

        std::optional<double> holeDiameter = std::nullopt;
        if (try_consume_char(rest, offset, 'X')) {
            holeDiameter = consume_float(rest, offset);
        }

        consume_char(rest, offset, '*');
        consume_char(rest, offset, '%');

        commands.push_back(std::make_shared<ADC>(aperture_id, diameter, holeDiameter));
        return offset;
    }

    offset_t Parser::parse_standard_aperture_p_tail(
        const std::string_view& source, const std::string& aperture_id
    ) {
        std::string_view rest   = source;
        offset_t         offset = 0;

        auto outerDiameter = consume_float(rest, offset);
        consume_char(rest, offset, 'X');
        auto verticesCount = consume_float(rest, offset);

        std::optional<double> rotation = std::nullopt;
        if (try_consume_char(rest, offset, 'X')) {
            rotation = consume_float(rest, offset);
        }
        std::optional<double> holeDiameter = std::nullopt;
        if (try_consume_char(rest, offset, 'X')) {
            holeDiameter = consume_float(rest, offset);
        }

        consume_char(rest, offset, '*');
        consume_char(rest, offset, '%');

        commands.push_back(
            std::make_shared<ADP>(aperture_id, outerDiameter, verticesCount, rotation, holeDiameter)
        );
        return offset;
    }

    [[nodiscard]] double Parser::consume_float(std::string_view& source, offset_t& offset) {
        auto float_str = match_float(source);

        offset += float_str.length();
        source = source.substr(float_str.length());

        return std::stod(float_str);
    }

    char Parser::consume_char(std::string_view& source, offset_t& offset, char expected) {
        match_char(source, expected);

        offset += 1;
        source = source.substr(1);

        return expected;
    }

    bool Parser::try_consume_char(std::string_view& source, offset_t& offset, char expected) {
        if (source.empty() || source[0] != expected) {
            return false;
        }

        offset += 1;
        source = source.substr(1);

        return true;
    }

    std::string
    Parser::consume_regex(std::string_view& source, offset_t& offset, std::regex& expected) {
        std::cmatch match;

        const auto result = std::regex_search(
            source.data(),
            source.data() + source.size(),
            match,
            expected,
            std::regex_constants::match_continuous
        );
        if (result && match.size() >= 1) {
            auto match_length = match.length();
            offset += match_length;
            source = source.substr(match_length);
            return match[0].str();
        }
        throw_syntax_error();
    }

    offset_t Parser::match_char(const std::string_view& source, char expected) {
        if (source.empty() || source[0] != expected) {
            throw_syntax_error();
        }
        return 1;
    }

    std::string Parser::match_float(const std::string_view& source) {
        std::cmatch match;

        const auto result = std::regex_search(
            source.data(),
            source.data() + source.size(),
            match,
            float_regex,
            std::regex_constants::match_continuous
        );
        if (result && match.size() >= 1) {
            return match[0].str();
        }
        throw_syntax_error();
    }

    offset_t Parser::parse_aperture_macro(const std::string_view& source) {
        std::shared_ptr<AMopen>    amOpen;
        AM::primitives_container_t primitives;
        std::shared_ptr<AMclose>   amClose;

        std::string_view rest = source;

        offset_t offset = 0;
        parse_aperture_macro_open(rest, offset, amOpen);

        offset_t previous_offset = offset;
        while (!source.empty() && previous_offset) {
            skip_whitespace(rest, offset);
            previous_offset = parse_aperture_macro_primitive(rest, primitives);
        }

        parse_aperture_macro_close(rest, offset, amClose);

        commands.push_back(std::make_shared<AM>(amOpen, primitives, amClose));
        return offset;
    }

    void Parser::parse_aperture_macro_open(
        std::string_view& source, offset_t& offset, std::shared_ptr<AMopen>& amOpen
    ) {
        consume_char(source, offset, '%');
        consume_char(source, offset, 'A');
        consume_char(source, offset, 'M');
        auto name = consume_regex(source, offset, name_regex);
        consume_char(source, offset, '*');

        amOpen = std::make_shared<AMopen>(name);
    }

    offset_t Parser::parse_aperture_macro_primitive(
        const std::string_view& source, AM::primitives_container_t& primitives
    ) {
        return 0;
    }

    void Parser::parse_aperture_macro_close(
        std::string_view& source, offset_t& offset, std::shared_ptr<AMclose>& amClose
    ) {
        consume_char(source, offset, '%');
        amClose = std::make_shared<AMclose>();
    }

    void Parser::skip_whitespace(std::string_view& source, offset_t& offset) {
        offset_t local_offset = 0;
        while (local_offset < source.length() && std::isspace(source[local_offset])) {
            local_offset++;
        }
        source = source.substr(local_offset);
        offset += local_offset;
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
            auto       match_string = match[1].str();
            const auto g_code_value = std::stoi(match_string);

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

    offset_t Parser::parse_m_code(const std::string_view& gerber) {
        std::cmatch match;

        bool result = std::regex_search(
            gerber.data(),
            gerber.data() + gerber.size(),
            match,
            m_code_regex,
            std::regex_constants::match_continuous
        );
        if (result && match.size() > 1) {
            auto       match_string = match[1].str();
            const auto m_code_value = std::stoi(match_string);

            switch (m_code_value) {
                case 2:
                    commands.push_back(std::make_shared<M02>());
                    return match.length();
            }
        }

        throw_syntax_error();
    }

    offset_t Parser::parse_d_code(const std::string_view& gerber, const location_t& index) {
        std::cmatch match;

        bool result = std::regex_search(
            gerber.data(),
            gerber.data() + gerber.size(),
            match,
            d_code_regex,
            std::regex_constants::match_continuous
        );
        if (result && match.size() > 1) {
            auto       match_string = match[1].str();
            const auto d_code_value = std::stoi(match_string);

            switch (d_code_value) {
                case 1:
                    commands.push_back(std::make_shared<D01>());
                    return match.length();
                case 2:
                    commands.push_back(std::make_shared<D02>());
                    return match.length();
                case 3:
                    commands.push_back(std::make_shared<D03>());
                    return match.length();
                default:
                    commands.push_back(std::make_shared<Dnn>(match_string));
                    return match.length();
            }
        }

        throw_syntax_error();
    }

    offset_t Parser::parse_integer(const std::string_view& source) {
        // Shortest possible integer is 0* or alike.
        if (source.empty()) {
            throw_syntax_error();
        }
        auto integer_length = parse_while_predicate(
            [](char c) {
                return std::isdigit(c);
            },
            source
        );

        if (integer_length == 0) {
            throw_syntax_error();
        }

        return integer_length;
    }

    offset_t
    Parser::parse_extended_command(const std::string_view& source, const location_t& index) {
        // Shortest possible extended command is probably %TD*%, so 5 chars at least.
        if (source.length() < 5) {
            throw_syntax_error();
        }

        switch (source[1]) {
            case 'A':
                return parse_aperture(source);
                break;

            case 'F':
                return parse_fs_command(source, index);
                break;

            case 'M':
                return parse_mo_command(source, index);
                break;

            case 'L':
                return parse_load_command(source, index);
                break;

            default:
                break;
        }

        throw_syntax_error();
    }

    offset_t Parser::parse_load_command(const std::string_view& source, const location_t& index) {
        // Shortest possible load command is %LPD*%, so 6 chars at least.
        if (source.length() < 6) {
            throw_syntax_error();
        }
        // Char 0 is %, char 1 is L, char 2 indicates specific command.
        switch (source[2]) {
            case 'P':
                if (source[3] != 'D' && source[3] != 'C') {
                    throw_syntax_error();
                }
                if (source[4] != '*') {
                    throw_syntax_error();
                }
                if (source[5] != '%') {
                    throw_syntax_error();
                }
                commands.push_back(std::make_shared<LP>(source[3]));
                return 6;
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