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

#include <utility>

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

        count,
    } m_kind;
    operand m_payload;

    static token error(error::ptr ptr) { return {kind::error, {ptr}}; }
    static token end() { return {kind::end, {}}; }
    static token integer(inf::integer i) {
        return {kind::integer, std::move(i)};
    }
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
    if (a.m_kind != b.m_kind) { return false; }
    switch (a.m_kind) {
    case token::kind::integer: return a.m_payload == b.m_payload;

    case token::kind::label:
        return a.m_payload.as<label>() == b.m_payload.as<label>();

    default: return true;
    }
}

inline bool operator!=(token const &a, token const &b) { return !(a == b); }

inline std::ostream &operator<<(std::ostream &out, token::kind const &kind) {
    switch (kind) {
    case token::kind::error:     out << "error"; break;
    case token::kind::end:       out << "end"; break;
    case token::kind::integer:   out << "integer"; break;
    case token::kind::label:     out << "label"; break;
    case token::kind::lparen:    out << "("; break;
    case token::kind::rparen:    out << ")"; break;
    case token::kind::semicolon: out << ";"; break;
    case token::kind::plus:      out << "+"; break;
    case token::kind::minus:     out << "-"; break;
    case token::kind::star:      out << "*"; break;
    case token::kind::fslash:    out << "/"; break;
    case token::kind::percent:   out << "%"; break;
    default:                     std::unreachable();
    }
    return out;
}

inline std::ostream &operator<<(std::ostream &out, token const &token) {
    return out << token.m_kind << " " << token.m_payload;
}

} // namespace inf

#endif // !INF_IMR_TOKEN_HPP
