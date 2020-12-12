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

#include <memory>

#include "src/ast/break_statement.h"
#include "src/ast/case_statement.h"
#include "src/ast/fallthrough_statement.h"
#include "src/ast/identifier_expression.h"
#include "src/ast/module.h"
#include "src/ast/sint_literal.h"
#include "src/ast/type/i32_type.h"
#include "src/writer/hlsl/test_helper.h"

namespace tint {
namespace writer {
namespace hlsl {
namespace {

using HlslGeneratorImplTest_Case = TestHelper;

TEST_F(HlslGeneratorImplTest_Case, Emit_Case) {
  ast::type::I32 i32;

  auto* body = create<ast::BlockStatement>();
  body->append(create<ast::BreakStatement>());

  ast::CaseSelectorList lit;
  lit.push_back(create<ast::SintLiteral>(Source{}, &i32, 5));
  ast::CaseStatement c(lit, body);

  gen.increment_indent();

  ASSERT_TRUE(gen.EmitCase(out, &c)) << gen.error();
  EXPECT_EQ(result(), R"(  case 5: {
    break;
  }
)");
}

TEST_F(HlslGeneratorImplTest_Case, Emit_Case_BreaksByDefault) {
  ast::type::I32 i32;

  ast::CaseSelectorList lit;
  lit.push_back(create<ast::SintLiteral>(Source{}, &i32, 5));
  ast::CaseStatement c(lit, create<ast::BlockStatement>());

  gen.increment_indent();

  ASSERT_TRUE(gen.EmitCase(out, &c)) << gen.error();
  EXPECT_EQ(result(), R"(  case 5: {
    break;
  }
)");
}

TEST_F(HlslGeneratorImplTest_Case, Emit_Case_WithFallthrough) {
  ast::type::I32 i32;

  auto* body = create<ast::BlockStatement>();
  body->append(create<ast::FallthroughStatement>());

  ast::CaseSelectorList lit;
  lit.push_back(create<ast::SintLiteral>(Source{}, &i32, 5));
  ast::CaseStatement c(lit, body);

  gen.increment_indent();

  ASSERT_TRUE(gen.EmitCase(out, &c)) << gen.error();
  EXPECT_EQ(result(), R"(  case 5: {
    /* fallthrough */
  }
)");
}

TEST_F(HlslGeneratorImplTest_Case, Emit_Case_MultipleSelectors) {
  ast::type::I32 i32;

  auto* body = create<ast::BlockStatement>();
  body->append(create<ast::BreakStatement>());

  ast::CaseSelectorList lit;
  lit.push_back(create<ast::SintLiteral>(Source{}, &i32, 5));
  lit.push_back(create<ast::SintLiteral>(Source{}, &i32, 6));
  ast::CaseStatement c(lit, body);

  gen.increment_indent();

  ASSERT_TRUE(gen.EmitCase(out, &c)) << gen.error();
  EXPECT_EQ(result(), R"(  case 5:
  case 6: {
    break;
  }
)");
}

TEST_F(HlslGeneratorImplTest_Case, Emit_Case_Default) {
  auto* body = create<ast::BlockStatement>();
  body->append(create<ast::BreakStatement>());
  ast::CaseStatement c(body);

  gen.increment_indent();

  ASSERT_TRUE(gen.EmitCase(out, &c)) << gen.error();
  EXPECT_EQ(result(), R"(  default: {
    break;
  }
)");
}

}  // namespace
}  // namespace hlsl
}  // namespace writer
}  // namespace tint
