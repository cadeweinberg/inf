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

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "core/lex.hpp"
#include "env/context.hpp"

static bool lex_token(inf::lexer &lexer, std::string_view view, inf::token token) {
    lexer.set_view(view);
    inf::token result = lexer.lex();
    return result == token;
}

BOOST_AUTO_TEST_CASE( lex )
{
    inf::context ctx;
    inf::lexer lexer{&ctx};

    BOOST_TEST(lex_token(lexer, {"32789432"}, inf::token::integer(32789432UL)));
    BOOST_TEST(lex_token(lexer, {"index"}, inf::token::label({"index"})));

    BOOST_TEST(lex_token(lexer, {"+"}, inf::token::plus()));
    BOOST_TEST(lex_token(lexer, {"-"}, inf::token::minus()));
    BOOST_TEST(lex_token(lexer, {"*"}, inf::token::star()));
    BOOST_TEST(lex_token(lexer, {"/"}, inf::token::fslash()));
    BOOST_TEST(lex_token(lexer, {"%"}, inf::token::percent()));
}
