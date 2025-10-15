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

#include "core/parse.hpp"
#include "env/context.hpp"


BOOST_AUTO_TEST_CASE ( parse )
{
    inf::context ctx;
    inf::parser parser{&ctx};

    inf::function fn;
    ctx.build_function(&fn);

    parser.set_view("1 + 1;");
    parser.prime();
    inf::operand parse_result = parser.parse();
    BOOST_TEST(parse_result.is<inf::ssa>());
}
