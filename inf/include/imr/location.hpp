// Copyright (C) 2024 Cade Weinberg
//
// This file is part of inf.
//
// inf is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// inf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with inf.  If not, see <http://www.gnu.org/licenses/>.
#ifndef INF_IMR_LOCATION_HPP
#define INF_IMR_LOCATION_HPP

#include <cstdint>
#include <filesystem>
#include <ostream>

namespace fs = std::filesystem;

namespace inf {
namespace detail {
using path = fs::path *;
}

struct position {
    int32_t column;
    int32_t line;

    position() : column(1), line(1) {}
};

struct location {
    detail::path path;
    position     begin;
    position     end;

    struct tag { uint64_t index; };

    location() : path(nullptr), begin(), end() {}
    location(detail::path path) : path(path), begin(), end() {}

    void step() { begin = end; }
    void inc(char const *token, char const *cursor) {
        while (token != cursor) {
            switch (*token++) {
            case '\n':
                end.column = 1;
                end.line += 1;
                break;
            default: end.column += 1; break;
            }
        }
    }
};

inline std::ostream &operator<<(std::ostream &out, location const &location) {
    out << "[";
    if (location.path != nullptr) {
        out << location.path->string() << ":";
    }
    out << location.begin.line << "]";
    return out;
}
} // namespace inf

#endif // !INF_IMR_LOCATION_HPP
