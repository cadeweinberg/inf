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
#ifndef INF_ENV_CONTEXT_HPP
#define INF_ENV_CONTEXT_HPP

#include <cstdint>
#include <optional>

#include "boost/assert.hpp"

#include "env/errors.hpp"
#include "env/frames.hpp"
#include "env/stack.hpp"
#include "env/string_interner.hpp"
#include "env/symbol_table.hpp"
#include "env/type_interner.hpp"
#include "imr/binding.hpp"

namespace inf {
class context {
    function       *fn;
    errors          errs;
    string_interner strings;
    type_interner   types;
    symbol_table    symbols;
    stack           eval_stack;
    frames          frame_stack;

  public:
    error::ptr fail(std::string message, location loc = {}) {
        errs.emplace_back(std::make_unique<error>(std::move(message), loc));
        return errs.back().get();
    }

    label string_intern(std::string_view view) {
        auto pair = strings.emplace(view);
        return {*pair.first};
    };

    type::ptr type_nil() noexcept { return types.get_nil(); }
    type::ptr type_integer() noexcept { return types.get_integer(); }
    type::ptr type_function(type::ptr return_type, type::function::arguments argument_types) {
        return types.get_function(return_type, std::move(argument_types));
    }

    std::optional<binding> lookup_binding(label name) {
        if (fn != nullptr) {
            function::local_iterator iter = fn->get_local(name);
            if (iter != fn->locals_end()) { return {*iter}; }
        }

        auto iter = symbols.find(name);
        if (iter == symbols.end()) { return std::nullopt; }

        return {iter->second};
    }

    auto global_emplace(label l, type::ptr type_, value value_) {
        return symbols.try_emplace(l, l, type_, value_);
    }

    bool push_function(label name) {
        auto iter = symbols.find(name);
        if (iter == symbols.end()) { return false; }

        symbol *sym = &(iter->second);
        BOOST_ASSERT(sym != nullptr);
        BOOST_ASSERT(sym->m_value.is<function>());
        fn = &sym->m_value.as<function>();

        uint64_t base = eval_stack.size();
        uint64_t size;
        BOOST_VERIFY(__builtin_add_overflow(base, fn->nlocals(), &size));
        eval_stack.resize(size);
        for (uint64_t i = 0; i < fn->nlocals(); ++i) {
            eval_stack.emplace_back();
        }
        frame_stack.emplace_back(fn, base);

        return true;
    }

    value &get_stack_slot(ssa loc) {
        BOOST_ASSERT(fn != nullptr);
        frame   &fm = frame_stack.back();
        uint64_t index;
        BOOST_VERIFY(__builtin_add_overflow(fm.m_base, loc.index, &index));
        return eval_stack.at(index);
    }

    void build_function(function *fn) {
        BOOST_ASSERT(fn != nullptr);
        this->fn = fn;
    }

    template <class... Args> ssa allocate_local(Args &&...args) noexcept {
        BOOST_ASSERT(fn != nullptr);
        return fn->allocate(std::forward<Args>(args)...);
    }

    local &get_local(ssa var) {
        BOOST_ASSERT(fn != nullptr);
        return fn->get_local(var);
    }

    void append_instruction(instruction inst) {
        BOOST_ASSERT(fn != nullptr);
        fn->append_instruction(inst);
    }
};
}; // namespace inf

#endif // !INF_ENV_CONTEXT_HPP
