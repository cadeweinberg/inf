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
#ifndef INF_IMR_VALUE_HPP
#define INF_IMR_VALUE_HPP

#include <variant>

#include "imr/number.hpp"

namespace inf {
class Value {
  public:
    using Nil = std::monostate;
    using Variant = std::variant<Nil, Integer>;

  private:
    Variant variant;

  public:
    Value() : variant() {}
    Value(Value const &value) : variant(value.variant) {}
    Value(Value &&value) : variant(std::move(value.variant)) {}
    template <class T> Value(T &&t) : variant(std::move(t)) {}

    template <class T> Value &operator=(T &&t) {
        variant = std::move(t);
        return *this;
    }

    Value &operator=(Value const &other) {
        if (this == &other) { return *this; }

        variant = other.variant;
        return *this;
    }

    Value &operator=(Value &&other) {
        if (this == &other) { return *this; }

        variant = std::move(other.variant);
        return *this;
    }

    Variant &get() noexcept { return variant; }
    Variant const &get() const noexcept { return variant; }

    template <class T> bool is() const noexcept {
        return std::holds_alternative<T>(variant);
    }

    template <class T> T       &as() { return std::get<T>(variant); }
    template <class T> T const &as() const { return std::get<T>(variant); }
};

bool        operator==(Value const &a, Value const &b);
inline bool operator!=(Value const &a, Value const &b) { return !(a == b); }
} // namespace inf

#endif // !INF_IMR_VALUE_HPP
