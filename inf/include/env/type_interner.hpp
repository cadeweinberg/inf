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
#ifndef INF_ENV_TYPE_INTERNER_HPP
#define INF_ENV_TYPE_INTERNER_HPP

#include <memory>

#include "imr/type.hpp"

namespace inf {
class type_interner {
    struct list {
        using vector = std::vector<std::unique_ptr<type>>;
        vector buffer;

        template <class... Args> type::ptr insert(Args &&...args) {
            auto element = std::make_unique<type>(std::forward<Args>(args)...);
            vector::iterator iter = std::ranges::find_if(
                buffer,
                [&element](std::unique_ptr<type> const &handle) {
                    return *handle == *element;
                });
            if (iter == buffer.end()) {
                buffer.emplace_back(std::move(element));
                return buffer.back().get();
            }

            return iter->get();
        }
    };

    std::unique_ptr<type> nil;
    std::unique_ptr<type> integer;
    list                  functions;

  public:
    type_interner()
        : nil(std::make_unique<type>(std::monostate{})),
          integer(std::make_unique<type>(inf::integer{0})),
          functions{} {}

    type::ptr get_nil() noexcept { return nil.get(); }
    type::ptr get_integer() noexcept { return integer.get(); }
    type::ptr get_function(type::ptr return_type, type::function::arguments argument_types) {
        return functions.insert(type::function{return_type, std::move(argument_types)});
    }
};
} // namespace inf

#endif // !INF_ENV_TYPE_INTERNER_HPP
