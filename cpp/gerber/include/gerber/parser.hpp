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

        offset_t parse_extended_command(const std::string_view& source, const location_t& index);
        offset_t parse_load_command(const std::string_view& source, const location_t& index);
        offset_t parse_fs_command(const std::string_view& source, const location_t& index);
        offset_t parse_mo_command(const std::string_view& source, const location_t& index);
    };

} // namespace gerber