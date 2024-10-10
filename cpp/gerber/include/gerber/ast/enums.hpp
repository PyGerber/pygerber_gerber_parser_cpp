#pragma once
#include <cstdint>
#include <string_view>

namespace gerber {
    class Zeros {
      public:
        enum Enum : uint8_t {
            SKIP_LEADING,
            SKIP_TRAILING
        };

        Enum value;

      private:
        Zeros() = delete;

      public:
        Zeros(Enum value);
        static Zeros fromString(const std::string_view& str);
        bool         operator==(const Zeros& other) const;
        bool         operator==(const Enum& other) const;
    };

    class CoordinateNotation {
      public:
        enum Enum : uint8_t {
            ABSOLUTE,
            INCREMENTAL
        };

        Enum value;

      private:
        CoordinateNotation() = delete;

      public:
        CoordinateNotation(Enum value);
        static CoordinateNotation fromString(const std::string_view& str);
        bool                      operator==(const CoordinateNotation& other) const;
        bool                      operator==(const Enum& other) const;
    };

    class UnitMode {
      public:
        enum Enum : uint8_t {
            INCHES,
            MILLIMETERS
        };

        Enum value;

      private:
        UnitMode() = delete;

      public:
        UnitMode(Enum value);

        static UnitMode fromString(const std::string_view& str);
        std::string     toString() const;
        bool            operator==(const UnitMode& other) const;
        bool            operator==(const Enum& other) const;
    };
} // namespace gerber