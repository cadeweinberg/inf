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
#ifndef INF_CORE_LEX_HPP
#define INF_CORE_LEX_HPP

#include <string_view>

#include "boost/assert.hpp"

#include "env/context.hpp"
#include "imr/ast.hpp"
#include "imr/location.hpp"

namespace yy {
class Lexer {
    char const   *buffer;
    char const   *token;
    char const   *marker;
    char const   *cursor;
    char const   *limit;
    inf::Context *context;
    location      location_;

    void up() {
        char const *p = token;
        while (p != cursor) {
            switch (*p) {
            case '\n':
                location_.lines();
                location_.end.column = 1;
                break;
            case '\t':
            case '\r':
            case ' ':
            default:   location_.columns(); break;
            }
        }
    }

  public:
    struct Token {
        enum class Kind {
            Error,
            End,

            Semicolon,
            LParen,
            RParen,
            Plus,
            Minus,
            Star,
            FSlash,
            Percent,

            Integer,
        } kind;
        inf::Ast::Ptr data;

        static Token create(Kind kind, inf::Ast::Ptr data) {
            return Token{kind, std::move(data)};
        }

        static Token error(inf::Error error) {
            return create(Kind::Error, inf::Ast::create(std::move(error)));
        }
        static Token end() { return create(Kind::End, {}); }
        static Token semicolon() { return create(Kind::Semicolon, {}); }
        static Token lparen() { return create(Kind::LParen, {}); }
        static Token rparen() { return create(Kind::RParen, {}); }
        static Token plus() { return create(Kind::Plus, {}); }
        static Token minus() { return create(Kind::Minus, {}); }
        static Token star() { return create(Kind::Star, {}); }
        static Token fslash() { return create(Kind::FSlash, {}); }
        static Token percent() { return create(Kind::Percent, {}); }
        static Token integer(inf::Integer i) {
            return create(Kind::Integer,
                          inf::Ast::create(inf::Value{std::move(i)}));
        }
    };

    Lexer(inf::Context *context)
        : buffer(nullptr), token(nullptr), cursor(nullptr), limit(nullptr),
          context(context) {
        BOOST_ASSERT(context != nullptr);
    }

    void set_view(std::string_view view) noexcept {
        buffer = token = cursor = view.data();
        limit                   = view.data() + view.length();
    }

    Token advance();
};
} // namespace yy

#endif // !INF_CORE_LEX_HPP
