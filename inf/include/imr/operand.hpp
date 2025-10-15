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

#include <cstdint>
#include <variant>

#include "imr/error.hpp"
#include "imr/label.hpp"
#include "imr/ssa.hpp"

namespace inf {
namespace detail {}

struct operand {
    using variant = std::variant<std::monostate,
                                 ssa,
                                 label,
                                 uint64_t,
                                 uint32_t,
                                 uint16_t,
                                 uint8_t,
                                 int64_t,
                                 int32_t,
                                 int16_t,
                                 int8_t,
                                 error::tag>;

    variant m_variant;

    operand() : m_variant(std::monostate{}) {}
    operand(operand &o) : m_variant(o.m_variant) {}
    operand(operand const &o) : m_variant(o.m_variant) {}
    operand(operand &&o) : m_variant(std::move(o.m_variant)) {}
    template <class T> operand(T &&t) : m_variant(std::move(t)) {}

    operand &operator=(operand &other) {
        m_variant = other.m_variant;
        return *this;
    }

    operand &operator=(operand const &other) {
        m_variant = other.m_variant;
        return *this;
    }

    operand &operator=(operand &&other) {
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
} // namespace inf

#endif // !INF_IMR_OPERAND_HPP
