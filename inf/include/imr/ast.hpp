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
#ifndef INF_IMR_AST_HPP
#define INF_IMR_AST_HPP

#include <memory>
#include <variant>

#include "imr/label.hpp"
#include "imr/type.hpp"
#include "imr/value.hpp"
#include "imr/error.hpp"

namespace inf {
class Ast {
  public:
    using Ptr = std::unique_ptr<Ast>;

    struct Binding {
        Label     label;
        Type::Ptr type;
        Ptr       expression;
    };

    struct Unop {
        enum class Opcode {
            Negate,
        } opcode;

        Ptr expression;
    };

    struct Binop {
        enum class Opcode {
            Add,
            Subtract,
            Multiply,
            Divide,
            Modulo,
        } opcode;

        Ptr left;
        Ptr right;
    };

    using Variant = std::variant<Value, Binding, Unop, Binop, Error>;

  private:
    Variant variant;

  public:
    Ast() : variant() {}
    Ast(Ast &&ast) : variant(std::move(ast.variant)) {}
    Ast(Ast const &ast) = delete;
    template <class T> Ast(T &&t) : variant(std::move(t)) {}
    template <class T> Ast(T const &t) = delete;

    template <class T> Ast &operator=(T &&t) {
        variant = std::move(t);
        return *this;
    }

    template <class T> Ast &operator=(T const &t) = delete;

    Ast &operator=(Ast &&other) {
        if (this == &other) { return *this; }

        variant = std::move(other.variant);
        return *this;
    }

    Ast &operator=(Ast const &other) = delete;

    Variant       &get() noexcept { return variant; }
    Variant const &get() const noexcept { return variant; }

    template <class T> bool is() const noexcept {
        return std::holds_alternative<T>(variant);
    }

    template <class T> T       &as() { return std::get<T>(variant); }
    template <class T> T const &as() const { return std::get<T>(variant); }

    template <class T> static Ptr create(T &&t) {
        return std::make_unique<Ast>(std::move(t));
    }

    static Ptr binding(Label label, Type::Ptr type, Ptr expression) {
        return create(Binding{label, std::move(type), std::move(expression)});
    }

    static Ptr negate(Ptr expression) {
        return create(Unop{Unop::Opcode::Negate, std::move(expression)});
    }

    static Ptr add(Ptr left, Ptr right) {
        return create(
            Binop{Binop::Opcode::Add, std::move(left), std::move(right)});
    }

    static Ptr subtract(Ptr left, Ptr right) {
        return create(
            Binop{Binop::Opcode::Subtract, std::move(left), std::move(right)});
    }

    static Ptr multiply(Ptr left, Ptr right) {
        return create(
            Binop{Binop::Opcode::Multiply, std::move(left), std::move(right)});
    }

    static Ptr divide(Ptr left, Ptr right) {
        return create(
            Binop{Binop::Opcode::Divide, std::move(left), std::move(right)});
    }

    static Ptr modulo(Ptr left, Ptr right) {
        return create(
            Binop{Binop::Opcode::Modulo, std::move(left), std::move(right)});
    }
};
} // namespace inf

#endif // !INF_IMR_AST_HPP
