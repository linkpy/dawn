// Copyright 2017 The Dawn Authors
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

#ifndef SRC_DAWN_COMMON_BITSETITERATOR_H_
#define SRC_DAWN_COMMON_BITSETITERATOR_H_

#include <bitset>
#include <limits>

#include "dawn/common/Assert.h"
#include "dawn/common/Math.h"
#include "dawn/common/UnderlyingType.h"

namespace dawn {

// This is ANGLE's BitSetIterator class with a customizable return type.
// Types have been updated to be more specific.
// TODO(crbug.com/dawn/306): it could be optimized, in particular when N <= 64

template <typename T>
T roundUp(const T value, const T alignment) {
    auto temp = value + alignment - static_cast<T>(1);
    return temp - temp % alignment;
}

template <size_t N, typename T>
class BitSetIterator final {
  public:
    explicit BitSetIterator(const std::bitset<N>& bitset);
    BitSetIterator(const BitSetIterator& other);
    BitSetIterator& operator=(const BitSetIterator& other);

    class Iterator final {
      public:
        explicit Iterator(const std::bitset<N>& bits);
        Iterator& operator++();

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

        T operator*() const {
            using U = UnderlyingType<T>;
            DAWN_ASSERT(static_cast<U>(mCurrentBit) <= std::numeric_limits<U>::max());
            return static_cast<T>(static_cast<U>(mCurrentBit));
        }

      private:
        uint32_t getNextBit();

        static constexpr size_t kBitsPerWord = sizeof(uint32_t) * 8;
        std::bitset<N> mBits;
        uint32_t mCurrentBit;
        uint32_t mOffset;
    };

    Iterator begin() const { return Iterator(mBits); }
    Iterator end() const { return Iterator(std::bitset<N>(0)); }

  private:
    const std::bitset<N> mBits;
};

template <size_t N, typename T>
BitSetIterator<N, T>::BitSetIterator(const std::bitset<N>& bitset) : mBits(bitset) {}

template <size_t N, typename T>
BitSetIterator<N, T>::BitSetIterator(const BitSetIterator& other) : mBits(other.mBits) {}

template <size_t N, typename T>
BitSetIterator<N, T>& BitSetIterator<N, T>::operator=(const BitSetIterator& other) {
    mBits = other.mBits;
    return *this;
}

template <size_t N, typename T>
BitSetIterator<N, T>::Iterator::Iterator(const std::bitset<N>& bits)
    : mBits(bits), mCurrentBit(0), mOffset(0) {
    if (bits.any()) {
        mCurrentBit = getNextBit();
    } else {
        mOffset = static_cast<uint32_t>(roundUp(N, kBitsPerWord));
    }
}

template <size_t N, typename T>
typename BitSetIterator<N, T>::Iterator& BitSetIterator<N, T>::Iterator::operator++() {
    DAWN_ASSERT(mBits.any());
    mBits.set(mCurrentBit - mOffset, 0);
    mCurrentBit = getNextBit();
    return *this;
}

template <size_t N, typename T>
bool BitSetIterator<N, T>::Iterator::operator==(const Iterator& other) const {
    return mOffset == other.mOffset && mBits == other.mBits;
}

template <size_t N, typename T>
bool BitSetIterator<N, T>::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

template <size_t N, typename T>
uint32_t BitSetIterator<N, T>::Iterator::getNextBit() {
    static std::bitset<N> wordMask(std::numeric_limits<uint32_t>::max());

    while (mOffset < N) {
        uint32_t wordBits = static_cast<uint32_t>((mBits & wordMask).to_ulong());
        if (wordBits != 0ul) {
            return ScanForward(wordBits) + mOffset;
        }

        mBits >>= kBitsPerWord;
        mOffset += kBitsPerWord;
    }
    return 0;
}

// Helper to avoid needing to specify the template parameter size
template <size_t N>
BitSetIterator<N, uint32_t> IterateBitSet(const std::bitset<N>& bitset) {
    return BitSetIterator<N, uint32_t>(bitset);
}

}  // namespace dawn

#endif  // SRC_DAWN_COMMON_BITSETITERATOR_H_
