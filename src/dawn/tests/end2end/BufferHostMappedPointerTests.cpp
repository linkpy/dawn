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

#include "dawn/tests/end2end/BufferHostMappedPointerTests.h"

namespace dawn {

std::pair<wgpu::Buffer, void*> BufferHostMappedPointerTestBackend::CreateHostMappedBuffer(
    wgpu::Device device,
    wgpu::BufferUsage usage,
    size_t size) {
    return CreateHostMappedBuffer(device, usage, size, [](void*) {});
}

std::vector<wgpu::FeatureName> BufferHostMappedPointerTests::GetRequiredFeatures() {
    if (!SupportsFeatures({wgpu::FeatureName::HostMappedPointer})) {
        return {};
    }
    return {wgpu::FeatureName::HostMappedPointer};
}

void BufferHostMappedPointerTests::SetUp() {
    DAWN_TEST_UNSUPPORTED_IF(UsesWire());
    DawnTestWithParams<BufferHostMappedPointerTestParams>::SetUp();
    DAWN_TEST_UNSUPPORTED_IF(!SupportsFeatures({wgpu::FeatureName::HostMappedPointer}));

    // TODO(crbug.com/dawn/2018): Expose a proper limit for the alignment.
    if (IsD3D12()) {
        mRequiredAlignment = 65536;
    } else {
        mRequiredAlignment = 4096;
    }
}

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(BufferHostMappedPointerTests);

namespace {

class BufferHostMappedPointerNoFeatureTests : public DawnTest {
    void SetUp() override {
        DawnTest::SetUp();
        DAWN_TEST_UNSUPPORTED_IF(UsesWire());
    }
};

// Test that the feature must be enabled to create buffers from host-mapped pointers.
TEST_P(BufferHostMappedPointerNoFeatureTests, Creation) {
    DAWN_TEST_UNSUPPORTED_IF(HasToggleEnabled("skip_validation"));

    wgpu::BufferHostMappedPointer hostMappedDesc;
    hostMappedDesc.pointer = nullptr;
    hostMappedDesc.disposeCallback = [](void* userdata) {};
    hostMappedDesc.userdata = nullptr;

    wgpu::BufferDescriptor bufferDesc;
    bufferDesc.usage = wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::MapWrite;
    bufferDesc.size = 1024;
    bufferDesc.nextInChain = &hostMappedDesc;

    ASSERT_DEVICE_ERROR_MSG(
        device.CreateBuffer(&bufferDesc),
        testing::HasSubstr(
            "SType::BufferHostMappedPointer requires FeatureName::HostMappedPointer"));
}

DAWN_INSTANTIATE_TEST(BufferHostMappedPointerNoFeatureTests,
                      D3D11Backend(),
                      D3D12Backend(),
                      VulkanBackend(),
                      MetalBackend(),
                      OpenGLBackend(),
                      OpenGLESBackend());

// Test that memory allocations must be aligned to the required alignment.
TEST_P(BufferHostMappedPointerTests, Alignment) {
    DAWN_TEST_UNSUPPORTED_IF(HasToggleEnabled("skip_validation"));

    // Invalid: half required alignment
    ASSERT_DEVICE_ERROR(GetParam().mBackend->CreateHostMappedBuffer(
        device, wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::MapWrite, mRequiredAlignment / 2u));

    GetParam().mBackend->CreateHostMappedBuffer(
        device, wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::MapWrite, mRequiredAlignment);

    // Invalid: just below required alignment
    ASSERT_DEVICE_ERROR(GetParam().mBackend->CreateHostMappedBuffer(
        device, wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::MapWrite, mRequiredAlignment - 1));

    // Invalid: just over required alignment
    ASSERT_DEVICE_ERROR(GetParam().mBackend->CreateHostMappedBuffer(
        device, wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::MapWrite, mRequiredAlignment + 1));

    // Valid: multiple of required alignment
    GetParam().mBackend->CreateHostMappedBuffer(
        device, wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::MapWrite, 2 * mRequiredAlignment);
}

// Test creating a buffer with data initially in the host-mapped memory.
// It should be GPU-visible immediately after creation.
// Then, change the host pointer, and see changes reflected on the GPU.
TEST_P(BufferHostMappedPointerTests, InitialDataAndCopySrc) {
    // Set up expected data.
    uint32_t bufferSize = mRequiredAlignment;
    std::vector<uint32_t> expected(bufferSize / sizeof(uint32_t));
    for (size_t i = 0; i < expected.size(); ++i) {
        expected[i] = i;
    }

    // Create the buffer and pre-fill it with data.
    auto [buffer, ptr] = GetParam().mBackend->CreateHostMappedBuffer(
        device, wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::MapWrite, bufferSize,
        [&](void* initialPtr) { memcpy(initialPtr, expected.data(), bufferSize); });

    // Check the buffer contents.
    EXPECT_BUFFER_U32_RANGE_EQ(expected.data(), buffer, 0, expected.size());

    // Wait for the GPU to complete, then change the host buffer contents.
    WaitForAllOperations();
    for (size_t i = 0; i < bufferSize / sizeof(uint32_t); ++i) {
        reinterpret_cast<uint32_t*>(ptr)[i] += 42;
    }

    // Expect to see the new contents in the buffer.
    for (auto& e : expected) {
        e += 42;
    }
    EXPECT_BUFFER_U32_RANGE_EQ(expected.data(), buffer, 0, expected.size());
}

// Create a host-mapped buffer with CopyDst usage. Test that changes on the GPU
// are visible to the host.
TEST_P(BufferHostMappedPointerTests, CopyDst) {
    // Set up expected data.
    uint32_t bufferSize = mRequiredAlignment;
    std::vector<uint32_t> expected(bufferSize / sizeof(uint32_t));
    for (size_t i = 0; i < expected.size(); ++i) {
        expected[i] = i;
    }

    // Create the buffer.
    auto [buffer, ptr] = GetParam().mBackend->CreateHostMappedBuffer(
        device, wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::MapRead, bufferSize);

    // Create another GPU buffer to use as the source.
    wgpu::BufferDescriptor bufferDesc;
    bufferDesc.size = bufferSize;
    bufferDesc.usage = wgpu::BufferUsage::CopySrc;
    bufferDesc.mappedAtCreation = true;
    wgpu::Buffer bufferSrc = device.CreateBuffer(&bufferDesc);

    // Fill the src buffer wth data.
    memcpy(bufferSrc.GetMappedRange(), expected.data(), bufferSize);
    bufferSrc.Unmap();

    // Do a GPU-GPU copy from the source buffer into the host-mapped buffer.
    wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
    encoder.CopyBufferToBuffer(bufferSrc, 0, buffer, 0, bufferSize);
    wgpu::CommandBuffer commandBuffer = encoder.Finish();
    device.GetQueue().Submit(1, &commandBuffer);

    // Wait for the GPU to complete.
    WaitForAllOperations();

    // Expect the changes to be reflected in the host pointer.
    EXPECT_EQ(memcmp(ptr, expected.data(), bufferSize), 0);
}

// Test interaction with other buffer mapping APIs.
TEST_P(BufferHostMappedPointerTests, Mapping) {
    DAWN_TEST_UNSUPPORTED_IF(HasToggleEnabled("skip_validation"));

    auto [buffer, _] = GetParam().mBackend->CreateHostMappedBuffer(
        device, wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::MapWrite, mRequiredAlignment);

    // Can't get mapped range from buffer.
    ASSERT_EQ(buffer.GetMappedRange(), nullptr);

    // Invalid to unmap a persistently host mapped buffer.
    ASSERT_DEVICE_ERROR(buffer.Unmap());

    // Invalid to map a persistently host mapped buffer.
    ASSERT_DEVICE_ERROR_MSG(
        buffer.MapAsync(wgpu::MapMode::Write, 0, wgpu::kWholeMapSize, nullptr, nullptr),
        testing::HasSubstr("cannot be mapped"));

    // Still invalid to GetMappedRange() or Unmap.
    ASSERT_EQ(buffer.GetMappedRange(), nullptr);
    ASSERT_DEVICE_ERROR(buffer.Unmap());

    // TODO(crbug.com/dawn/2018):
    // Test it is invalid to pass mappedAtCreation = true
}

// TODO(crbug.com/dawn/2018):
// - Figure out and test error handling. Is / when is the dispose callback
//   called when there is an error?

}  // anonymous namespace
}  // namespace dawn
