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

#include "imr/value.hpp"

namespace inf {
namespace detail {
struct ValueEqualityVisitor {
    Value const *b;

    bool operator()(Value::Nil const &) { return b->is<Value::Nil>(); }

    bool operator()(Integer const &integer) {
        if (!b->is<Value::Nil>()) { return false; }
        return integer == b->as<Integer>();
    }
};
} // namespace detail

bool operator==(Value const &a, Value const &b) {
    detail::ValueEqualityVisitor visitor{&b};
    return std::visit(visitor, a.get());
}
} // namespace inf
