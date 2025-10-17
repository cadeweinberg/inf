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
#ifndef INF_IMR_OPERAND_HPP
#define INF_IMR_OPERAND_HPP

#include <variant>

#include "imr/error.hpp"
#include "imr/label.hpp"
#include "imr/ssa.hpp"
#include "imr/number.hpp"

namespace inf {
struct operand {
    using variant = std::variant<std::monostate,
                                 ssa,
                                 label,
                                 integer,
                                 error::ptr>;

    variant m_variant;

    constexpr operand() : m_variant(std::monostate{}) {}
    constexpr operand(operand &o) : m_variant(o.m_variant) {}
    constexpr operand(operand const &o) : m_variant(o.m_variant) {}
    constexpr operand(operand &&o) : m_variant(std::move(o.m_variant)) {}
    template <class T> constexpr operand(T &&t) : m_variant(std::move(t)) {}

    constexpr operand &operator=(operand &other) {
        m_variant = other.m_variant;
        return *this;
    }

    constexpr operand &operator=(operand const &other) {
        m_variant = other.m_variant;
        return *this;
    }

    constexpr operand &operator=(operand &&other) {
        m_variant = std::move(other.m_variant);
        return *this;
    }

    template <class T> constexpr bool is() const noexcept {
        return std::holds_alternative<T>(m_variant);
    }

    template <class T> constexpr T &as() { return std::get<T>(m_variant); }
    template <class T> constexpr T const &as() const {
        return std::get<T>(m_variant);
    }

    template <std::size_t I, class T> constexpr T &as() {
        return std::get<I>(m_variant);
    }
    template <std::size_t I, class T> constexpr T const &as() const {
        return std::get<I>(m_variant);
    }
};

namespace detail {
struct operand_equality_visitor {
    operand b;

    constexpr bool operator()(std::monostate const &) {
        return b.is<std::monostate>();
    }

    constexpr bool operator()(ssa const &var) {
        if (!b.is<ssa>()) { return false; }
        return var == b.as<ssa>();
    }

    constexpr bool operator()(label const &name) {
        if (!b.is<ssa>()) { return false; }
        return name == b.as<label>();
    }

    constexpr bool operator()(integer const &i) {
        if (!b.is<integer>()) { return false; }
        return i == b.as<integer>();
    }

    constexpr bool operator()(error::ptr const &) {
        return b.is<error::ptr>(); // errors are errors
    }
};

struct operand_leftshift_visitor {
    std::ostream *out;

    std::ostream &operator()(std::monostate const &) { return *out << "nil"; }
    std::ostream &operator()(ssa const &var) { return *out << var; }
    std::ostream &operator()(label const &name) { return *out << "$" << name; }
    std::ostream &operator()(integer const &i) { return *out << i; }
    std::ostream &operator()(error::ptr const &p) { return *out << *p; }
};
} // namespace detail

constexpr inline bool operator==(operand const &a, operand const &b) {
    detail::operand_equality_visitor visitor{b};
    return std::visit(visitor, a.m_variant);
}

constexpr inline bool operator!=(operand const &a, operand const &b) {
    return !(a == b);
}

inline std::ostream &operator<<(std::ostream &out, operand const &a) {
    detail::operand_leftshift_visitor visitor{&out};
    return std::visit(visitor, a.m_variant);
}

} // namespace inf

#endif // !INF_IMR_OPERAND_HPP
