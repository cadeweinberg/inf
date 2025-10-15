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

#include <cstdint>
#include <variant>



#include "imr/function.hpp"

namespace inf {
struct value {
    using variant = std::variant<std::monostate,
                                 uint64_t,
                                 uint32_t,
                                 uint16_t,
                                 uint8_t,
                                 int64_t,
                                 int32_t,
                                 int16_t,
                                 int8_t,
                                 function,
                                 error::tag>;

    variant m_variant;

    value() : m_variant(std::monostate{}) {}
    value(value &v) : m_variant(v.m_variant) {}
    value(value const &v) : m_variant(v.m_variant) {}
    value(value &&v) : m_variant(std::move(v.m_variant)) {}
    template <class T> value(T &&t) : m_variant(std::move(t)) {}

    value &operator=(value &other) {
        m_variant = other.m_variant;
        return *this;
    }

    value &operator=(value const &other) {
        m_variant = other.m_variant;
        return *this;
    }

    value &operator=(value &&other) {
        m_variant = std::move(other.m_variant);
        return *this;
    }

    template <class T> bool is() const noexcept {
        return std::holds_alternative<T>(m_variant);
    }

    template <class T> T       &as() { return std::get<T>(m_variant); }
    template <class T> T const &as() const { return std::get<T>(m_variant); }

    template <std::size_t I, class T> T &as() { return std::get<I>(m_variant); }
    template <std::size_t I, class T> T const &as() const {
        return std::get<I>(m_variant);
    }
};
}; // namespace inf

#endif // INF_IMR_VALUE_HPP
