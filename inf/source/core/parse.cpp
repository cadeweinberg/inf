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

#include <utility>
#include <array>

#include "core/compile.hpp"
#include "core/parse.hpp"
#include "env/context.hpp"

namespace inf {

operand parser::fail(std::string message) {
    return ctx->fail(std::move(message), lexer_.loc());
}

operand parser::expression() { return infix(precedence::assignment); }

operand parser::infix(precedence prec) {
    rule r = rule_of(token_);
    if (r.prefix == nullptr) { return fail("expected expression"); }

    operand result = (this->*r.prefix)();
    if (result.is<error::ptr>()) { return result; }

    while (true) {
        r = rule_of(token_);
        if (prec > r.prec) { break; }

        operand left = (this->*r.infix)(result);
        if (left.is<error::ptr>()) { return left; }

        result = left;
    }

    return result;
}

operand parser::parens() {
    BOOST_ASSERT(token_ == token::lparen());
    next();
    operand result = expression();

    if (!expect(token::rparen())) { return fail("expected ')'"); }

    return result;
}

operand parser::unop() {
    token op = token_;
    next();

    operand result = infix(precedence::unary);
    if (result.is<error::ptr>()) { return result; }

    switch (op.m_kind) {
    case token::kind::minus: return compile_neg(result, *ctx);
    default:                 return fail("unexpected unop");
    }
}

operand parser::binop(operand left) {
    token        op   = token_;
    parser::rule rule = rule_of(op);
    next();

    operand result = infix(inc(rule.prec));
    if (result.is<error::ptr>()) {
        return result;
    }

    switch (op.m_kind) {
    case token::kind::plus:    return compile_add(left, result, *ctx);
    case token::kind::minus:   return compile_sub(left, result, *ctx);
    case token::kind::star:    return compile_mul(left, result, *ctx);
    case token::kind::fslash:  return compile_div(left, result, *ctx);
    case token::kind::percent: return compile_mod(left, result, *ctx);
    default:                   return fail("unexpected binop");
    }
}

operand parser::u64() {
    BOOST_ASSERT(token_.m_kind == token::kind::u64);
    operand result = token_.m_payload;
    next();
    return result;
}

operand parser::u32() {
    BOOST_ASSERT(token_.m_kind == token::kind::u32);
    operand result = token_.m_payload;
    next();
    return result;
}

operand parser::u16() {
    BOOST_ASSERT(token_.m_kind == token::kind::u16);
    operand result = token_.m_payload;
    next();
    return result;
}

operand parser::u8() {
    BOOST_ASSERT(token_.m_kind == token::kind::u8);
    operand result = token_.m_payload;
    next();
    return result;
}

operand parser::i64() {
    BOOST_ASSERT(token_.m_kind == token::kind::i64);
    operand result = token_.m_payload;
    next();
    return result;
}

operand parser::i32() {
    BOOST_ASSERT(token_.m_kind == token::kind::i32);
    operand result = token_.m_payload;
    next();
    return result;
}

operand parser::i16() {
    BOOST_ASSERT(token_.m_kind == token::kind::i16);
    operand result = token_.m_payload;
    next();
    return result;
}

operand parser::i8() {
    BOOST_ASSERT(token_.m_kind == token::kind::i8);
    operand result = token_.m_payload;
    next();
    return result;
}

operand parser::label() {
    BOOST_ASSERT(token_.m_kind == token::kind::label);
    operand result = token_.m_payload;
    next();
    return result;
}

operand parser::top() { return expression(); }

// clang-format off
// static
parser::rule parser::rule_of(token token_) {
    static std::array<parser::rule,
        (size_t)std::to_underlying(token::kind::count)> rules = {{
        {nullptr,          nullptr,        parser::precedence::none}, // token::error
        {nullptr,          nullptr,        parser::precedence::none}, // token::end
        {&parser::u64,     nullptr,        parser::precedence::primary}, // token::u64
        {&parser::u32,     nullptr,        parser::precedence::primary}, // token::u32
        {&parser::u16,     nullptr,        parser::precedence::primary}, // token::u16
        {&parser::u8,      nullptr,        parser::precedence::primary}, // token::u8
        {&parser::i64,     nullptr,        parser::precedence::primary}, // token::i64
        {&parser::i32,     nullptr,        parser::precedence::primary}, // token::i32
        {&parser::i16,     nullptr,        parser::precedence::primary}, // token::i16
        {&parser::i8,      nullptr,        parser::precedence::primary}, // token::i8
        {&parser::label,   nullptr,        parser::precedence::primary}, // token::label
        {&parser::parens,  nullptr,        parser::precedence::call}, // token::lparen
        {nullptr,          nullptr,        parser::precedence::none}, // token::rparen
        {nullptr,          nullptr,        parser::precedence::none}, // token::semicolon
        {nullptr,          &parser::binop, parser::precedence::term}, // token::plus
        {&parser::unop,    &parser::binop, parser::precedence::term}, // token::minus
        {nullptr,          &parser::binop, parser::precedence::factor}, // token::star
        {nullptr,          &parser::binop, parser::precedence::factor}, // token::fslash
        {nullptr,          &parser::binop, parser::precedence::factor}, // token::percent
    }};

    return rules.at((size_t)std::to_underlying(token_.m_kind));
}
// clang-format on
} // namespace inf
