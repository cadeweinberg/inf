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

#include "imr/token.hpp"
#include "imr/location.hpp"

namespace inf {
class context;

class lexer {
    char const *buffer;
    char const *token;
    char const *cursor;
    char const *limit;
    location location_;
    context *ctx;

    void up() { location_.inc(token, cursor); }

public:
    lexer(context *ctx)
        : buffer(nullptr), token(nullptr), cursor(nullptr), limit(nullptr), ctx(ctx) {
            BOOST_ASSERT(ctx != nullptr);
        }

    void set_view(std::string_view view) noexcept {
        buffer = token = cursor = view.data();
        limit  = view.data() + view.length();
    }

    void set_path(detail::path path) {
        location_.path = path;
    }

    location loc() const noexcept { return location_; }

    inf::token lex();
};
} // namespace inf

#endif // !INF_CORE_LEX_HPP
