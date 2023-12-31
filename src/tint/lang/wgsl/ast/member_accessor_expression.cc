// Copyright 2020 The Tint Authors.
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

#include "src/tint/lang/wgsl/ast/member_accessor_expression.h"

#include "src/tint/lang/wgsl/ast/builder.h"
#include "src/tint/lang/wgsl/ast/clone_context.h"

TINT_INSTANTIATE_TYPEINFO(tint::ast::MemberAccessorExpression);

namespace tint::ast {

MemberAccessorExpression::MemberAccessorExpression(GenerationID pid,
                                                   NodeID nid,
                                                   const Source& src,
                                                   const Expression* obj,
                                                   const Identifier* mem)
    : Base(pid, nid, src, obj), member(mem) {
    TINT_ASSERT(member);
    TINT_ASSERT_GENERATION_IDS_EQUAL_IF_VALID(member, generation_id);

    // It is currently invalid for a structure to hold a templated member
    if (member) {
        TINT_ASSERT(!member->Is<TemplatedIdentifier>());
    }
}

MemberAccessorExpression::~MemberAccessorExpression() = default;

const MemberAccessorExpression* MemberAccessorExpression::Clone(CloneContext& ctx) const {
    // Clone arguments outside of create() call to have deterministic ordering
    auto src = ctx.Clone(source);
    auto* obj = ctx.Clone(object);
    auto* mem = ctx.Clone(member);
    return ctx.dst->create<MemberAccessorExpression>(src, obj, mem);
}

}  // namespace tint::ast
