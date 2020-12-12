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

#include "gtest/gtest.h"
#include "src/ast/bool_literal.h"
#include "src/ast/float_literal.h"
#include "src/ast/identifier_expression.h"
#include "src/ast/scalar_constructor_expression.h"
#include "src/ast/sint_literal.h"
#include "src/ast/type/bool_type.h"
#include "src/ast/type/f32_type.h"
#include "src/ast/type/i32_type.h"
#include "src/ast/type/vector_type.h"
#include "src/ast/unary_op_expression.h"
#include "src/type_determiner.h"
#include "src/writer/spirv/builder.h"
#include "src/writer/spirv/spv_dump.h"
#include "src/writer/spirv/test_helper.h"

namespace tint {
namespace writer {
namespace spirv {
namespace {

using BuilderTest = TestHelper;

TEST_F(BuilderTest, UnaryOp_Negation_Integer) {
  ast::type::I32 i32;

  ast::UnaryOpExpression expr(ast::UnaryOp::kNegation,
                              create<ast::ScalarConstructorExpression>(
                                  create<ast::SintLiteral>(Source{}, &i32, 1)));

  ASSERT_TRUE(td.DetermineResultType(&expr)) << td.error();

  b.push_function(Function{});
  EXPECT_EQ(b.GenerateUnaryOpExpression(&expr), 1u) << b.error();
  EXPECT_EQ(DumpInstructions(b.types()), R"(%2 = OpTypeInt 32 1
%3 = OpConstant %2 1
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].instructions()),
            R"(%1 = OpSNegate %2 %3
)");
}

TEST_F(BuilderTest, UnaryOp_Negation_Float) {
  ast::type::F32 f32;

  ast::UnaryOpExpression expr(
      ast::UnaryOp::kNegation,
      create<ast::ScalarConstructorExpression>(
          create<ast::FloatLiteral>(Source{}, &f32, 1)));

  ASSERT_TRUE(td.DetermineResultType(&expr)) << td.error();

  b.push_function(Function{});
  EXPECT_EQ(b.GenerateUnaryOpExpression(&expr), 1u) << b.error();
  EXPECT_EQ(DumpInstructions(b.types()), R"(%2 = OpTypeFloat 32
%3 = OpConstant %2 1
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].instructions()),
            R"(%1 = OpFNegate %2 %3
)");
}

TEST_F(BuilderTest, UnaryOp_Not) {
  ast::type::Bool bool_type;

  ast::UnaryOpExpression expr(
      ast::UnaryOp::kNot,
      create<ast::ScalarConstructorExpression>(
          create<ast::BoolLiteral>(Source{}, &bool_type, false)));

  ASSERT_TRUE(td.DetermineResultType(&expr)) << td.error();

  b.push_function(Function{});
  EXPECT_EQ(b.GenerateUnaryOpExpression(&expr), 1u) << b.error();
  EXPECT_EQ(DumpInstructions(b.types()), R"(%2 = OpTypeBool
%3 = OpConstantFalse %2
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].instructions()),
            R"(%1 = OpLogicalNot %2 %3
)");
}

TEST_F(BuilderTest, UnaryOp_LoadRequired) {
  ast::type::F32 f32;
  ast::type::Vector vec(&f32, 3);

  ast::Variable var(Source{}, "param", ast::StorageClass::kFunction, &vec,
                    false, nullptr, ast::VariableDecorationList{});

  ast::UnaryOpExpression expr(
      ast::UnaryOp::kNegation,
      create<ast::IdentifierExpression>(mod->RegisterSymbol("param"), "param"));

  td.RegisterVariableForTesting(&var);
  EXPECT_TRUE(td.DetermineResultType(&expr)) << td.error();

  b.push_function(Function{});
  EXPECT_TRUE(b.GenerateFunctionVariable(&var)) << b.error();
  EXPECT_EQ(b.GenerateUnaryOpExpression(&expr), 6u) << b.error();
  ASSERT_FALSE(b.has_error()) << b.error();

  EXPECT_EQ(DumpInstructions(b.types()), R"(%4 = OpTypeFloat 32
%3 = OpTypeVector %4 3
%2 = OpTypePointer Function %3
%5 = OpConstantNull %3
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].variables()),
            R"(%1 = OpVariable %2 Function %5
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].instructions()),
            R"(%7 = OpLoad %3 %1
%6 = OpFNegate %3 %7
)");
}

}  // namespace
}  // namespace spirv
}  // namespace writer
}  // namespace tint
