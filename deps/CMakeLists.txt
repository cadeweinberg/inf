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

include(ExternalProject)

set_directory_properties(PROPERTIES EP_PREFIX ${PROJECT_BINARY_DIR}/deps)

ExternalProject_Add(Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG ef7fea34711a189472893b88205b1dd3c275677b # release 1.89.0
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)

ExternalProject_Add(LLVM
  GIT_REPOSITORY https://github.com/llvm/llvm-project.git
  GIT_TAG 8e2cd28cd4ba46613a46467b0c91b1cabead26cd # release 21.1.5
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    # Control which projects are enabled. For example, you may want to work
    # on clang or lldb by specifying -DLLVM_ENABLE_PROJECTS="clang;lldb".
    # bolt;clang;clang-tools-extra;compiler-rt;cross-project-tests;libclc;lld;lldb;mlir;openmp;polly
    -DLLVM_ENABLE_PROJECTS=lld
    # Control which targets are enabled. For example, you may only need to
    # enable your native target with, for example, -DLLVM_TARGETS_TO_BUILD=X86.
    -DLLVM_TARGETS_TO_BUILD=X86
    # Enables code assertions. Defaults to ON if and only if CMAKE_BUILD_TYPE is Debug.
    -DLLVM_ENABLE_ASSERTIONS=ON
    # If enabled and building a debug or assert build,
    # the CMake build system will generate a Release build
    # tree to build a fully optimized tablegen for use during
    # the build. Enabling this option can significantly speed
    # up build times, especially when building LLVM in Debug configurations.
    -DLLVM_OPTIMIZED_TABLEGEN=ON
    # To prevent OOM
    -DLLVM_PARALLEL_LINK_JOBS=2
  SOURCE_SUBDIR llvm
)

ExternalProject_Add(
    INF_SUPERBUILD
    DEPENDS Boost LLVM
    SOURCE_DIR ${PROJECT_SOURCE_DIR}
    BINARY_DIR ${PROJECT_BINARY_DIR}
    INSTALL_DIR ${PROJECT_BINARY_DIR}
    CMAKE_ARGS
        -DCMAKE_PREFIX_PATH=${PROJECT_BINARY_DIR}/deps
        -DBUILD_DEPENDENCIES=OFF
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
)
