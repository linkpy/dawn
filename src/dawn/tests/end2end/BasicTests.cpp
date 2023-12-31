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

#include "dawn/common/FutureUtils.h"
#include "dawn/tests/DawnTest.h"
#include "dawn/utils/WGPUHelpers.h"

namespace dawn {
namespace {

class BasicTests : public DawnTest {};

// Test adapter filter by vendor id.
TEST_P(BasicTests, VendorIdFilter) {
    DAWN_TEST_UNSUPPORTED_IF(!HasVendorIdFilter());

    ASSERT_EQ(GetAdapterProperties().vendorID, GetVendorIdFilter());
}

// Test adapter filter by backend type.
TEST_P(BasicTests, BackendType) {
    DAWN_TEST_UNSUPPORTED_IF(!HasBackendTypeFilter());

    ASSERT_EQ(GetAdapterProperties().backendType, GetBackendTypeFilter());
}

// Test Queue::WriteBuffer changes the content of the buffer, but really this is the most
// basic test possible, and tests the test harness
TEST_P(BasicTests, QueueWriteBuffer) {
    wgpu::BufferDescriptor descriptor;
    descriptor.size = 4;
    descriptor.usage = wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::CopyDst;
    wgpu::Buffer buffer = device.CreateBuffer(&descriptor);

    uint32_t value = 0x01020304;
    queue.WriteBuffer(buffer, 0, &value, sizeof(value));

    EXPECT_BUFFER_U32_EQ(value, buffer, 0);
}

// Test a validation error for Queue::WriteBuffer but really this is the most basic test possible
// for ASSERT_DEVICE_ERROR
TEST_P(BasicTests, QueueWriteBufferError) {
    wgpu::BufferDescriptor descriptor;
    descriptor.size = 4;
    descriptor.usage = wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::CopyDst;
    wgpu::Buffer buffer = device.CreateBuffer(&descriptor);

    uint8_t value = 187;
    ASSERT_DEVICE_ERROR(queue.WriteBuffer(buffer, 1000, &value, sizeof(value)));
}

TEST_P(BasicTests, GetInstanceFeatures) {
    wgpu::InstanceFeatures instanceFeatures{};
    bool success = wgpu::GetInstanceFeatures(&instanceFeatures);
    EXPECT_TRUE(success);
    EXPECT_EQ(instanceFeatures.timedWaitAnyEnable, !UsesWire());
    EXPECT_EQ(instanceFeatures.timedWaitAnyMaxCount, kTimedWaitAnyMaxCountDefault);
    EXPECT_EQ(instanceFeatures.nextInChain, nullptr);

    wgpu::ChainedStruct chained{};
    instanceFeatures.nextInChain = &chained;
    success = wgpu::GetInstanceFeatures(&instanceFeatures);
    EXPECT_FALSE(success);
}

DAWN_INSTANTIATE_TEST(BasicTests,
                      D3D11Backend(),
                      D3D12Backend(),
                      MetalBackend(),
                      OpenGLBackend(),
                      OpenGLESBackend(),
                      VulkanBackend());

}  // anonymous namespace
}  // namespace dawn
