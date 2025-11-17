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

#include <variant>

#include "boost/assert.hpp"

#include "llvm/IR/Constants.h"

#include "env/context.hpp"
#include "imr/location.hpp"

namespace yy {
class Lexer {
    char const *buffer;
    char const *token;
    char const *marker;
    char const *cursor;
    char const *limit;
    location    location_;
    inf::Context *context;

    void up();

  public:
    struct Token {
        struct Error { inf::ErrorList::size_type index; };
        struct End {};
        struct Semicolon {};
        struct LParen {};
        struct RParen {};
        struct Plus {};
        struct Minus {};
        struct Star {};
        struct FSlash {};
        struct Percent {};
        struct Integer { llvm::ConstantInt *integer; };

        using Variant = std::variant<End,
                                     Error,
                                     Semicolon,
                                     LParen,
                                     RParen,
                                     Plus,
                                     Minus,
                                     Star,
                                     FSlash,
                                     Percent,
                                     Integer>;
        Variant variant;
        Token() : variant(End{}) {}
        Token(Token &&token) : variant(std::move(token.variant)) {}
        Token(Token const &token) : variant(token.variant) {}
        template <class T> Token(T &&t) : variant(std::move(t)) {}
        template <class T> Token(T const &t) : variant(t) {}

        Token &operator=(Token &&other) {
            if (&other == this) { return *this; }

            variant = std::move(other.variant);
            return *this;
        }

        Token &operator=(Token const &other) {
            if (&other == this) { return *this; }

            variant = other.variant;
            return *this;
        }

        template <class T> Token &operator=(T &&t) {
            variant = std::move(t);
            return *this;
        }

        template <class T> Token &operator=(T const &t) {
            variant = t;
            return *this;
        }

        Variant       &get() noexcept { return variant; }
        Variant const &get() const noexcept { return variant; }

        template <class T> bool is() const noexcept {
            return std::holds_alternative<T>(variant);
        }

        template <class T> T       &as() { return std::get<T>(variant); }
        template <class T> T const &as() const { return std::get<T>(variant); }
    };

    Lexer()
        : buffer(nullptr), token(nullptr), cursor(nullptr), limit(nullptr) {}

    void set_view(std::string_view view) noexcept {
        buffer = token = cursor = view.data();
        limit                   = view.data() + view.length();
    }

    location const &loc() const noexcept { return location_; }

    Token advance();
};

bool operator==(Lexer::Token const &a, Lexer::Token const &b);
inline bool operator!=(Lexer::Token const &a, Lexer::Token const &b) {
    return !(a == b);
}

} // namespace yy

#endif // !INF_CORE_LEX_HPP
