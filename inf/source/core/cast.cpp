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

#include <type_traits>

#include "core/cast.hpp"

namespace inf {

namespace detail {
template <class To> struct cast_operand_visitor {
    context *ctx;

    operand operator()(std::monostate const &) {
        if constexpr (std::is_same<To, ssa>()) {
            return {ctx->fail("Bad Cast")};
        }

        if constexpr (std::is_same<To, label>()) {
            return {ctx->fail("Bad Cast")};
        }

        return {static_cast<To>(0)};
    }
};

template <class To> struct cast_value_visitor {
    context *ctx;

    value operator()(std::monostate const &) {
        if constexpr (std::is_same<To, function>()) {
            return {ctx->fail("Bad Cast")};
        }

        return {static_cast<To>(0)};
    }

    value operator()(uint64_t const &from) {
        if constexpr (std::is_same<To, function>()) {
            return {ctx->fail("Bad Cast")};
        }

        return {static_cast<To>(from)};
    }

    value operator()(uint32_t const &from) {
        if constexpr (std::is_same<To, function>()) {
            return {ctx->fail("Bad Cast")};
        }

        return {static_cast<To>(from)};
    }

    value operator()(uint16_t const &from) {
        if constexpr (std::is_same<To, function>()) {
            return {ctx->fail("Bad Cast")};
        }

        return {static_cast<To>(from)};
    }

    value operator()(uint8_t const &from) {
        if constexpr (std::is_same<To, function>()) {
            return {ctx->fail("Bad Cast")};
        }

        return {static_cast<To>(from)};
    }

    value operator()(int64_t const &from) {
        if constexpr (std::is_same<To, function>()) {
            return {ctx->fail("Bad Cast")};
        }

        return {static_cast<To>(from)};
    }

    value operator()(int32_t const &from) {
        if constexpr (std::is_same<To, function>()) {
            return {ctx->fail("Bad Cast")};
        }

        return {static_cast<To>(from)};
    }

    value operator()(int16_t const &from) {
        if constexpr (std::is_same<To, function>()) {
            return {ctx->fail("Bad Cast")};
        }

        return {static_cast<To>(from)};
    }

    value operator()(int8_t const &from) {
        if constexpr (std::is_same<To, function>()) {
            return {ctx->fail("Bad Cast")};
        }

        return {static_cast<To>(from)};
    }

    value operator()(function const &) { return {ctx->fail("Bad Cast")}; }

    value operator()(error::ptr const &ptr) { return ptr; }
};
} // namespace detail

template <class To> operand cast(operand from, context &ctx) {
    detail::cast_operand_visitor<To> visitor{&ctx};
    return std::visit(visitor, from);
}

template <class To> value cast(value from, context &ctx) {
    detail::cast_value_visitor<To> visitor(&ctx);
    return std::visit(visitor, from);
}

} // namespace inf
