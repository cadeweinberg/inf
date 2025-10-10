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
#ifndef INF_IMR_INSTRUCTION_HPP
#define INF_IMR_INSTRUCTION_HPP

#include "imr/operand.hpp"

namespace inf {
struct instruction {
    enum class opcode {
        NEG,

        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
    } opcode;

    operand a;
    operand b;
    operand c;

    static instruction neg(operand a, operand b) {
        return {opcode::NEG, a, b, {}};
    }

    static instruction add(operand a, operand b, operand c) {
        return {opcode::ADD, a, b, c};
    }

    static instruction sub(operand a, operand b, operand c) {
        return {opcode::SUB, a, b, c};
    }

    static instruction mul(operand a, operand b, operand c) {
        return {opcode::MUL, a, b, c};
    }

    static instruction div(operand a, operand b, operand c) {
        return {opcode::DIV, a, b, c};
    }

    static instruction mod(operand a, operand b, operand c) {
        return {opcode::MOD, a, b, c};
    }
};


} // namespace inf

#endif // !INF_IMR_INSTRUCTION_HPP
