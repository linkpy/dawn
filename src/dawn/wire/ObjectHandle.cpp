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

#include "dawn/wire/ObjectHandle.h"

#include "dawn/common/Assert.h"

namespace dawn::wire {

ObjectHandle::ObjectHandle() = default;
ObjectHandle::ObjectHandle(ObjectId id, ObjectGeneration generation)
    : id(id), generation(generation) {
    DAWN_ASSERT(id != 0);
}

ObjectHandle::ObjectHandle(const volatile ObjectHandle& rhs)
    : id(rhs.id), generation(rhs.generation) {}
ObjectHandle& ObjectHandle::operator=(const volatile ObjectHandle& rhs) {
    id = rhs.id;
    generation = rhs.generation;
    return *this;
}

ObjectHandle::ObjectHandle(const ObjectHandle& rhs) = default;
ObjectHandle& ObjectHandle::operator=(const ObjectHandle& rhs) = default;

ObjectHandle& ObjectHandle::AssignFrom(const ObjectHandle& rhs) {
    id = rhs.id;
    generation = rhs.generation;
    return *this;
}
ObjectHandle& ObjectHandle::AssignFrom(const volatile ObjectHandle& rhs) {
    id = rhs.id;
    generation = rhs.generation;
    return *this;
}

bool ObjectHandle::IsValid() const {
    return id > 0;
}

}  // namespace dawn::wire
