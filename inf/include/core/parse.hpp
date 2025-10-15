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
#ifndef INF_CORE_PARSE_HPP
#define INF_CORE_PARSE_HPP

#include <utility>

#include "core/lex.hpp"

namespace inf {
class context;

class parser {
    context *ctx;
    lexer    lexer_;
    token    token_;

    enum class precedence {
        none,
        assignment,
        or_,
        and_,
        equality,
        comparison,
        term,
        factor,
        unary,
        call,
        primary,
    };

    precedence inc(precedence prec) {
        if (prec == precedence::primary) {
            return precedence::primary;
        }

        return static_cast<precedence>(std::to_underlying(prec) + 1);
    }

    using prefix_fn = operand (parser::*)();
    using infix_fn  = operand (parser::*)(operand left);

    struct rule {
        prefix_fn  prefix;
        infix_fn   infix;
        precedence prec;
    };

    operand fail(std::string message);

    bool peek(token expected) { return token_ == expected; }
    void next() { token_ = lexer_.lex(); }

    bool expect(token expected) {
        if (peek(expected)) {
            next();
            return true;
        }

        return false;
    }

    static rule rule_of(token token_);

    operand expression();
    operand infix(precedence prec);
    operand parens();
    operand unop();
    operand binop(operand left);
    operand integer();
    operand label();
    operand top();

  public:
    parser(context *ctx) : ctx(ctx), lexer_(ctx) {}

    void set_view(std::string_view view) { lexer_.set_view(view); }
    void set_path(detail::path path) { lexer_.set_path(path); }
    void prime() { next(); }
    bool done() { return token_ == token::end(); }
    operand parse() { return top(); }
};
} // namespace inf

#endif // !INF_CORE_PARSE_HPP
