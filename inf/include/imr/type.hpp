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
#ifndef INF_IMR_TYPE_HPP
#define INF_IMR_TYPE_HPP

#include <memory>
#include <variant>

namespace inf {
class Type {
  public:
    using Ptr = std::unique_ptr<Type>;
    struct Nil {};
    struct Integer {};

  private:
    using Variant = std::variant<Nil, Integer>;
    Variant variant;

  public:
    Type() : variant() {}
    Type(Type const &type) : variant(type.variant) {}
    Type(Type &&type) : variant(std::move(type.variant)) {}
    template <class T> Type(T &&t) : variant(std::move(t)) {}
    template <class T> Type(T const &t) : variant(t) {}

    template <class T> Type &operator=(T &&t) {
        variant = std::move(t);
        return *this;
    }

    template <class T> Type &operator=(T const &t) {
        variant = t;
        return *this;
    }

    Type &operator=(Type &&other) {
        if (this == &other) { return *this; }

        variant = std::move(other.variant);
        return *this;
    }

    Type &operator=(Type const &other) {
        if (this == &other) { return *this; }

        variant = other.variant;
        return *this;
    }

    Variant       &get() noexcept { return variant; }
    Variant const &get() const noexcept { return variant; }

    template <class T> bool is() const noexcept {
        return std::holds_alternative<T>(variant);
    }

    template <class T> T       &as() { return std::get<T>(variant); }
    template <class T> T const &as() const { return std::get<T>(variant); }

    template <class T> static Ptr create(T &&t) {
        return std::make_unique<Type>(std::move(t));
    }
};

bool        operator==(Type const &a, Type const &b);
inline bool operator!=(Type const &a, Type const &b) { return !(a == b); }
} // namespace inf

#endif // !INF_IMR_TYPE_HPP
