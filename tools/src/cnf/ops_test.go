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

package cnf_test

import (
	"testing"

	"dawn.googlesource.com/dawn/tools/src/cnf"
	"github.com/google/go-cmp/cmp"
)

func TestAnd(t *testing.T) {
	type E = cnf.Expr
	for _, test := range []struct {
		lhs, rhs cnf.Expr
		expect   cnf.Expr
	}{
		{ // And('', '') => ''
			lhs: E{}, rhs: E{},
			expect: E{},
		},
		{ // And('X', '') => 'X'
			lhs: E{{T("X")}}, rhs: E{},
			expect: E{{T("X")}},
		},
		{ // And('!X', '') => '!X'
			lhs: E{{F("X")}}, rhs: E{},
			expect: E{{F("X")}},
		},
		{ // And('', 'X') => 'X'
			lhs: E{}, rhs: E{{T("X")}},
			expect: E{{T("X")}},
		},
		{ // And('', '!X') => '!X'
			lhs: E{}, rhs: E{{F("X")}},
			expect: E{{F("X")}},
		},
		{ // And('X', 'Y') => 'X && Y'
			lhs: E{{T("X")}}, rhs: E{{T("Y")}},
			expect: E{{T("X")}, {T("Y")}},
		},
		{ // And('X', '!Y') => 'X && !Y'
			lhs: E{{T("X")}}, rhs: E{{F("Y")}},
			expect: E{{T("X")}, {F("Y")}},
		},
		{ // And('Y', 'X') => 'Y && X'
			lhs: E{{T("Y")}}, rhs: E{{T("X")}},
			expect: E{{T("Y")}, {T("X")}},
		},
		{ // And('!Y', 'X') => '!Y && X'
			lhs: E{{F("Y")}}, rhs: E{{T("X")}},
			expect: E{{F("Y")}, {T("X")}},
		},
		{ // And('X && Y', 'Z') => 'X && Y && Z'
			lhs: E{{T("X")}, {T("Y")}}, rhs: E{{T("Z")}},
			expect: E{{T("X")}, {T("Y")}, {T("Z")}},
		},
		{ // And('!X && Y', 'Z') => '!X && Y && Z'
			lhs: E{{F("X")}, {T("Y")}}, rhs: E{{T("Z")}},
			expect: E{{F("X")}, {T("Y")}, {T("Z")}},
		},
		{ // And('X && !Y', 'Z') => 'X && !Y && Z'
			lhs: E{{T("X")}, {F("Y")}}, rhs: E{{T("Z")}},
			expect: E{{T("X")}, {F("Y")}, {T("Z")}},
		},
		{ // And('X && Y', '!Z') => 'X && Y && !Z'
			lhs: E{{T("X")}, {T("Y")}}, rhs: E{{F("Z")}},
			expect: E{{T("X")}, {T("Y")}, {F("Z")}},
		},
		{ // And('X', 'Y && Z') => 'X && Y && Z'
			lhs: E{{T("X")}}, rhs: E{{T("Y")}, {T("Z")}},
			expect: E{{T("X")}, {T("Y")}, {T("Z")}},
		},
		{ // And('!X', 'Y && Z') => '!X && Y && Z'
			lhs: E{{F("X")}}, rhs: E{{T("Y")}, {T("Z")}},
			expect: E{{F("X")}, {T("Y")}, {T("Z")}},
		},
		{ // And('X', '!Y && Z') => 'X && !Y && Z'
			lhs: E{{T("X")}}, rhs: E{{F("Y")}, {T("Z")}},
			expect: E{{T("X")}, {F("Y")}, {T("Z")}},
		},
		{ // And('X', 'Y && !Z') => 'X && Y && !Z'
			lhs: E{{T("X")}}, rhs: E{{T("Y")}, {F("Z")}},
			expect: E{{T("X")}, {T("Y")}, {F("Z")}},
		},
		{ // And('X && Y', 'Y && Z') => 'X && Y && Y && Z'
			lhs: E{{T("X")}, {T("Y")}}, rhs: E{{T("Y")}, {T("Z")}},
			expect: E{{T("X")}, {T("Y")}, {T("Y")}, {T("Z")}},
		},
		{ // And('!X && Y', 'Y && !Z') => '!X && Y && Y && !Z'
			lhs: E{{F("X")}, {T("Y")}}, rhs: E{{T("Y")}, {F("Z")}},
			expect: E{{F("X")}, {T("Y")}, {T("Y")}, {F("Z")}},
		},
		{ // And('X || Y', 'Z') => '(X || Y) && Z'
			lhs: E{{T("X"), T("Y")}}, rhs: E{{T("Z")}},
			expect: E{{T("X"), T("Y")}, {T("Z")}},
		},
		{ // And('!X || Y', 'Z') => '(!X || Y) && Z'
			lhs: E{{F("X"), T("Y")}}, rhs: E{{T("Z")}},
			expect: E{{F("X"), T("Y")}, {T("Z")}},
		},
		{ // And('X || !Y', 'Z') => '(X || !Y) && Z'
			lhs: E{{T("X"), F("Y")}}, rhs: E{{T("Z")}},
			expect: E{{T("X"), F("Y")}, {T("Z")}},
		},
		{ // And('X || Y', '!Z') => '(X || Y) && !Z'
			lhs: E{{T("X"), T("Y")}}, rhs: E{{F("Z")}},
			expect: E{{T("X"), T("Y")}, {F("Z")}},
		},
		{ // And('X', 'Y || Z') => 'X && (Y || Z)'
			lhs: E{{T("X")}}, rhs: E{{T("Y"), T("Z")}},
			expect: E{{T("X")}, {T("Y"), T("Z")}},
		},
		{ // And('!X', 'Y || Z') => '!X && (Y || Z)'
			lhs: E{{F("X")}}, rhs: E{{T("Y"), T("Z")}},
			expect: E{{F("X")}, {T("Y"), T("Z")}},
		},
		{ // And('X', '!Y || Z') => 'X && (!Y || Z)'
			lhs: E{{T("X")}}, rhs: E{{F("Y"), T("Z")}},
			expect: E{{T("X")}, {F("Y"), T("Z")}},
		},
		{ // And('X', 'Y || !Z') => 'X && (Y || !Z)'
			lhs: E{{T("X")}}, rhs: E{{T("Y"), F("Z")}},
			expect: E{{T("X")}, {T("Y"), F("Z")}},
		},
		{ // And('X || Y', 'Y || Z') => '(X || Y) && (Y || Z)'
			lhs: E{{T("X"), T("Y")}}, rhs: E{{T("Y"), T("Z")}},
			expect: E{{T("X"), T("Y")}, {T("Y"), T("Z")}},
		},
		{ // And('!X || Y', 'Y || !Z') => '(!X || Y) && (Y || !Z)'
			lhs: E{{F("X"), T("Y")}}, rhs: E{{T("Y"), F("Z")}},
			expect: E{{F("X"), T("Y")}, {T("Y"), F("Z")}},
		},
		{ // And('X || Y', 'Y && Z') => '(X || Y) && Y && Z'
			lhs: E{{T("X"), T("Y")}}, rhs: E{{T("Y")}, {T("Z")}},
			expect: E{{T("X"), T("Y")}, {T("Y")}, {T("Z")}},
		},
		{ // And('!X && Y', 'Y || !Z') => '!X && Y && (Y || !Z)'
			lhs: E{{F("X")}, {T("Y")}}, rhs: E{{T("Y"), F("Z")}},
			expect: E{{F("X")}, {T("Y")}, {T("Y"), F("Z")}},
		},
	} {
		got := cnf.And(test.lhs, test.rhs)
		if diff := cmp.Diff(test.expect, got); diff != "" {
			t.Errorf("And('%v', '%v') returned '%v'. Diff:\n%v", test.lhs, test.rhs, got, diff)
		}
	}
}

func TestOr(t *testing.T) {
	type E = cnf.Expr
	for _, test := range []struct {
		lhs, rhs cnf.Expr
		expect   cnf.Expr
	}{
		{ // Or('', '') => ''
			lhs: E{}, rhs: E{},
			expect: E{},
		},
		{ // Or('X', '') => 'X'
			lhs: E{{T("X")}}, rhs: E{},
			expect: E{{T("X")}},
		},
		{ // Or('!X', '') => '!X'
			lhs: E{{F("X")}}, rhs: E{},
			expect: E{{F("X")}},
		},
		{ // Or('', 'X') => 'X'
			lhs: E{}, rhs: E{{T("X")}},
			expect: E{{T("X")}},
		},
		{ // Or('', '!X') => '!X'
			lhs: E{}, rhs: E{{F("X")}},
			expect: E{{F("X")}},
		},
		{ // Or('X', 'Y') => 'X || Y'
			lhs: E{{T("X")}}, rhs: E{{T("Y")}},
			expect: E{{T("X"), T("Y")}},
		},
		{ // Or('X', '!Y') => 'X || !Y'
			lhs: E{{T("X")}}, rhs: E{{F("Y")}},
			expect: E{{T("X"), F("Y")}},
		},
		{ // Or('Y', 'X') => 'Y || X'
			lhs: E{{T("Y")}}, rhs: E{{T("X")}},
			expect: E{{T("Y"), T("X")}},
		},
		{ // Or('!Y', 'X') => '!Y || X'
			lhs: E{{F("Y")}}, rhs: E{{T("X")}},
			expect: E{{F("Y"), T("X")}},
		},
		{ // Or('X && Y', 'Z') => '(X || Z) && (Y || Z)'
			lhs: E{{T("X")}, {T("Y")}}, rhs: E{{T("Z")}},
			expect: E{{T("X"), T("Z")}, {T("Y"), T("Z")}},
		},
		{ // Or('!X && Y', 'Z') => '(!X || Z) && (Y || Z)'
			lhs: E{{F("X")}, {T("Y")}}, rhs: E{{T("Z")}},
			expect: E{{F("X"), T("Z")}, {T("Y"), T("Z")}},
		},
		{ // Or('X && !Y', 'Z') => '(X || Z) && (!Y || Z)'
			lhs: E{{T("X")}, {F("Y")}}, rhs: E{{T("Z")}},
			expect: E{{T("X"), T("Z")}, {F("Y"), T("Z")}},
		},
		{ // Or('X && Y', '!Z') => '(X || !Z) && (Y || !Z)'
			lhs: E{{T("X")}, {T("Y")}}, rhs: E{{F("Z")}},
			expect: E{{T("X"), F("Z")}, {T("Y"), F("Z")}},
		},
		{ // Or('X', 'Y && Z') => '(X || Y) && (X || Z)'
			lhs: E{{T("X")}}, rhs: E{{T("Y")}, {T("Z")}},
			expect: E{{T("X"), T("Y")}, {T("X"), T("Z")}},
		},
		{ // Or('!X', 'Y && Z') => '(!X || Y) && (!X || Z)'
			lhs: E{{F("X")}}, rhs: E{{T("Y")}, {T("Z")}},
			expect: E{{F("X"), T("Y")}, {F("X"), T("Z")}},
		},
		{ // Or('X', '!Y && Z') => '(X || !Y) && (X || Z)'
			lhs: E{{T("X")}}, rhs: E{{F("Y")}, {T("Z")}},
			expect: E{{T("X"), F("Y")}, {T("X"), T("Z")}},
		},
		{ // Or('X', 'Y && !Z') => '(X || Y) && (X || !Z)'
			lhs: E{{T("X")}}, rhs: E{{T("Y")}, {F("Z")}},
			expect: E{{T("X"), T("Y")}, {T("X"), F("Z")}},
		},
		{ // Or('X && Y', 'Z && W') => '(X || Z) && (X || W) && (Y || Z) && (Y || W)'
			lhs: E{{T("X")}, {T("Y")}}, rhs: E{{T("Z")}, {T("W")}},
			expect: E{{T("X"), T("Z")}, {T("X"), T("W")}, {T("Y"), T("Z")}, {T("Y"), T("W")}},
		},
		{ // Or('!X && Y', 'Z && !W') => '(!X || Z) && (!X || !W) && (Y || Z) && (Y || !W)'
			lhs: E{{F("X")}, {T("Y")}}, rhs: E{{T("Z")}, {F("W")}},
			expect: E{{F("X"), T("Z")}, {F("X"), F("W")}, {T("Y"), T("Z")}, {T("Y"), F("W")}},
		},
		{ // Or('X || Y', 'Z') => 'X || Y || Z'
			lhs: E{{T("X"), T("Y")}}, rhs: E{{T("Z")}},
			expect: E{{T("X"), T("Y"), T("Z")}},
		},
		{ // Or('!X || Y', 'Z') => '!X || Y || Z'
			lhs: E{{F("X"), T("Y")}}, rhs: E{{T("Z")}},
			expect: E{{F("X"), T("Y"), T("Z")}},
		},
		{ // Or('X || !Y', 'Z') => 'X || !Y || Z'
			lhs: E{{T("X"), F("Y")}}, rhs: E{{T("Z")}},
			expect: E{{T("X"), F("Y"), T("Z")}},
		},
		{ // Or('X || Y', '!Z') => 'X || Y || !Z'
			lhs: E{{T("X"), T("Y")}}, rhs: E{{F("Z")}},
			expect: E{{T("X"), T("Y"), F("Z")}},
		},
		{ // Or('X', 'Y || Z') => 'X || Y || Z'
			lhs: E{{T("X")}}, rhs: E{{T("Y"), T("Z")}},
			expect: E{{T("X"), T("Y"), T("Z")}},
		},
		{ // Or('!X', 'Y || Z') => '!X || Y || Z'
			lhs: E{{F("X")}}, rhs: E{{T("Y"), T("Z")}},
			expect: E{{F("X"), T("Y"), T("Z")}},
		},
		{ // Or('X', '!Y || Z') => 'X || !Y || Z'
			lhs: E{{T("X")}}, rhs: E{{F("Y"), T("Z")}},
			expect: E{{T("X"), F("Y"), T("Z")}},
		},
		{ // Or('X', 'Y || !Z') => 'X || Y || !Z'
			lhs: E{{T("X")}}, rhs: E{{T("Y"), F("Z")}},
			expect: E{{T("X"), T("Y"), F("Z")}},
		},
		{ // Or('X || Y', 'Y || Z') => 'X || Y || Y || Z'
			lhs: E{{T("X"), T("Y")}}, rhs: E{{T("Y"), T("Z")}},
			expect: E{{T("X"), T("Y"), T("Y"), T("Z")}},
		},
		{ // Or('!X || Y', 'Y || !Z') => '!X || Y || Y || !Z'
			lhs: E{{F("X"), T("Y")}}, rhs: E{{T("Y"), F("Z")}},
			expect: E{{F("X"), T("Y"), T("Y"), F("Z")}},
		},
		{ // Or('X || Y', 'Y && Z') => '(X || Y || Y) && (X || Y || Z)'
			lhs: E{{T("X"), T("Y")}}, rhs: E{{T("Y")}, {T("Z")}},
			expect: E{{T("X"), T("Y"), T("Y")}, {T("X"), T("Y"), T("Z")}},
		},
		{ // Or('!X && Y', 'Z || !W') => '(!X || Z || !W) && (Y || Z || !W)'
			lhs: E{{F("X")}, {T("Y")}}, rhs: E{{T("Z"), F("W")}},
			expect: E{{F("X"), T("Z"), F("W")}, {T("Y"), T("Z"), F("W")}},
		},
	} {
		got := cnf.Or(test.lhs, test.rhs)
		if diff := cmp.Diff(test.expect, got); diff != "" {
			t.Errorf("Or('%v', '%v') returned '%v'. Diff:\n%v", test.lhs, test.rhs, got, diff)
		}
	}
}

func TestNot(t *testing.T) {
	type E = cnf.Expr
	for _, test := range []struct {
		expr   cnf.Expr
		expect cnf.Expr
	}{
		{ // Not('') => ''
			expr:   E{},
			expect: E{},
		},
		{ // Not('X') => '!X'
			expr:   E{{T("X")}},
			expect: E{{F("X")}},
		},
		{ // Not('!X') => 'X'
			expr:   E{{F("X")}},
			expect: E{{T("X")}},
		},
		{ // Not('X || Y') => '!X && !Y'
			expr:   E{{T("X"), T("Y")}},
			expect: E{{F("X")}, {F("Y")}},
		},
		{ // Not('X || !Y') => '!X && Y'
			expr:   E{{T("X"), F("Y")}},
			expect: E{{F("X")}, {T("Y")}},
		},
		{ // Not('X && Y') => '!X || !Y'
			expr:   E{{T("X")}, {T("Y")}},
			expect: E{{F("X"), F("Y")}},
		},
		{ // Not('!X && Y') => 'X || !Y'
			expr:   E{{F("X")}, {T("Y")}},
			expect: E{{T("X"), F("Y")}},
		},
		{ // Not('(X || Y) && Z') => '(!X || !Z) && (!Y || !Z)'
			expr:   E{{T("X"), T("Y")}, {T("Z")}},
			expect: E{{F("X"), F("Z")}, {F("Y"), F("Z")}},
		},
		{ // Not('X && (Y || Z)') => '(!X || !Y) && (!X || !Z)'
			expr:   E{{T("X")}, {T("Y"), T("Z")}},
			expect: E{{F("X"), F("Y")}, {F("X"), F("Z")}},
		},
		{ // Not('X && (!Y || Z)') => '(!X || Y) && (!X || !Z)'
			expr:   E{{T("X")}, {F("Y"), T("Z")}},
			expect: E{{F("X"), T("Y")}, {F("X"), F("Z")}},
		},
		{ // Not('(X || Y) && (Z || W)') => '(!X || !Z) && (!X || !W) && (!Y || !Z) && (!Y || !W)'
			expr:   E{{T("X"), T("Y")}, {T("Z"), T("W")}},
			expect: E{{F("X"), F("Z")}, {F("X"), F("W")}, {F("Y"), F("Z")}, {F("Y"), F("W")}},
		},
		{ // Not('(X || !Y) && (!Z || W)') => '(!X || Z) && (!X || !W) && (Y || Z) && (Y || !W)'
			expr:   E{{T("X"), F("Y")}, {F("Z"), T("W")}},
			expect: E{{F("X"), T("Z")}, {F("X"), F("W")}, {T("Y"), T("Z")}, {T("Y"), F("W")}},
		},
	} {
		got := cnf.Not(test.expr)
		if diff := cmp.Diff(test.expect, got); diff != "" {
			t.Errorf("Not('%v') returned '%v'. Diff:\n%v", test.expr, got, diff)
		}
	}
}
