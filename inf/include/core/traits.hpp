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
#ifndef INF_CORE_TRAITS_HPP
#define INF_CORE_TRAITS_HPP

#include "env/context.hpp"
#include "imr/operand.hpp"
#include "imr/type.hpp"

namespace inf {
constexpr inline bool is_integral(type const &t);
constexpr inline bool is_integral(operand const &o, context &ctx);

namespace detail {
struct is_integral_type_visitor {
    constexpr bool operator()(std::monostate const &) { return false; }
    constexpr bool operator()(uint64_t const &) { return true; }
    constexpr bool operator()(uint32_t const &) { return true; }
    constexpr bool operator()(uint16_t const &) { return true; }
    constexpr bool operator()(uint8_t const &) { return true; }
    constexpr bool operator()(int64_t const &) { return true; }
    constexpr bool operator()(int32_t const &) { return true; }
    constexpr bool operator()(int16_t const &) { return true; }
    constexpr bool operator()(int8_t const &) { return true; }
    constexpr bool operator()(type::function const &) { return false; }
};

struct is_integral_operand_visitor {
    context *ctx;

    bool operator()(std::monostate const &) { return false; }

    bool operator()(ssa const & var) { return true; }

    bool operator()(label const & name) { return true; }

    bool operator()(uint64_t const &) { return true; }
    bool operator()(uint32_t const &) { return true; }
    bool operator()(uint16_t const &) { return true; }
    bool operator()(uint8_t const &) { return true; }
    bool operator()(int64_t const &) { return true; }
    bool operator()(int32_t const &) { return true; }
    bool operator()(int16_t const &) { return true; }
    bool operator()(int8_t const &) { return true; }
    bool operator()(error::tag const &) { return false; }
};
} // namespace detail

constexpr inline bool is_integral(type const &t) {
    detail::is_integral_type_visitor visitor{};
    return std::visit(visitor, t.m_variant);
}

constexpr inline bool is_integral(operand const &o, context &ctx) {
    detail::is_integral_operand_visitor visitor{&ctx};
    return std::visit(visitor, o.m_variant);
}
} // namespace inf

#endif // !INF_CORE_TRAITS_HPP
