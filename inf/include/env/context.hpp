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

#include "env/string_interner.hpp"
#include "env/symbol_table.hpp"
#include "env/locations.hpp"
#include "env/errors.hpp"

namespace inf {
class context {
    function *function_;
    errors errs;
    locations locs;
    string_interner strings;
    symbol_table symbols;

public:
    error::tag fail(std::string message, location loc) {
        location::tag tag = put_loc(loc);
        errs.emplace_back(std::move(message), tag);
        return {errs.size()};
    }

    error &get_error(error::tag tag) {
        return errs.at(tag.index);
    }

    location::tag put_loc(location loc) {
        locs.emplace_back(loc);
        return {locs.size()};
    }

    location &get_loc(location::tag tag) {
        return locs.at(tag.index);
    }

    label string_intern(std::string_view view) {
        auto pair = strings.emplace(view);
        return {*pair.first};
    };

    std::optional<symbol *> symbol_lookup(label l) {
        auto iter = symbols.find(l);
        if (iter == symbols.end()) {
            return std::nullopt;
        }

        return {&iter->second};
    }

    auto symbol_emplace(label l, value value_) {
        return symbols.try_emplace(l, l, value_);
    }

    void build_function(function *fn) {
        BOOST_ASSERT(fn != nullptr);
        function_ = fn;
    }

    void build_function(label l) {
        value v = inf::function{};
        auto pair = symbol_emplace(l, std::move(v));
        function_ = &(pair.first->second.value_.as<inf::function>());
    }

    ssa allocate_local() noexcept {
        BOOST_ASSERT(function_ != nullptr);
        return function_->allocate();
    }

    void append_instruction(instruction inst) {
        BOOST_ASSERT(function_ != nullptr);
        function_->append_instruction(inst);
    }
};
};

#endif // !INF_ENV_CONTEXT_HPP

