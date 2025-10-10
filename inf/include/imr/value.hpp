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
namespace detail {
    using value_variant = std::variant<std::monostate,
    uint64_t,
    uint32_t,
    uint16_t,
    uint8_t,
    int64_t,
    int32_t,
    int16_t,
    int8_t,
    function>;
}

class value : public detail::value_variant {
  public:
    value() : detail::value_variant(std::monostate{}) {}
    template <class T> value(T &&t) : detail::value_variant(std::move(t)) {}

    template <class T> bool is() const noexcept {
        return std::holds_alternative<T>(*this);
    }

    template <class T> T       &as() { return std::get<T>(*this); }
    template <class T> T const &as() const { return std::get<T>(*this); }

    template <std::size_t I, class T> T &as() { return std::get<I>(*this); }
    template <std::size_t I, class T> T const &as() const {
        return std::get<I>(*this);
    }
};
}; // namespace inf

#endif // INF_IMR_VALUE_HPP
