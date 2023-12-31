// Copyright 2023 The Dawn Authors
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

package auth

import (
	"dawn.googlesource.com/dawn/tools/src/fileutils"
	"go.chromium.org/luci/auth"
	"go.chromium.org/luci/hardcoded/chromeinfra"
)

// DefaultAuthOptions returns the default authentication options for use by
// command line arguments.
func DefaultAuthOptions() auth.Options {
	def := chromeinfra.DefaultAuthOptions()
	def.SecretsDir = fileutils.ExpandHome("~/.config/dawn-cts")
	def.Scopes = append(def.Scopes, "https://www.googleapis.com/auth/gerritcodereview", auth.OAuthScopeEmail)
	return def
}
