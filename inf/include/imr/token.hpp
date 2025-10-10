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
#ifndef INF_IMR_TOKEN_HPP
#define INF_IMR_TOKEN_HPP

#include "imr/operand.hpp"

namespace inf {
struct token {
    enum class kind {
        error,
        end,

        integer,
        label,

        lparen,
        rparen,
        semicolon,

        plus,
        minus,
        star,
        fslash,
        percent,
    } kind;
    operand payload;

    static token error() { return {kind::error, {}}; }
    static token end() { return {kind::end, {}}; }
    static token integer(uint64_t i) { return {kind::integer, i}; }
    static token label(label l) { return {kind::label, l}; }
    static token lparen() { return {kind::lparen, {}}; }
    static token rparen() { return {kind::rparen, {}}; }
    static token semicolon() { return {kind::semicolon, {}}; }
    static token plus() { return {kind::plus, {}}; }
    static token minus() { return {kind::minus, {}}; }
    static token star() { return {kind::star, {}}; }
    static token fslash() { return {kind::fslash, {}}; }
    static token percent() { return {kind::percent, {}}; }
};

inline bool operator==(token const &a, token const &b) {
    if (a.kind != b.kind) { return false; }
    switch (a.kind) {
    case token::kind::integer:
        return a.payload.as<uint64_t>() == b.payload.as<uint64_t>();

    case token::kind::label:
        return a.payload.as<label>() == b.payload.as<label>();

    default: return true;
    }
}

inline bool operator!=(token const &a, token const &b) { return !(a == b); }

} // namespace inf

#endif // !INF_IMR_TOKEN_HPP
