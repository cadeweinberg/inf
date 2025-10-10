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

#include "core/compile.hpp"
#include "env/context.hpp"

namespace inf {
static operand fail(std::string msg, context &context) {
    error::tag tag = context.fail(std::move(msg), {});
    return {tag};
}

operand compile_neg(operand right, context &context) {
    switch (right.index()) {
    case 0: // std::monostate
        return fail("operation unsupported on type", context);

    case 1:   // ssa
    case 2: { // label
        operand a{context.allocate_local()};
        context.append_instruction(instruction::neg(a, right));
        return a;
    }

    case 3: // uint64_t
    case 4: // uint32_t
    case 5: // uint16_t
    case 6: // uint8_t
        return fail("operation unsupported on type", context);

    case 7: // int64_t
        return {-(right.as<int64_t>())};
    case 8: // int32_t
        return {-(right.as<int32_t>())};
    case 9: // int16_t
        return {-(right.as<int16_t>())};
    case 10: // int8_t
        return {-(right.as<int8_t>())};

    case 11:                                                   // error::tag
        return fail("operation unsupported on type", context); // int8_t

    default: return fail("unexpected operand kind", context);
    }
}

operand compile_add(operand left, operand right, context &context) {
    operand a{context.allocate_local()};
    context.append_instruction(instruction::add(a, left, right));
    return a;
}

operand compile_sub(operand left, operand right, context &context) {
    operand a{context.allocate_local()};
    context.append_instruction(instruction::sub(a, left, right));
    return a;
}

operand compile_mul(operand left, operand right, context &context) {
    operand a{context.allocate_local()};
    context.append_instruction(instruction::mul(a, left, right));
    return a;
}

operand compile_div(operand left, operand right, context &context) {
    operand a{context.allocate_local()};
    context.append_instruction(instruction::div(a, left, right));
    return a;
}

operand compile_mod(operand left, operand right, context &context) {
    operand a{context.allocate_local()};
    context.append_instruction(instruction::mod(a, left, right));
    return a;
}
} // namespace inf
