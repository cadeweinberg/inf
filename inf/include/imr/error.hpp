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
#ifndef INF_IMR_ERROR_HPP
#define INF_IMR_ERROR_HPP

#include <string>

#include "imr/location.hpp"

namespace inf {
struct error {
    std::string msg;
    location    loc;

  public:
    using ptr = error const *;

    error(std::string msg, location loc = {}) : msg(std::move(msg)), loc(loc) {}
};

inline std::ostream &operator<<(std::ostream &out, error const &e) {
    return out << e.loc << " " << e.msg;
}
} // namespace inf

#endif // !INF_IMR_ERROR_HPP
