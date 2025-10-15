# Copyright (C) 2024 Cade Weinberg
#
# This file is part of inf.
#
# inf is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# inf is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with inf.  If not, see <http://www.gnu.org/licenses/>.
cmake_minimum_required(VERSION 3.20)

execute_process(
  COMMAND git rev-parse --short HEAD
  OUTPUT_VARIABLE INF_GIT_REVISION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(TIMESTAMP INF_BUILD_TIME "%Y-%m-%d %H:%M:%S")

set(INF_VERSION_STRING "")
string(APPEND INF_VERSION_STRING
"inf version ("
${INF_VERSION_MAJOR}
"."
${INF_VERSION_MINOR}
"."
${INF_VERSION_PATCH}
") built at ("
${INF_BUILD_TIME}
") git revision ("
${INF_GIT_REVISION}
")"
)

configure_file(
    ${INF_INCLUDE_DIR}/support/config.hpp.in
    ${INF_INCLUDE_DIR}/support/config.hpp
)

add_custom_command(
    OUTPUT ${INF_SOURCE_DIR}/core/lex.cpp
    COMMAND re2c --output ${INF_SOURCE_DIR}/core/lex.cpp ${INF_SOURCE_DIR}/core/lex.re2c
    DEPENDS ${INF_SOURCE_DIR}/core/lex.re2c
)

set(INF_COMMON_SOURCE_FILES
  ${INF_SOURCE_DIR}/core/cast.cpp
  ${INF_SOURCE_DIR}/core/compile.cpp
  ${INF_SOURCE_DIR}/core/lex.cpp
  ${INF_SOURCE_DIR}/core/parse.cpp
)
add_library(inf_common ${INF_COMMON_SOURCE_FILES})
target_include_directories(inf_common PUBLIC ${INF_INCLUDE_DIR})
target_compile_options(inf_common PUBLIC ${INF_COMPILE_OPTIONS})
target_link_options(inf_common PUBLIC ${INF_LINK_OPTIONS})
target_link_libraries(inf_common PUBLIC ${INF_DEPS})

add_executable(inf
    ${INF_SOURCE_DIR}/main.cpp
)
target_include_directories(inf PRIVATE ${INF_INCLUDE_DIR})
target_compile_options(inf PRIVATE ${INF_COMPILE_OPTIONS})
target_link_options(inf PRIVATE ${INF_LINK_OPTIONS})
target_link_libraries(inf PRIVATE inf_common)


add_executable(inf_test
    ${INF_TEST_DIR}/lex.cpp
    ${INF_TEST_DIR}/main.cpp
    ${INF_TEST_DIR}/parse.cpp
)
target_include_directories(inf_test PRIVATE ${INF_INCLUDE_DIR})
target_compile_options(inf_test PRIVATE ${INF_COMPILE_OPTIONS})
target_link_options(inf_test PRIVATE ${INF_LINK_OPTIONS})
target_link_libraries(inf_test PRIVATE inf_common)

enable_testing()
add_test(NAME lex COMMAND inf_test -t lex)
add_test(NAME parse COMMAND inf_test -t parse)









