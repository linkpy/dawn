// Copyright 2023 The Tint Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "src/tint/lang/wgsl/sem/accessor_expression.h"

#include <utility>

#include "src/tint/lang/wgsl/ast/index_accessor_expression.h"

TINT_INSTANTIATE_TYPEINFO(tint::sem::AccessorExpression);

namespace tint::sem {

AccessorExpression::AccessorExpression(const ast::AccessorExpression* declaration,
                                       const core::type::Type* type,
                                       core::EvaluationStage stage,
                                       const ValueExpression* object,
                                       const Statement* statement,
                                       const core::constant::Value* constant,
                                       bool has_side_effects,
                                       const Variable* root_ident /* = nullptr */)
    : Base(declaration, type, stage, statement, constant, has_side_effects, root_ident),
      object_(object) {}

AccessorExpression::~AccessorExpression() = default;

}  // namespace tint::sem
