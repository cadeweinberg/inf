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
#include "core/traits.hpp"
#include "env/context.hpp"

namespace inf {
namespace detail {

/**
 * #NOTE (10/15/2025)
 * My reasoning right now is that we are in a compile
 * time context, therefore when we are evaluating constant
 * expressions we should be leniant with the programmer
 * and widen the representation as much as we can for
 * the correct evaluation of a given expression.
 * perhaps this assumption is something that would be formalized
 * into a langauge definition or rule or something.
 * My instinct is to have the same behavior in a "evaluate" function
 * as well.
 */

/**
 * valid instructions:
 * neg <ssa>
 * neg <label>
 *
 * folded instructions:
 * neg <integer>
 */
struct compile_neg_visitor {
    context *ctx;

    operand operator()(std::monostate) {
        return ctx->fail("operation unsupported on type");
    }

    operand operator()(ssa var) {
        local &local = ctx->get_local(var);

        if (!is_integral(local.m_type)) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(local.m_type);
        ctx->append_instruction(instruction::neg(a, {var}));
        return a;
    }

    operand operator()(label name) {
        auto found = ctx->lookup_binding(name);
        if (!found) { return ctx->fail("local not found"); }

        if (!is_integral(found->get_type())) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(name, found->get_type());
        ctx->append_instruction(instruction::neg(a, {name}));
        return a;
    }

    operand operator()(integer i) {
        return -i;
    }

    operand operator()(error::ptr ptr) { return {ptr}; }
};

/**
 * valid instructions
 * add <ssa>, <ssa>,      <ssa>
 * add <ssa>, <ssa>,      <label>
 * add <ssa>, <ssa>,      <integer>
 *
 * add <ssa>, <label>,    <ssa>
 * add <ssa>, <label>,    <label>
 * add <ssa>, <label>,    <integer>
 *
 * add <ssa>, <integer>,  <ssa>
 * add <ssa>, <integer>,  <label>
 *
 * folded instructions
 * add <ssa>, <integer>,  <integer>
 *
 */
struct compile_add_visitor {
    context *ctx;
    operand  right;

    operand operator()(std::monostate) {
        return ctx->fail("operation unsupported on type");
    }

    operand operator()(ssa var) {
        local &local = ctx->get_local(var);

        if (!is_integral(local.m_type)) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(local.m_type);
        ctx->append_instruction(instruction::add(a, {var}, right));
        return a;
    }

    operand operator()(label name) {
        auto found = ctx->lookup_binding(name);
        if (!found) { return ctx->fail("local not found"); }

        if (!is_integral(found->get_type())) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(name, found->get_type());
        ctx->append_instruction(instruction::add(a, {name}, right));
        return a;
    }

    operand operator()(integer i) {
        if (right.is<integer>()) {
            return i + right.as<integer>();
        }

        operand a = ctx->allocate_local(ctx->type_integer());
        ctx->append_instruction(instruction::add(a, i, right));
        return a;
    }

    operand operator()(error::ptr ptr) { return {ptr}; }
};
} // namespace detail

operand compile_neg(operand right, context &context) {
    detail::compile_neg_visitor visitor{&context};
    return std::visit(visitor, right.m_variant);
}

operand compile_add(operand left, operand right, context &context) {
    detail::compile_add_visitor visitor{&context, right};
    return std::visit(visitor, left.m_variant);
}

operand compile_sub(operand left, operand right, context &context) {}

operand compile_mul(operand left, operand right, context &context) {}

operand compile_div(operand left, operand right, context &context) {}

operand compile_mod(operand left, operand right, context &context) {}
} // namespace inf
