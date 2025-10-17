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
#ifndef INF_IMR_SSA_HPP
#define INF_IMR_SSA_HPP

#include <ostream>
#include <cstdint>

namespace inf {
struct ssa {
    uint64_t index;
};

constexpr inline bool operator==(ssa const &a, ssa const &b) {
    return a.index == b.index;
}

constexpr inline bool operator!=(ssa const &a, ssa const &b) {
    return !(a == b);
}

inline std::ostream &operator<<(std::ostream &out, ssa const &var) {
    return out << "$" << var.index;
}
} // namespace inf

#endif // !INF_IMR_SSA_HPP
