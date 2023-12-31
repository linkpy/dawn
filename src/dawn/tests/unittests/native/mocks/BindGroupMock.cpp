// Copyright 2022 The Dawn Authors
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

#include "dawn/tests/unittests/native/mocks/BindGroupMock.h"

namespace dawn::native {

BindGroupMock::BindGroupMock(DeviceMock* device, const BindGroupDescriptor* descriptor)
    : BindGroupDataHolder(descriptor->layout->GetInternalBindGroupLayout()->GetBindingDataSize()),
      BindGroupBase(device, descriptor, mBindingDataAllocation) {
    ON_CALL(*this, DestroyImpl).WillByDefault([this] { this->BindGroupBase::DestroyImpl(); });
}

BindGroupMock::~BindGroupMock() = default;

}  // namespace dawn::native
