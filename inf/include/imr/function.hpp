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

#include <algorithm>

#include "imr/block.hpp"
#include "imr/locals.hpp"
#include "imr/ssa.hpp"

namespace inf {
class function {
  public:
    using body_iterator        = block::iterator;
    using body_const_iterator  = block::const_iterator;
    using local_iterator       = locals::iterator;
    using local_const_iterator = locals::const_iterator;

  private:
    locals m_locals;
    block  m_block;

  public:
    template <class... Args> ssa allocate(Args &&...args) noexcept {
        m_locals.emplace_back(std::forward<Args>(args)...);
        return {m_locals.size() - 1};
    }
    uint64_t nlocals() const noexcept { return m_locals.size(); }

    local         &get_local(ssa var) { return m_locals.at(var.index); }
    local_iterator get_local(label name) {
        return std::ranges::find_if(m_locals, [name](local const &local) {
            return name == local.m_label;
        });
    }

    void append_instruction(instruction i) { m_block.emplace_back(i); }

    body_iterator        body_begin() noexcept { return m_block.begin(); }
    body_iterator        body_end() noexcept { return m_block.end(); }
    body_const_iterator  body_begin() const noexcept { return m_block.begin(); }
    body_const_iterator  body_end() const noexcept { return m_block.end(); }
    local_iterator       locals_begin() noexcept { return m_locals.begin(); }
    local_iterator       locals_end() noexcept { return m_locals.end(); }
    local_const_iterator locals_begin() const noexcept {
        return m_locals.begin();
    }
    local_const_iterator locals_end() const noexcept { return m_locals.end(); }
};
}; // namespace inf

#endif // !INF_IMR_FUNCTION_HPP
