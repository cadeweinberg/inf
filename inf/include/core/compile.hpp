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
#ifndef INF_CORE_COMPILE_HPP
#define INF_CORE_COMPILE_HPP

#include "env/context.hpp"

namespace inf {
operand compile_neg(operand right, context &context);
operand compile_add(operand left, operand right, context &context);
operand compile_sub(operand left, operand right, context &context);
operand compile_mul(operand left, operand right, context &context);
operand compile_div(operand left, operand right, context &context);
operand compile_mod(operand left, operand right, context &context);
} // namespace inf

#endif // !INF_CORE_COMPILE_HPP
