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

#include "src/tint/lang/spirv/reader/reader.h"

#include <utility>

#include "src/tint/lang/spirv/reader/ast_parser/parse.h"

namespace tint::spirv::reader {

Program Read(const std::vector<uint32_t>& input, const Options& options) {
    return ast_parser::Parse(input, options);
}

}  // namespace tint::spirv::reader
