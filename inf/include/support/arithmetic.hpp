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
#ifndef INF_SUPPORT_ARITHMETIC_HPP
#define INF_SUPPORT_ARITHMETIC_HPP

#include <cstdint>

namespace inf {
inline uint64_t sra(uint64_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint64_t         result              = value;
    __asm__("sarq %[shamt], %[result]"
            : [result] "+r"(result)
            : [shamt] "r"(shamt));
    return result;
}

inline uint32_t sra(uint32_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint32_t         result              = value;
    __asm__("sarl %[shamt], %[result]"
            : [result] "+r"(result)
            : [shamt] "r"(shamt));
    return result;
}

inline uint16_t sra(uint16_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint16_t         result              = value;
    __asm__("sarw %[shamt], %[result]"
            : [result] "+r"(result)
            : [shamt] "r"(shamt));
    return result;
}

inline uint8_t sra(uint8_t value, uint64_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint8_t          result              = value;
    __asm__("sarb %[shamt], %[result]"
            : [result] "+r"(result)
            : [shamt] "r"(shamt));
    return result;
}

inline uint64_t srl(uint64_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint64_t         result              = value;
    __asm__("shrq %[shamt], %[result]"
            : [result] "+r"(result)
            : [shamt] "r"(shamt));
    return result;
}

inline uint32_t srl(uint32_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint32_t         result              = value;
    __asm__("shrl %[shamt], %[result]"
            : [result] "+r"(result)
            : [shamt] "r"(shamt));
    return result;
}

inline uint16_t srl(uint16_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint16_t         result              = value;
    __asm__("shrw %[shamt], %[result]"
            : [result] "+r"(result)
            : [shamt] "r"(shamt));
    return result;
}

inline uint8_t srl(uint8_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint8_t          result              = value;
    __asm__("shrb %[shamt], %[result]"
            : [result] "+r"(result)
            : [shamt] "r"(shamt));
    return result;
}

inline uint64_t sla(uint64_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint64_t         result              = value;
    __asm__("salq %[shamt], %[result]"
    : [result] "+r"(result)
    : [shamt] "r"(shamt));
    return result;
}

inline uint32_t sla(uint32_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint32_t         result              = value;
    __asm__("sall %[shamt], %[result]"
    : [result] "+r"(result)
    : [shamt] "r"(shamt));
    return result;
}

inline uint16_t sla(uint16_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint16_t         result              = value;
    __asm__("salw %[shamt], %[result]"
    : [result] "+r"(result)
    : [shamt] "r"(shamt));
    return result;
}

inline uint8_t sla(uint8_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint8_t         result              = value;
    __asm__("salb %[shamt], %[result]"
    : [result] "+r"(result)
    : [shamt] "r"(shamt));
    return result;
}

inline uint64_t sll(uint64_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint64_t         result              = value;
    __asm__("shlq %[shamt], %[result]"
    : [result] "+r"(result)
    : [shamt] "r"(shamt));
    return result;
}

inline uint32_t sll(uint32_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint32_t         result              = value;
    __asm__("shll %[shamt], %[result]"
    : [result] "+r"(result)
    : [shamt] "r"(shamt));
    return result;
}

inline uint16_t sll(uint16_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint16_t         result              = value;
    __asm__("shlw %[shamt], %[result]"
    : [result] "+r"(result)
    : [shamt] "r"(shamt));
    return result;
}

inline uint8_t sll(uint8_t value, uint8_t amount) {
    register uint8_t shamt __asm__("cl") = (uint8_t)(amount & 0x1F);
    uint8_t         result              = value;
    __asm__("shlb %[shamt], %[result]"
    : [result] "+r"(result)
    : [shamt] "r"(shamt));
    return result;
}
}; // namespace inf

#endif // !INF_SUPPORT_ARITHMETIC_HPP
