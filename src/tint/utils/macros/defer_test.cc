// Copyright 2021 The Tint Authors.
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

#include "src/tint/utils/macros/defer.h"

#include "gtest/gtest.h"

namespace tint {
namespace {

TEST(DeferTest, Basic) {
    bool deferCalled = false;
    { TINT_DEFER(deferCalled = true); }
    ASSERT_TRUE(deferCalled);
}

TEST(DeferTest, DeferOrder) {
    int counter = 0;
    int a = 0, b = 0, c = 0;
    {
        TINT_DEFER(a = ++counter);
        TINT_DEFER(b = ++counter);
        TINT_DEFER(c = ++counter);
    }
    ASSERT_EQ(a, 3);
    ASSERT_EQ(b, 2);
    ASSERT_EQ(c, 1);
}

}  // namespace
}  // namespace tint
