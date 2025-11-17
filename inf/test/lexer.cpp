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
#include "boost/test/unit_test.hpp"

#include "core/lexer.hpp"

static inline bool tokenize(yy::Lexer &lexer,
                            yy::Lexer::Token expected,
                            std::string_view text) {
    lexer.set_view(text);
    yy::Lexer::Token actual = lexer.advance();
    return actual == expected;
}

BOOST_AUTO_TEST_CASE ( lexer )
{
    yy::Lexer lexer;
    BOOST_TEST(tokenize(lexer, yy::Lexer::Token::Semicolon{}, ";"));
    BOOST_TEST(tokenize(lexer, yy::Lexer::Token::LParen{}, "("));
    BOOST_TEST(tokenize(lexer, yy::Lexer::Token::RParen{}, ")"));
    BOOST_TEST(tokenize(lexer, yy::Lexer::Token::Plus{}, "+"));
    BOOST_TEST(tokenize(lexer, yy::Lexer::Token::Minus{}, "-"));
    BOOST_TEST(tokenize(lexer, yy::Lexer::Token::Star{}, "*"));
    BOOST_TEST(tokenize(lexer, yy::Lexer::Token::FSlash{}, "/"));
    BOOST_TEST(tokenize(lexer, yy::Lexer::Token::Percent{}, "%"));
    BOOST_TEST(tokenize(lexer, inf::Integer{0}, "0"));
    BOOST_TEST(tokenize(lexer, inf::Integer{778932789523}, "778932789523"));
}
