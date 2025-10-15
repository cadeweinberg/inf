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
#ifndef INF_IMR_BINDING_HPP
#define INF_IMR_BINDING_HPP

#include <variant>

#include "imr/local.hpp"
#include "imr/symbol.hpp"

namespace inf {
namespace detail {
using binding_variant = std::variant<local, symbol>;
}

class binding : public detail::binding_variant {
public:
    template <class T> binding(T &&t) : detail::binding_variant(std::move(t)) {}

    type const &get_type() const noexcept {
        if (is<local>()) {
            return as<local>().m_type;
        }

        return as<symbol>().m_type;
    }

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
}

#endif // !INF_IMR_BINDING_HPP
