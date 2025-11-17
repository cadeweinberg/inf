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

#include "env/error_list.hpp"
#include "imr/label.hpp"

#include "llvm/ADT/StringSet.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Target/TargetMachine.h"

namespace inf {
class Context {
    llvm::TargetMachine *llvm_target_machine;
    llvm::LLVMContext    llvm_context;
    llvm::Module         llvm_module;
    llvm::IRBuilder<>    llvm_ir_builder;
    llvm::StringSet<>    string_interner;
    ErrorList            error_list;

    static std::string host_cpu_features() noexcept;

  public:
    Context(Label module_name);

    ErrorList::size_type error(Error error);
    Error const         &error_at(ErrorList::size_type index) const;

    Label intern_string(llvm::StringRef string);
};
} // namespace inf

#endif // !INF_ENV_CONTEXT_HPP
