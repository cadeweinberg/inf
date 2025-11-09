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
#ifndef INF_ENV_CONTEXT_HPP
#define INF_ENV_CONTEXT_HPP

#include "imr/label.hpp"
#include "env/string_interner.hpp"

namespace inf {
class Context {
    StringInterner string_interner;

public:
    Label intern_string(std::string string) {
        auto [iter, cons] = string_interner.emplace(std::move(string));
        return {*iter};
    }
};
}

#endif // !INF_ENV_CONTEXT_HPP
