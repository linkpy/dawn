// Copyright 2021 The Dawn Authors
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

#ifndef SRC_DAWN_TESTS_WHITE_BOX_VULKANIMAGEWRAPPINGTESTS_H_
#define SRC_DAWN_TESTS_WHITE_BOX_VULKANIMAGEWRAPPINGTESTS_H_

#include <memory>
#include <vector>

// This must be above all other includes otherwise VulkanBackend.h includes vulkan.h before we had
// time to wrap it with vulkan_platform.h
#include "dawn/common/vulkan_platform.h"

#include "dawn/common/NonCopyable.h"
#include "dawn/native/VulkanBackend.h"
#include "dawn/webgpu_cpp.h"

namespace dawn::native::vulkan {

struct ExternalImageDescriptorVkForTesting;
struct ExternalImageExportInfoVkForTesting;

class VulkanImageWrappingTestBackend {
  public:
    virtual ~VulkanImageWrappingTestBackend() = default;

    class ExternalTexture : NonCopyable {
      public:
        virtual ~ExternalTexture() = default;
    };
    class ExternalSemaphore : NonCopyable {
      public:
        virtual ~ExternalSemaphore() = default;
    };

    // Test parameters passed from the wrapping tests so that they can be used by the test
    // backends. The DAWN_TEST_PARAM_STRUCT is not declared here because it is unnecessary but also
    // because it declares a bunch of functions that would cause ODR violations.
    struct TestParams {
        ExternalImageType externalImageType = ExternalImageType::OpaqueFD;
        bool useDedicatedAllocation = false;
        bool detectDedicatedAllocation = false;
    };
    void SetParam(const TestParams& params);
    const TestParams& GetParam() const;
    virtual bool SupportsTestParams(const TestParams& params) const = 0;

    virtual std::unique_ptr<ExternalTexture> CreateTexture(uint32_t width,
                                                           uint32_t height,
                                                           wgpu::TextureFormat format,
                                                           wgpu::TextureUsage usage) = 0;
    virtual wgpu::Texture WrapImage(const wgpu::Device& device,
                                    const ExternalTexture* texture,
                                    const ExternalImageDescriptorVkForTesting& descriptor,
                                    std::vector<std::unique_ptr<ExternalSemaphore>> semaphores) = 0;

    virtual bool ExportImage(const wgpu::Texture& texture,
                             ExternalImageExportInfoVkForTesting* exportInfo) = 0;

    static std::unique_ptr<VulkanImageWrappingTestBackend> Create(const wgpu::Device& device,
                                                                  const TestParams params);

  private:
    TestParams mParams;
};

struct ExternalImageDescriptorVkForTesting : public ExternalImageDescriptorVk {
  public:
    explicit ExternalImageDescriptorVkForTesting(ExternalImageType type)
        : ExternalImageDescriptorVk(type) {}
};

struct ExternalImageExportInfoVkForTesting : public ExternalImageExportInfoVk {
  public:
    explicit ExternalImageExportInfoVkForTesting(ExternalImageType type)
        : ExternalImageExportInfoVk(type) {}
    std::vector<std::unique_ptr<VulkanImageWrappingTestBackend::ExternalSemaphore>> semaphores;
};

}  // namespace dawn::native::vulkan

#endif  // SRC_DAWN_TESTS_WHITE_BOX_VULKANIMAGEWRAPPINGTESTS_H_
