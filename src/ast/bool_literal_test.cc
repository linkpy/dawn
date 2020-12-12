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

#include "src/ast/bool_literal.h"

#include "src/ast/float_literal.h"
#include "src/ast/null_literal.h"
#include "src/ast/sint_literal.h"
#include "src/ast/test_helper.h"
#include "src/ast/type/bool_type.h"
#include "src/ast/uint_literal.h"

namespace tint {
namespace ast {
namespace {

using BoolLiteralTest = TestHelper;

TEST_F(BoolLiteralTest, True) {
  type::Bool bool_type;
  BoolLiteral b{Source{}, &bool_type, true};
  ASSERT_TRUE(b.Is<BoolLiteral>());
  ASSERT_TRUE(b.IsTrue());
  ASSERT_FALSE(b.IsFalse());
}

TEST_F(BoolLiteralTest, False) {
  type::Bool bool_type;
  BoolLiteral b{Source{}, &bool_type, false};
  ASSERT_TRUE(b.Is<BoolLiteral>());
  ASSERT_FALSE(b.IsTrue());
  ASSERT_TRUE(b.IsFalse());
}

TEST_F(BoolLiteralTest, Is) {
  type::Bool bool_type;
  BoolLiteral b{Source{}, &bool_type, false};
  Literal* l = &b;
  EXPECT_TRUE(l->Is<BoolLiteral>());
  EXPECT_FALSE(l->Is<SintLiteral>());
  EXPECT_FALSE(l->Is<FloatLiteral>());
  EXPECT_FALSE(l->Is<UintLiteral>());
  EXPECT_FALSE(l->Is<IntLiteral>());
  EXPECT_FALSE(l->Is<NullLiteral>());
}

TEST_F(BoolLiteralTest, ToStr) {
  type::Bool bool_type;
  BoolLiteral t{Source{}, &bool_type, true};
  BoolLiteral f{Source{}, &bool_type, false};

  EXPECT_EQ(t.to_str(), "true");
  EXPECT_EQ(f.to_str(), "false");
}

}  // namespace
}  // namespace ast
}  // namespace tint
