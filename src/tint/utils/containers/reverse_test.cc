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

#include "src/tint/utils/containers/reverse.h"

#include <vector>

#include "gmock/gmock.h"

namespace tint {
namespace {

TEST(ReverseTest, Vector) {
    std::vector<int> vec{1, 3, 5, 7, 9};
    std::vector<int> rev;
    for (auto v : Reverse(vec)) {
        rev.emplace_back(v);
    }
    ASSERT_THAT(rev, testing::ElementsAre(9, 7, 5, 3, 1));
}

}  // namespace
}  // namespace tint
