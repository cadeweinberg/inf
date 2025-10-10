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
#ifndef INF_IMR_FUNCTION_HPP
#define INF_IMR_FUNCTION_HPP

#include "imr/ssa.hpp"
#include "imr/block.hpp"

namespace inf {
class function {
public:
    using body_iterator = block::iterator;
    using body_const_iterator = block::const_iterator;

private:
    uint32_t locals;
    block m_block;

  public:
    ssa allocate() noexcept { return {locals++}; }

    void append_instruction(instruction i) { m_block.emplace_back(i); }

    body_iterator body_begin() noexcept { return m_block.begin(); }
    body_iterator body_end() noexcept { return m_block.end(); }
    body_const_iterator body_begin() const noexcept { return m_block.begin(); }
    body_const_iterator body_end() const noexcept { return m_block.end(); }
};
}; // namespace inf

#endif // !INF_IMR_FUNCTION_HPP
