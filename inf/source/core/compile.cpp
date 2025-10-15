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
 * neg <uint64_t>
 * neg <uint32_t>
 * neg <uint16_t>
 * neg <uint8_t>
 * neg <int64_t>
 * neg <int32_t>
 * neg <int16_t>
 * neg <int8_t>
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

    operand operator()(uint64_t x) {
        if (x > INT64_MAX) { return ctx->fail("integer overflow"); }
        return {-static_cast<int64_t>(x)};
    }

    operand operator()(uint32_t x) {
        if (x > INT32_MAX) { return {-static_cast<int64_t>(x)}; }
        return {-static_cast<int32_t>(x)};
    }

    operand operator()(uint16_t x) {
        if (x > INT16_MAX) { return {-static_cast<int32_t>(x)}; }
        return {-static_cast<int16_t>(x)};
    }

    operand operator()(uint8_t x) {
        if (x > INT8_MAX) { return {-static_cast<int16_t>(x)}; }
        return {-static_cast<int8_t>(x)};
    }

    operand operator()(int64_t x) {
        if (x == INT64_MIN) { return ctx->fail("integer overflow"); }
        return {-x};
    }

    operand operator()(int32_t x) {
        if (x == INT32_MIN) { return {-static_cast<int64_t>(x)}; }
        return {-x};
    }

    operand operator()(int16_t x) {
        if (x == INT16_MIN) { return {-static_cast<int32_t>(x)}; }
        return {-x};
    }

    operand operator()(int8_t x) {
        if (x == INT8_MIN) { return {-static_cast<int16_t>(x)}; }
        return {-x};
    }

    operand operator()(error::tag tag) { return {tag}; }
};

struct compile_add_u64_visitor {
    context *ctx;
    uint64_t x;

    operand operator()(std::monostate) {
        return ctx->fail("operation unsupported on type");
    }

    operand operator()(ssa var) {
        local &local = ctx->get_local(var);

        if (!is_integral(local.m_type)) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(local.m_type);
        ctx->append_instruction(instruction::add(a, x, {var}));
        return a;
    }

    operand operator()(label name) {
        auto found = ctx->lookup_binding(name);
        if (!found) { return ctx->fail("local not found"); }

        if (!is_integral(found->get_type())) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(name, found->get_type());
        ctx->append_instruction(instruction::add(a, x, {name}));
        return a;
    }

    // <uint64_t> + <uint64_t>
    operand operator()(uint64_t y) {
        uint64_t r;
        if (__builtin_add_overflow(x, y, &r)) {
            // no wider builtin type to cast to
            return ctx->fail("integer overflow");
        }
        return r;
    }

    // <uint64_t> + <uint32_t>
    operand operator()(uint32_t y) {
        // since UINT32_MAX < UINT64_MAX we are always safe to cast
        uint64_t r;
        if (__builtin_add_overflow(x, (uint64_t)y, &r)) {
            // no wider builtin type to cast to
            return ctx->fail("integer overflow");
        }
        return r;
    }

    // <uint64_t> + <uint16_t>
    operand operator()(uint16_t y) {
        // since UINT16_MAX < UINT64_MAX we are always safe to cast
        uint64_t r;
        if (__builtin_add_overflow(x, (uint64_t)y, &r)) {
            // no wider builtin type to cast to
            return ctx->fail("integer overflow");
        }
        return r;
    }

    // <uint64_t> + <uint8_t>
    operand operator()(uint8_t y) {
        // since UINT8_MAX < UINT64_MAX we are always safe to cast
        uint64_t r;
        if (__builtin_add_overflow(x, (uint64_t)y, &r)) {
            // no wider builtin type to cast to
            return ctx->fail("integer overflow");
        }
        return r;
    }

    // <uint64_t> + <int64_t>
    operand operator()(int64_t y) {
        if (y < 0) { // try and cast x to a signed integer
            // y is in the range [INT64_MIN, -1]
            if (x > INT64_MAX) {
                // since x > INT64_MAX it is never safe to cast.
                // and there is no wider builtin integer type.
                return ctx->fail("integer out of bounds");
            }

            // since x <= INT64_MAX it is always safe to cast
            int64_t r;
            if (__builtin_add_overflow((int64_t)x, y, &r)) {
                // if we overflow here there is no wider builtin integer type.
                return ctx->fail("integer overflow");
            }
            return r;
        }

        // cast y to an unsigned integer
        // y is in the range [0, INT64_MAX]
        // since INT64_MAX < UINT64_MAX it is always safe to cast.
        uint64_t r;
        if (__builtin_add_overflow(x, (uint64_t)y, &r)) {
            // if we overflow here there is no wider builtin integer type.
            return ctx->fail("integer overflow");
        }
        return r;
    }

    // <uint64_t> + <int32_t>
    operand operator()(int32_t y) {
        if (y < 0) {// try and cast x to a signed integer
            // y is in the range [INT64_MIN, -1]
            if (x > INT64_MAX) {
                // since x > INT64_MAX it is never safe to cast.
                // and there is no wider builtin integer type.
                return ctx->fail("integer out of bounds");
            }

            // since x <= INT64_MAX it is always safe to cast
            // since x is 64 bits wide, and y is 32 bits wide
            // wether or not the addition will overflow a 32 bit result
            // depends on the exact value of both x and y considered
            // together. the catch-all check is to simply widen y to
            // 64 bits and see if that overflows. we have no wider
            // integer builtin type, so that's the stopping point.
            int64_t r;
            if (__builtin_add_overflow((int64_t)x, (int64_t)y, &r)) {
                return ctx->fail("integer overflow");
            }
            return r;
        }

        uint64_t r;
        if (__builtin_add_overflow(x, (uint64_t)y, &r)) {
            return ctx->fail("integer overflow");
        }
        return r;
    }

    // <uint64_t> + <int16_t>
    operand operator()(int16_t y) {
        if (y < 0) {
            if (x > INT64_MAX) { return ctx->fail("integer out of bounds"); }

            int64_t r;
            if (__builtin_add_overflow((int64_t)x, (int64_t)y, &r)) {
                return ctx->fail("integer overflow");
            }
            return r;
        }

        uint64_t r;
        if (__builtin_add_overflow(x, (uint64_t)y, &r)) {
            return ctx->fail("integer overflow");
        }
        return r;
    }

    // <uint64_t> + <int8_t>
    operand operator()(int8_t y) {
        if (y < 0) {
            if (x > INT64_MAX) { return ctx->fail("integer out of bounds"); }

            int64_t r;
            if (__builtin_add_overflow((int64_t)x, (int64_t)y, &r)) {
                return ctx->fail("integer overflow");
            }
            return r;
        }

        uint64_t r;
        if (__builtin_add_overflow(x, (uint64_t)y, &r)) {
            return ctx->fail("integer overflow");
        }
        return r;
    }

    operand operator()(error::tag tag) { return {tag}; }
};

struct compile_add_u32_visitor {
    context *ctx;
    uint32_t x;

    operand operator()(std::monostate) {
        return ctx->fail("operation unsupported on type");
    }

    operand operator()(ssa var) {
        local &local = ctx->get_local(var);

        if (!is_integral(local.m_type)) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(local.m_type);
        ctx->append_instruction(instruction::add(a, x, {var}));
        return a;
    }

    operand operator()(label name) {
        auto found = ctx->lookup_binding(name);
        if (!found) { return ctx->fail("local not found"); }

        if (!is_integral(found->get_type())) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(name, found->get_type());
        ctx->append_instruction(instruction::add(a, x, {name}));
        return a;
    }

    // <uint32_t> + <uint64_t>
    operand operator()(uint64_t y) {
        // since UINT32_MAX < UINT64_MAX we are always safe to cast
        uint64_t r;
        if (__builtin_add_overflow((uint64_t)x, y, &r)) {
            return ctx->fail("integer overflow");
        }
        return r;
    }

    // <uint32_t> + <uint32_t>
    operand operator()(uint32_t y) {
        uint32_t r;
        if (__builtin_add_overflow(x, y, &r)) {
            // since UINT32_MAX + UINT32_MAX < UINT64_MAX
            // we don't need to check for the overflow.
            uint64_t e = (uint64_t)x + (uint64_t)y;
            return e;
        }
        return r;
    }

    // <uint32_t> + <uint16_t>
    operand operator()(uint16_t y) {
        uint32_t r;
        if (__builtin_add_overflow(x, (uint32_t)y, &r)) {
            uint64_t e = (uint64_t)x + (uint64_t)y;
            return e;
        }
        return r;
    }

    // <uint32_t> + <uint8_t>
    operand operator()(uint8_t y) {
        uint32_t r;
        if (__builtin_add_overflow(x, (uint32_t)y, &r)) {
            uint64_t e = (uint64_t)x + (uint64_t)y;
            return e;
        }
        return r;
    }

    // <uint32_t> + <int64_t>
    operand operator()(int64_t y) {
        if (y < 0) {
            // since UINT32_MAX < INT64_MAX we are always safe to cast.
            int64_t r;
            if (__builtin_add_overflow((int64_t)x, y, &r)) {
                return ctx->fail("integer overflow");
            }
            return r;
        }

        // since INT64_MAX < UINT64_MAX we are always safe to cast
        uint64_t r;
        if (__builtin_add_overflow(x, (uint64_t)y, &r)) {
            return ctx->fail("integer overflow");
        }
        return r;
    }

    // <uint32_t> + <int32_t>
    operand operator()(int32_t y) {
        if (y < 0) { // try and cast x to a signed integer
            // y is in the range [INT32_MIN, -1]
            if (x >= INT32_MAX) {
                // x is in the range [INT32_MAX, UINT32_MAX]
                // since UINT32_MAX < INT64_MAX we are always safe to cast
                // since INT32_MIN < INT64_MIN we are always safe to cast
                // since -1 + UINT32_MAX < INT64_MAX we don't need to check for
                // overflow
                int64_t e = (int64_t)x + (int64_t)y;
                return e;
            }

            // x is in the range [0, INT32_MAX)
            int32_t r;
            if (__builtin_add_overflow((int32_t)x, y, &r)) {
                // since (INT32_MAX - 1) + INT32_MAX < INT64_MAX
                // and 0 + INT32_MIN > INT64_MIN then we
                // don't have to check for overflow on 64 bit addition
                int64_t e = (int64_t)x + (int64_t)y;
                return e;
            }
            return r;
        }

        // y is in the range [0, INT32_MAX]
        // since INT32_MAX < UINT32_MAX we are always safe to cast
        uint32_t r;
        if (__builtin_add_overflow(x, (uint32_t)y, &r)) {
            // since UINT32_MAX + INT32_MAX < UINT64_MAX we don't need to check
            // for overflow here
            uint64_t e = (uint64_t)x + (uint64_t)y;
            return e;
        }

        return r;
    }

    // <uint32_t> + <int16_t>
    operand operator()(int16_t y) {
        if (y < 0) {
            // y is in the range [INT32_MIN, -1]
            if (x >= INT16_MAX) {
                int64_t e = (int64_t)x + (int64_t)y;
                return e;
            }

            int32_t r;
            if (__builtin_add_overflow((int32_t)x, (int32_t)y, &r)) {
                int64_t e = (int64_t)x + (int64_t)y;
                return e;
            }
            return r;
        }

        uint32_t r;
        if (__builtin_add_overflow(x, (uint32_t)y, &r)) {
            uint64_t e = (uint64_t)x + (uint64_t)y;
            return e;
        }
        return {r};
    }

    operand operator()(int8_t y) {
        if (y < 0) {
            if (x > INT32_MAX) {
                int64_t e = (int64_t)x + (int64_t)y;
                return e;
            }

            int32_t r;
            if (__builtin_add_overflow((int32_t)x, (int32_t)y, &r)) {
                int64_t e = (int64_t)x + (int64_t)y;
                return e;
            }
            return r;
        }

        uint32_t r;
        if (__builtin_add_overflow(x, (uint32_t)y, &r)) {
            uint64_t e = (uint64_t)x + (uint64_t)y;
            return e;
        }
        return {r};
    }

    operand operator()(error::tag tag) { return {tag}; }
};

struct compile_add_u16_visitor {
    context *ctx;
    uint16_t x;

    operand operator()(std::monostate) {
        return ctx->fail("operation unsupported on type");
    }

    operand operator()(ssa var) {
        local &local = ctx->get_local(var);

        if (!is_integral(local.m_type)) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(local.m_type);
        ctx->append_instruction(instruction::add(a, x, {var}));
        return a;
    }

    operand operator()(label name) {
        auto found = ctx->lookup_binding(name);
        if (!found) { return ctx->fail("local not found"); }

        if (!is_integral(found->get_type())) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(name, found->get_type());
        ctx->append_instruction(instruction::add(a, x, {name}));
        return a;
    }

    operand operator()(uint64_t y) {
        uint64_t r;
        if (__builtin_add_overflow((uint64_t)x, y, &r)) {
            return ctx->fail("integer overflow");
        }
        return r;
    }

    operand operator()(uint32_t y) {
        uint32_t r;
        if (__builtin_add_overflow((uint32_t)x, y, &r)) {
            uint64_t e = (uint64_t)x + (uint64_t)y;
            return e;
        }
        return r;
    }

    operand operator()(uint16_t y) {
        uint16_t r;
        if (__builtin_add_overflow(x, y, &r)) {
            uint32_t e = (uint32_t)x + (uint32_t)y;
            return e;
        }
        return r;
    }

    operand operator()(uint8_t y) {
        uint8_t r;
        if (__builtin_add_overflow(x, y, &r)) {
            uint16_t e = (uint16_t)x + (uint16_t)y;
            return e;
        }
        return r;
    }

    operand operator()(int64_t y) {}

    operand operator()(int32_t y) {}

    operand operator()(int16_t y) {}

    operand operator()(int8_t y) {}

    operand operator()(error::tag tag) { return {tag}; }
};

struct compile_add_u8_visitor {
    context *ctx;
    uint8_t  x;

    operand operator()(std::monostate) {
        return ctx->fail("operation unsupported on type");
    }

    operand operator()(ssa var) {
        local &local = ctx->get_local(var);

        if (!is_integral(local.m_type)) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(local.m_type);
        ctx->append_instruction(instruction::add(a, x, {var}));
        return a;
    }

    operand operator()(label name) {
        auto found = ctx->lookup_binding(name);
        if (!found) { return ctx->fail("local not found"); }

        if (!is_integral(found->get_type())) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(name, found->get_type());
        ctx->append_instruction(instruction::add(a, x, {name}));
        return a;
    }

    operand operator()(uint64_t y) {}

    operand operator()(uint32_t y) {}

    operand operator()(uint16_t y) {}

    operand operator()(uint8_t y) {}

    operand operator()(int64_t y) {}

    operand operator()(int32_t y) {}

    operand operator()(int16_t y) {}

    operand operator()(int8_t y) {}

    operand operator()(error::tag tag) { return {tag}; }
};

struct compile_add_i64_visitor {
    context *ctx;
    int64_t  x;

    operand operator()(std::monostate) {
        return ctx->fail("operation unsupported on type");
    }

    operand operator()(ssa var) {
        local &local = ctx->get_local(var);

        if (!is_integral(local.m_type)) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(local.m_type);
        ctx->append_instruction(instruction::add(a, x, {var}));
        return a;
    }

    operand operator()(label name) {
        auto found = ctx->lookup_binding(name);
        if (!found) { return ctx->fail("local not found"); }

        if (!is_integral(found->get_type())) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(name, found->get_type());
        ctx->append_instruction(instruction::add(a, x, {name}));
        return a;
    }

    operand operator()(uint64_t y) {}

    operand operator()(uint32_t y) {}

    operand operator()(uint16_t y) {}

    operand operator()(uint8_t y) {}

    operand operator()(int64_t y) {}

    operand operator()(int32_t y) {}

    operand operator()(int16_t y) {}

    operand operator()(int8_t y) {}

    operand operator()(error::tag tag) { return {tag}; }
};

struct compile_add_i32_visitor {
    context *ctx;
    int32_t  x;

    operand operator()(std::monostate) {
        return ctx->fail("operation unsupported on type");
    }

    operand operator()(ssa var) {
        local &local = ctx->get_local(var);

        if (!is_integral(local.m_type)) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(local.m_type);
        ctx->append_instruction(instruction::add(a, x, {var}));
        return a;
    }

    operand operator()(label name) {
        auto found = ctx->lookup_binding(name);
        if (!found) { return ctx->fail("local not found"); }

        if (!is_integral(found->get_type())) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(name, found->get_type());
        ctx->append_instruction(instruction::add(a, x, {name}));
        return a;
    }

    operand operator()(uint64_t y) {}

    operand operator()(uint32_t y) {}

    operand operator()(uint16_t y) {}

    operand operator()(uint8_t y) {}

    operand operator()(int64_t y) {}

    operand operator()(int32_t y) {}

    operand operator()(int16_t y) {}

    operand operator()(int8_t y) {}

    operand operator()(error::tag tag) { return {tag}; }
};

struct compile_add_i16_visitor {
    context *ctx;
    int16_t  x;

    operand operator()(std::monostate) {
        return ctx->fail("operation unsupported on type");
    }

    operand operator()(ssa var) {
        local &local = ctx->get_local(var);

        if (!is_integral(local.m_type)) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(local.m_type);
        ctx->append_instruction(instruction::add(a, x, {var}));
        return a;
    }

    operand operator()(label name) {
        auto found = ctx->lookup_binding(name);
        if (!found) { return ctx->fail("local not found"); }

        if (!is_integral(found->get_type())) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(name, found->get_type());
        ctx->append_instruction(instruction::add(a, x, {name}));
        return a;
    }

    operand operator()(uint64_t y) {}

    operand operator()(uint32_t y) {}

    operand operator()(uint16_t y) {}

    operand operator()(uint8_t y) {}

    operand operator()(int64_t y) {}

    operand operator()(int32_t y) {}

    operand operator()(int16_t y) {}

    operand operator()(int8_t y) {}

    operand operator()(error::tag tag) { return {tag}; }
};

struct compile_add_i8_visitor {
    context *ctx;
    int8_t   x;

    operand operator()(std::monostate) {
        return ctx->fail("operation unsupported on type");
    }

    operand operator()(ssa var) {
        local &local = ctx->get_local(var);

        if (!is_integral(local.m_type)) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(local.m_type);
        ctx->append_instruction(instruction::add(a, x, {var}));
        return a;
    }

    operand operator()(label name) {
        auto found = ctx->lookup_binding(name);
        if (!found) { return ctx->fail("local not found"); }

        if (!is_integral(found->get_type())) {
            return ctx->fail("operation unsupported on type");
        }

        operand a = ctx->allocate_local(name, found->get_type());
        ctx->append_instruction(instruction::add(a, x, {name}));
        return a;
    }

    operand operator()(uint64_t y) {}

    operand operator()(uint32_t y) {}

    operand operator()(uint16_t y) {}

    operand operator()(uint8_t y) {}

    operand operator()(int64_t y) {}

    operand operator()(int32_t y) {}

    operand operator()(int16_t y) {}

    operand operator()(int8_t y) {}

    operand operator()(error::tag tag) { return {tag}; }
};

/**
 * valid instructions
 * add <ssa>, <ssa>,      <ssa>
 * add <ssa>, <ssa>,      <label>
 * add <ssa>, <ssa>,      <uint64_t>
 * add <ssa>, <ssa>,      <uint32_t>
 * add <ssa>, <ssa>,      <uint16_t>
 * add <ssa>, <ssa>,      <uint8_t>
 * add <ssa>, <ssa>,      <int64_t>
 * add <ssa>, <ssa>,      <int32_t>
 * add <ssa>, <ssa>,      <int16_t>
 * add <ssa>, <ssa>,      <int8_t>
 *
 * add <ssa>, <label>,    <ssa>
 * add <ssa>, <label>,    <label>
 * add <ssa>, <label>,    <uint64_t>
 * add <ssa>, <label>,    <uint32_t>
 * add <ssa>, <label>,    <uint16_t>
 * add <ssa>, <label>,    <uint8_t>
 * add <ssa>, <label>,    <int64_t>
 * add <ssa>, <label>,    <int32_t>
 * add <ssa>, <label>,    <int16_t>
 * add <ssa>, <label>,    <int8_t>
 *
 * add <ssa>, <uint64_t>, <ssa>
 * add <ssa>, <uint64_t>, <label>
 *
 * add <ssa>, <uint32_t>, <ssa>
 * add <ssa>, <uint32_t>, <label>
 *
 * add <ssa>, <uint16_t>, <ssa>
 * add <ssa>, <uint16_t>, <label>
 *
 * add <ssa>, <uint8_t>,  <ssa>
 * add <ssa>, <uint8_t>,  <label>
 *
 * add <ssa>, <int64_t>,  <ssa>
 * add <ssa>, <int64_t>,  <label>
 *
 * add <ssa>, <int32_t>,  <ssa>
 * add <ssa>, <int32_t>,  <label>
 *
 * add <ssa>, <int16_t>,  <ssa>
 * add <ssa>, <int16_t>,  <label>
 *
 * add <ssa>, <int8_t>,   <ssa>
 * add <ssa>, <int8_t>,   <label>
 *
 * folded instructions:
 * add <ssa>, <uint64_t>, <uint64_t>
 * add <ssa>, <uint64_t>, <uint32_t>
 * add <ssa>, <uint64_t>, <uint16_t>
 * add <ssa>, <uint64_t>, <uint8_t>
 * add <ssa>, <uint64_t>, <int64_t>
 * add <ssa>, <uint64_t>, <int32_t>
 * add <ssa>, <uint64_t>, <int16_t>
 * add <ssa>, <uint64_t>, <int8_t>
 *
 * add <ssa>, <uint32_t>, <uint64_t>
 * add <ssa>, <uint32_t>, <uint32_t>
 * add <ssa>, <uint32_t>, <uint16_t>
 * add <ssa>, <uint32_t>, <uint8_t>
 * add <ssa>, <uint32_t>, <int64_t>
 * add <ssa>, <uint32_t>, <int32_t>
 * add <ssa>, <uint32_t>, <int16_t>
 * add <ssa>, <uint32_t>, <int8_t>
 *
 * add <ssa>, <uint16_t>, <uint64_t>
 * add <ssa>, <uint16_t>, <uint32_t>
 * add <ssa>, <uint16_t>, <uint16_t>
 * add <ssa>, <uint16_t>, <uint8_t>
 * add <ssa>, <uint16_t>, <int64_t>
 * add <ssa>, <uint16_t>, <int32_t>
 * add <ssa>, <uint16_t>, <int16_t>
 * add <ssa>, <uint16_t>, <int8_t>
 *
 * add <ssa>, <uint8_t>, <uint64_t>
 * add <ssa>, <uint8_t>, <uint32_t>
 * add <ssa>, <uint8_t>, <uint16_t>
 * add <ssa>, <uint8_t>, <uint8_t>
 * add <ssa>, <uint8_t>, <int64_t>
 * add <ssa>, <uint8_t>, <int32_t>
 * add <ssa>, <uint8_t>, <int16_t>
 * add <ssa>, <uint8_t>, <int8_t>
 *
 * add <ssa>, <int64_t>, <uint64_t>
 * add <ssa>, <int64_t>, <uint32_t>
 * add <ssa>, <int64_t>, <uint16_t>
 * add <ssa>, <int64_t>, <uint8_t>
 * add <ssa>, <int64_t>, <int64_t>
 * add <ssa>, <int64_t>, <int32_t>
 * add <ssa>, <int64_t>, <int16_t>
 * add <ssa>, <int64_t>, <int8_t>
 *
 * add <ssa>, <int32_t>, <uint64_t>
 * add <ssa>, <int32_t>, <uint32_t>
 * add <ssa>, <int32_t>, <uint16_t>
 * add <ssa>, <int32_t>, <uint8_t>
 * add <ssa>, <int32_t>, <int64_t>
 * add <ssa>, <int32_t>, <int32_t>
 * add <ssa>, <int32_t>, <int16_t>
 * add <ssa>, <int32_t>, <int8_t>
 *
 * add <ssa>, <int16_t>, <uint64_t>
 * add <ssa>, <int16_t>, <uint32_t>
 * add <ssa>, <int16_t>, <uint16_t>
 * add <ssa>, <int16_t>, <uint8_t>
 * add <ssa>, <int16_t>, <int64_t>
 * add <ssa>, <int16_t>, <int32_t>
 * add <ssa>, <int16_t>, <int16_t>
 * add <ssa>, <int16_t>, <int8_t>
 *
 * add <ssa>, <int8_t>, <uint64_t>
 * add <ssa>, <int8_t>, <uint32_t>
 * add <ssa>, <int8_t>, <uint16_t>
 * add <ssa>, <int8_t>, <uint8_t>
 * add <ssa>, <int8_t>, <int64_t>
 * add <ssa>, <int8_t>, <int32_t>
 * add <ssa>, <int8_t>, <int16_t>
 * add <ssa>, <int8_t>, <int8_t>
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

    operand operator()(uint64_t x) {
        compile_add_u64_visitor visitor{ctx, x};
        return std::visit(visitor, right.m_variant);
    }

    operand operator()(uint32_t x) {
        compile_add_u32_visitor visitor{ctx, x};
        return std::visit(visitor, right.m_variant);
    }

    operand operator()(uint16_t x) {
        compile_add_u16_visitor visitor{ctx, x};
        return std::visit(visitor, right.m_variant);
    }

    operand operator()(uint8_t x) {
        compile_add_u8_visitor visitor{ctx, x};
        return std::visit(visitor, right.m_variant);
    }

    operand operator()(int64_t x) {
        compile_add_i64_visitor visitor{ctx, x};
        return std::visit(visitor, right.m_variant);
    }

    operand operator()(int32_t x) {
        compile_add_i32_visitor visitor{ctx, x};
        return std::visit(visitor, right.m_variant);
    }

    operand operator()(int16_t x) {
        compile_add_i16_visitor visitor{ctx, x};
        return std::visit(visitor, right.m_variant);
    }

    operand operator()(int8_t x) {
        compile_add_i8_visitor visitor{ctx, x};
        return std::visit(visitor, right.m_variant);
    }

    operand operator()(error::tag tag) { return {tag}; }
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

operand compile_sub(operand left, operand right, context &context) {

}

operand compile_mul(operand left, operand right, context &context) {

}

operand compile_div(operand left, operand right, context &context) {

}

operand compile_mod(operand left, operand right, context &context) {

}
} // namespace inf
