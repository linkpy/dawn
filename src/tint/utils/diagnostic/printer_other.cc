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

// GEN_BUILD:CONDITION((!is_linux) && (!is_mac) && (!is_win))

#include <cstring>

#include "src/tint/utils/diagnostic/printer.h"

namespace tint::diag {
namespace {

class PrinterOther : public Printer {
  public:
    explicit PrinterOther(FILE* f) : file(f) {}

    void write(const std::string& str, const Style&) override {
        fwrite(str.data(), 1, str.size(), file);
    }

  private:
    FILE* file;
};

}  // namespace

std::unique_ptr<Printer> Printer::create(FILE* out, bool) {
    return std::make_unique<PrinterOther>(out);
}

}  // namespace tint::diag
