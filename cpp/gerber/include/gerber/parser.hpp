#pragma once
#include "fmt/format.h"
#include "gerber/ast/ast.hpp"
#include "gerber/errors.hpp"
#include <algorithm>
#include <cstdint>
#include <memory>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace gerber {
    using location_t = uint64_t;
    using offset_t   = uint64_t;

    const std::tuple<location_t, location_t>
    get_line_column(const std::string_view& source, const location_t& index);

    class Parser {
      private:
        std::vector<std::shared_ptr<Node>> commands;
        std::string_view                   full_source;
        location_t                         global_index;
        // Regular expressions cache
        // G-codes
        std::regex                         g_code_regex{"^[Gg]0*([1-9][0-9]*)\\*"};
        std::regex                         g04_regex{"^[Gg]0*4([^%*]+)\\*"};
        // Properties
        std::regex fs_regex{"^%FS([TL])([IA])X([0-9])([0-9])Y([0-9])([0-9])\\*%"};
        std::regex mo_regex{"^%MO(IN|MM)\\*%"};

      public:
        Parser();

        File              parse(const std::string& source);
        location_t        parse_global(const std::string_view& source, const location_t& index);
        [[noreturn]] void throw_syntax_error();
        offset_t          parse_g_code(const std::string_view& gerber, const location_t& index);

        template <typename coordinate_type>
        offset_t parse_coordinate(const std::string_view& source, const location_t& index) {
            // Shortest possible X coordinate is X0* or alike.
            if (source.length() < 3) {
                throw_syntax_error();
            }

            auto length = parse_integer(source.substr(1));
            commands.push_back(std::make_shared<coordinate_type>(source.substr(1, length)));

            return 1 + length;
        }

        offset_t parse_integer(const std::string_view& source);

        template <typename lambda_t>
        offset_t parse_while_predicate(lambda_t char_predicate, const std::string_view& source) {
            offset_t substring_length = 0;
            auto     source_length    = source.length();

            while (substring_length < source_length && char_predicate(source[substring_length])) {
                substring_length++;
            }
            return substring_length;
        }

        offset_t parse_extended_command(const std::string_view& source, const location_t& index);
        offset_t parse_load_command(const std::string_view& source, const location_t& index);
        offset_t parse_fs_command(const std::string_view& source, const location_t& index);
        offset_t parse_mo_command(const std::string_view& source, const location_t& index);
    };

} // namespace gerber