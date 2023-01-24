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

////////////////////////////////////////////////////////////////////////////////
// File generated by tools/src/cmd/gen
// using the template:
//   src/tint/ast/diagnostic_control_bench.cc.tmpl
//
// Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#include "src/tint/ast/diagnostic_control.h"

#include <array>

#include "benchmark/benchmark.h"

namespace tint::ast {
namespace {

void DiagnosticSeverityParser(::benchmark::State& state) {
    std::array kStrings{
        "erccr",    "3o",        "eVror",   "error",   "erro1",  "qqrJr",  "errll7r",
        "ppqnfH",   "c",         "iGf",     "info",    "invii",  "inWWo",  "Mxxo",
        "ogg",      "X",         "3ff",     "off",     "oEf",    "oPTT",   "dxxf",
        "w44rning", "waSSniVVg", "RarR22g", "warning", "wFni9g", "waring", "VOORRHng",
    };
    for (auto _ : state) {
        for (auto& str : kStrings) {
            auto result = ParseDiagnosticSeverity(str);
            benchmark::DoNotOptimize(result);
        }
    }
}

BENCHMARK(DiagnosticSeverityParser);

void DiagnosticRuleParser(::benchmark::State& state) {
    std::array kStrings{
        "hromium_unyeachable_code",   "chrorrillmGunnreachable_c77de", "chromium_unreachable4cod00",
        "chromium_unreachable_code",  "chromium_unracaboo_code",       "chromium_unrzzchabl_code",
        "ciipp11ium_unreachable_cod",
    };
    for (auto _ : state) {
        for (auto& str : kStrings) {
            auto result = ParseDiagnosticRule(str);
            benchmark::DoNotOptimize(result);
        }
    }
}

BENCHMARK(DiagnosticRuleParser);

}  // namespace
}  // namespace tint::ast
