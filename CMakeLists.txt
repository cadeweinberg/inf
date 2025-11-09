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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  set(INF_HOST_SYSTEM_LINUX true)
  execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE HOST_CPU
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
else()
  set(INF_HOST_SYSTEM_LINUX false)
  message(FATAL_ERROR "Unsupported Host System: ${CMAKE_HOST_SYSTEM_NAME}")
endif()

if(HOST_CPU STREQUAL "x86_64")
  set(INF_HOST_CPU_x86_64 true)
else()
  set(INF_HOST_CPU_x86_64 false)
  message(FATAL_ERROR "Unsupported Host CPU: ${HOST_CPU}")
endif()

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)

project(INF
    LANGUAGES C CXX ASM
    VERSION 0.0.1
    DESCRIPTION "A Compiler"
)

set(INF_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/inf/include)
set(INF_SOURCE_DIR ${PROJECT_SOURCE_DIR}/inf/source)
set(INF_TEST_DIR ${PROJECT_SOURCE_DIR}/inf/test)

set(INF_WARNINGS
    -Wall
    -Wdeprecated
    -Wextra
    -Wpedantic
    -Wconversion
)

set(INF_SANITIZERS
    -fsanitize=address,undefined,leak
)
set(INF_PROFILE
    -pg
)

if (CMAKE_BUILD_TYPE STREQUAL "Sanitize")
  set(INF_COMPILE_OPTIONS ${INF_WARNINGS} ${INF_SANITIZERS})
  set(INF_LINK_OPTIONS ${INF_SANITIZERS})
elseif (CMAKE_BUILD_TYPE STREQUAL "Profile")
  set(INF_COMPILE_OPTIONS ${INF_WARNINGS} ${INF_PROFILE})
  set(INF_LINK_OPTIONS ${INF_PROFILE})
else()
  set(INF_COMPILE_OPTIONS ${INF_WARNINGS})
  set(INF_LINK_OPTIONS)
endif()

find_package(PkgConfig REQUIRED)

pkg_check_modules(GMP gmp)
pkg_check_modules(MPFR mpfr)

include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG ef7fea34711a189472893b88205b1dd3c275677b # release 1.89.0
)

FetchContent_Declare(
  LLVM
  GIT_REPOSITORY https://github.com/llvm/llvm-project.git
  GIT_TAG 8e2cd28cd4ba46613a46467b0c91b1cabead26cd # release 21.1.5
)

# Control which projects are enabled. For example, you may want to work
# on clang or lldb by specifying -DLLVM_ENABLE_PROJECTS="clang;lldb".
# bolt;clang;clang-tools-extra;compiler-rt;cross-project-tests;libclc;lld;lldb;mlir;openmp;polly
set(LLVM_ENABLE_PROJECTS "lld")

# Control which targets are enabled. For example, you may only need to
# enable your native target with, for example, -DLLVM_TARGETS_TO_BUILD=X86.
set(LLVM_TARGETS_TO_BUILD "X86")

# Enables code assertions. Defaults to ON if and only if CMAKE_BUILD_TYPE is Debug.
set(LLVM_ENABLE_ASSERTIONS ON)

# If enabled and building a debug or assert build,
# the CMake build system will generate a Release build
# tree to build a fully optimized tablegen for use during
# the build. Enabling this option can significantly speed
# up build times, especially when building LLVM in Debug configurations.
set(LLVM_OPTIMIZED_TABLEGEN ON)

FetchContent_MakeAvailable(Boost LLVM)


set(INF_DEPS
  gmp
  mpfr
  Boost::graph
  Boost::unit_test_framework
  Boost::multiprecision
  Boost::log
  LLVM
)

message(STATUS "Build: ${CMAKE_BUILD_TYPE}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER_ID}")
message(STATUS "C standard: ${CMAKE_C_STANDARD}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

include(CTest)
add_subdirectory(${INF_SOURCE_DIR})









