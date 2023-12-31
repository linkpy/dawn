// Copyright 2022 The Tint Authors.
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

#ifndef SRC_TINT_LANG_WGSL_AST_TRANSFORM_GET_INSERTION_POINT_H_
#define SRC_TINT_LANG_WGSL_AST_TRANSFORM_GET_INSERTION_POINT_H_

#include <utility>

#include "src/tint/lang/wgsl/ast/builder.h"
#include "src/tint/lang/wgsl/sem/block_statement.h"

// Forward declarations
namespace tint::program {
class CloneContext;
}

namespace tint::ast::transform::utils {

/// InsertionPoint is a pair of the block (`first`) within which, and the
/// statement (`second`) before or after which to insert.
using InsertionPoint = std::pair<const sem::BlockStatement*, const Statement*>;

/// For the input statement, returns the block and statement within that
/// block to insert before/after. If `stmt` is a for-loop continue statement,
/// the function returns {nullptr, nullptr} as we cannot insert before/after it.
/// @param ctx the clone context
/// @param stmt the statement to insert before or after
/// @return the insertion point
InsertionPoint GetInsertionPoint(program::CloneContext& ctx, const Statement* stmt);

}  // namespace tint::ast::transform::utils

#endif  // SRC_TINT_LANG_WGSL_AST_TRANSFORM_GET_INSERTION_POINT_H_
