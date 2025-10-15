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
    std::unique_ptr<type> u64;
    std::unique_ptr<type> u32;
    std::unique_ptr<type> u16;
    std::unique_ptr<type> u8;
    std::unique_ptr<type> i64;
    std::unique_ptr<type> i32;
    std::unique_ptr<type> i16;
    std::unique_ptr<type> i8;
    list                  functions;

  public:
    type_interner()
        : nil(std::make_unique<type>(std::monostate{})),
          u64(std::make_unique<type>(uint64_t{})),
          u32(std::make_unique<type>(uint32_t{})),
          u16(std::make_unique<type>(uint16_t{})),
          u8(std::make_unique<type>(uint8_t{})),
          i64(std::make_unique<type>(int64_t{})),
          i32(std::make_unique<type>(int32_t{})),
          i16(std::make_unique<type>(int16_t{})),
          i8(std::make_unique<type>(int8_t{})),
          functions{} {}

    type::ptr get_nil() noexcept { return nil.get(); }
    type::ptr get_u64() noexcept { return u64.get(); }
    type::ptr get_u32() noexcept { return u32.get(); }
    type::ptr get_u16() noexcept { return u16.get(); }
    type::ptr get_u8() noexcept { return u8.get(); }
    type::ptr get_i64() noexcept { return i64.get(); }
    type::ptr get_i32() noexcept { return i32.get(); }
    type::ptr get_i16() noexcept { return i16.get(); }
    type::ptr get_i8() noexcept { return i8.get(); }
    type::ptr get_function(type::ptr return_type, type::function::arguments argument_types) {
        return functions.insert(type::function{return_type, std::move(argument_types)});
    }
};
} // namespace inf

#endif // !INF_ENV_TYPE_INTERNER_HPP
