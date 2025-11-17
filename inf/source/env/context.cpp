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

#include "env/context.hpp"

#include "llvm/TargetParser/Host.h"
#include "llvm/MC/TargetRegistry.h"

namespace inf {
std::string Context::host_cpu_features() noexcept {
    std::string                 features;
    const llvm::StringMap<bool> feature_map = llvm::sys::getHostCPUFeatures();

    return features;
}

Context::Context(Label module_name)
    : llvm_context(), llvm_module(module_name, llvm_context),
      llvm_ir_builder(llvm_context) {
    llvm::StringRef cpu    = llvm::sys::getHostCPUName();
    std::string triple = llvm::sys::getProcessTriple();
    std::string cpu_features = host_cpu_features();
    std::string error_string;
    const llvm::Target *target =
        llvm::TargetRegistry::lookupTarget(triple, error_string);

    llvm_target_machine = target->createTargetMachine(triple, cpu, cpu_features, );
}

ErrorList::size_type Context::error(Error error) {
    error_list.emplace_back(std::move(error));
    return error_list.size() - 1;
}

Error const &Context::error_at(ErrorList::size_type index) const {
    return error_list.at(index);
}

Label Context::intern_string(llvm::StringRef string) {
    auto [iter, cons] = string_interner.insert(string);
    return iter->getKey();
}
} // namespace inf
