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
#ifndef INF_CORE_CAST_HPP
#define INF_CORE_CAST_HPP

#include "imr/value.hpp"
#include "imr/operand.hpp"
#include "env/context.hpp"

namespace inf {
template <class To> operand cast(operand from, context &ctx);
template <class To> value cast(value from, context &ctx);
}

#endif // !INF_CORE_CAST_HPP
