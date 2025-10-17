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
#ifndef INF_IMR_LOCAL_HPP
#define INF_IMR_LOCAL_HPP

#include "imr/label.hpp"
#include "imr/type.hpp"

namespace inf {
struct local {
    label     m_label;
    type::ptr m_type;

    local(label name, type::ptr kind) : m_label(name), m_type(kind) {}
    local(type::ptr kind) : m_label(), m_type(kind) {}
};
} // namespace inf

#endif // !INF_IMR_LOCAL_HPP
