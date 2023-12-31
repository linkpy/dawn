// Copyright 2020 The Dawn Authors
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

#ifndef SRC_DAWN_COMMON_ITYP_VECTOR_H_
#define SRC_DAWN_COMMON_ITYP_VECTOR_H_

#include <limits>
#include <type_traits>
#include <vector>

#include "dawn/common/TypedInteger.h"
#include "dawn/common/UnderlyingType.h"

namespace dawn::ityp {

// ityp::vector is a helper class that wraps std::vector with the restriction that
// indices must be a particular type |Index|.
template <typename Index, typename Value>
class vector : public std::vector<Value> {
    using I = UnderlyingType<Index>;
    using Base = std::vector<Value>;

  private:
    // Disallow access to base constructors and untyped index/size-related operators.
    using Base::Base;
    using Base::operator=;
    using Base::operator[];
    using Base::at;
    using Base::reserve;
    using Base::resize;
    using Base::size;

  public:
    vector() : Base() {}

    explicit vector(Index size) : Base(static_cast<I>(size)) {}

    vector(Index size, const Value& init) : Base(static_cast<I>(size), init) {}

    vector(const vector& rhs) : Base(static_cast<const Base&>(rhs)) {}

    vector(vector&& rhs) : Base(static_cast<Base&&>(rhs)) {}

    vector(std::initializer_list<Value> init) : Base(init) {}

    vector& operator=(const vector& rhs) {
        Base::operator=(static_cast<const Base&>(rhs));
        return *this;
    }

    vector& operator=(vector&& rhs) noexcept {
        Base::operator=(static_cast<Base&&>(rhs));
        return *this;
    }

    Value& operator[](Index i) {
        DAWN_ASSERT(i >= Index(0) && i < size());
        return Base::operator[](static_cast<I>(i));
    }

    constexpr const Value& operator[](Index i) const {
        DAWN_ASSERT(i >= Index(0) && i < size());
        return Base::operator[](static_cast<I>(i));
    }

    Value& at(Index i) {
        DAWN_ASSERT(i >= Index(0) && i < size());
        return Base::at(static_cast<I>(i));
    }

    constexpr const Value& at(Index i) const {
        DAWN_ASSERT(i >= Index(0) && i < size());
        return Base::at(static_cast<I>(i));
    }

    constexpr Index size() const {
        DAWN_ASSERT(std::numeric_limits<I>::max() >= Base::size());
        return Index(static_cast<I>(Base::size()));
    }

    void resize(Index size) { Base::resize(static_cast<I>(size)); }

    void reserve(Index size) { Base::reserve(static_cast<I>(size)); }
};

}  // namespace dawn::ityp

#endif  // SRC_DAWN_COMMON_ITYP_VECTOR_H_
