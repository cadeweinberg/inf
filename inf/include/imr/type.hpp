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

#include <algorithm>
#include <cstdint>
#include <variant>
#include <vector>

namespace inf {

struct type {
    using ptr = type const *;

    struct function {
        using arguments = std::vector<ptr>;
        ptr       return_type;
        arguments argument_types;
    };

    using variant = std::variant<std::monostate,
                                 uint64_t,
                                 uint32_t,
                                 uint16_t,
                                 uint8_t,
                                 int64_t,
                                 int32_t,
                                 int16_t,
                                 int8_t,
                                 function>;
    variant m_variant;

    type() : m_variant(std::monostate{}) {}
    type(type &t) : m_variant(t.m_variant) {}
    type(type const &t) : m_variant(t.m_variant) {}
    type(type &&t) : m_variant(std::move(t.m_variant)) {}
    template <class T> type(T &&t) : m_variant(std::move(t)) {}

    type &operator=(type &other) {
        m_variant = other.m_variant;
        return *this;
    }

    type &operator=(type const &other) {
        m_variant = other.m_variant;
        return *this;
    }

    type &operator=(type &&other) {
        m_variant = std::move(other.m_variant);
        return *this;
    }

    template <class T> bool is() const noexcept {
        return std::holds_alternative<T>(m_variant);
    }

    template <class T> T       &as() { return std::get<T>(*this); }
    template <class T> T const &as() const { return std::get<T>(m_variant); }

    template <std::size_t I, class T> T &as() { return std::get<I>(m_variant); }
    template <std::size_t I, class T> T const &as() const {
        return std::get<I>(*this);
    }
};

constexpr inline bool operator==(type const &left, type const &right);
constexpr inline bool operator!=(type const &left, type const &right) {
    return !(left == right);
}

namespace detail {
struct type_equality_visitor {
    type::ptr right;

    constexpr bool operator()(std::monostate) {
        return right->is<std::monostate>();
    }

    constexpr bool operator()(uint64_t) { return right->is<uint64_t>(); }

    constexpr bool operator()(uint32_t) { return right->is<uint32_t>(); }

    constexpr bool operator()(uint16_t) { return right->is<uint16_t>(); }

    constexpr bool operator()(uint8_t) { return right->is<uint8_t>(); }

    constexpr bool operator()(int64_t) { return right->is<int64_t>(); }

    constexpr bool operator()(int32_t) { return right->is<int32_t>(); }

    constexpr bool operator()(int16_t) { return right->is<int16_t>(); }

    constexpr bool operator()(int8_t) { return right->is<int8_t>(); }

    constexpr bool operator()(type::function const &left) {
        if (!right->is<type::function>()) { return false; }

        type::function const &rfn = right->as<type::function>();

        if (*left.return_type != *rfn.return_type) { return false; }

        return std::ranges::equal(
            left.argument_types,
            rfn.argument_types,
            [](type::ptr left, type::ptr right) { return *left == *right; });
    }
};
} // namespace detail

constexpr inline bool operator==(type const &left, type const &right) {
    detail::type_equality_visitor visitor{&right};
    return std::visit(visitor, left.m_variant);
}

} // namespace inf

#endif // !INF_IMR_TYPE_HPP
